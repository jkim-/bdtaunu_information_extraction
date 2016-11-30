#include <iostream>
#include <fstream>
#include <vector>

#include <PsqlReader.h>
#include <CsvWriter.h>
#include <pgstring_utils.h>

#include "McLundGraph.h"
#include "McDecayModeCurator.h"
#include "McBTypeAnalyzer.h"

#include <boost/program_options.hpp>

namespace po = boost::program_options;
namespace pu = pgstring_utils;

void extract_mcbtypes(const po::variables_map &vm);

int main(int argc, char **argv) {

  try {

    // define program options
    po::options_description generic("Generic options");
    generic.add_options()
        ("help,h", "produce help message")
    ;

    po::options_description config("Configuration options");
    config.add_options()
        ("dbname", po::value<std::string>(), 
             "database name. ")
        ("table_name", po::value<std::string>(), 
             "name of the table containing framework tuples. ")
        ("output_fname", po::value<std::string>(), 
             "output csv file name. ")
        ("cursor_fetch_size", po::value<int>()->default_value(5000), 
             "number of rows per cursor fetch. ")
    ;

    po::options_description hidden("Hidden options");
    hidden.add_options()
        ("config_file", po::value<std::string>(), 
             "name of a configuration file. ")
    ;

    po::options_description cmdline_options;
    cmdline_options.add(generic).add(config).add(hidden);

    po::options_description config_file_options;
    config_file_options.add(config);

    po::options_description visible;
    visible.add(generic).add(config);

    po::positional_options_description p;
    p.add("config_file", -1);

    // parse program options and configuration file
    po::variables_map vm;
    store(po::command_line_parser(argc, argv).
          options(cmdline_options).positional(p).run(), vm);
    notify(vm);

    if (vm.count("help") || !vm.count("config_file")) {
      std::cout << std::endl;
      std::cout << "Usage: ./extract_mcbtypes ";
      std::cout << "[options] config_fname" << std::endl;
      std::cout << visible << "\n";
      return 0;
    }

    std::ifstream fin(vm["config_file"].as<std::string>());
    if (!fin) {
      std::cout << "cannot open config file: ";
      std::cout << vm["config_file"].as<std::string>() << std::endl;
      return 0;
    }

    store(parse_config_file(fin, config_file_options), vm);
    notify(vm);

    // main routine
    extract_mcbtypes(vm);

  } catch(std::exception& e) {

    std::cerr << "error: " << e.what() << "\n";
    return 1;

  } catch(...) {

    std::cerr << "Exception of unknown type!\n";
    return 1;
  }

  return 0;
}


void extract_mcbtypes(const po::variables_map &vm) {

  // open database connection and populate fields
  std::string dbname = vm["dbname"].as<std::string>();
  std::string table_name = vm["table_name"].as<std::string>();
  int cursor_fetch_size = vm["cursor_fetch_size"].as<int>();

  // open database connection and populate fields
  PsqlReader psql;
  psql.open_connection("dbname="+dbname);
  psql.open_cursor(table_name, { 
        "eid", 
        "mc_n_vertices", "mc_n_edges",
        "mc_from_vertices", "mc_to_vertices", 
        "mc_lund_id"
      }, cursor_fetch_size);

  // open output csv file
  std::string output_fname = vm["output_fname"].as<std::string>();
  CsvWriter csv;
  csv.open(output_fname, {
      "eid", "b1_mctype", "b2_mctype", "b1tau_mctype", "b2tau_mctype"
  });

  // initialize worker classes
  McLundGraphFactory graph_factory;
  McDecayModeCurator curator;
  McDecayModeSummary summary;
  McBTypeAnalyzer analyzer;

  // main loop to extract information 
  int eid, mc_n_vertices, mc_n_edges;
  std::vector<int> mc_from_vertices, mc_to_vertices, mc_lund_id;
  while (psql.next()) {

    pu::string2type(psql.get("eid"), eid);
    pu::string2type(psql.get("mc_n_vertices"), mc_n_vertices);
    pu::string2type(psql.get("mc_n_edges"), mc_n_edges);
    pu::string2type(psql.get("mc_from_vertices"), mc_from_vertices);
    pu::string2type(psql.get("mc_to_vertices"), mc_to_vertices);
    pu::string2type(psql.get("mc_lund_id"), mc_lund_id);

    McLundGraph g = graph_factory.create_graph(
        mc_n_vertices, mc_n_edges, 
        mc_from_vertices, mc_to_vertices, mc_lund_id);

    curator.curate(g, summary);
    analyzer.analyze(g, summary);

    csv.set("eid", pu::type2string(eid));
    csv.set("b1_mctype", analyzer.b1_mctype_string());
    csv.set("b2_mctype", analyzer.b2_mctype_string());
    csv.set("b1tau_mctype", analyzer.b1tau_mctype_string());
    csv.set("b2tau_mctype", analyzer.b2tau_mctype_string());

    csv.commit();
  }
 
  // close database connection
  psql.close_cursor();
  psql.close_connection();

  // close output file
  csv.close();

}

