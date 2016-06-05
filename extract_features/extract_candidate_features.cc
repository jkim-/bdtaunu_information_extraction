#include <iostream>
#include <vector>
#include <fstream>

#include <PsqlReader.h>
#include <pgstring_convert.h>

#include <boost/program_options.hpp>

#include "RecoFeatureExtractor.h"

namespace po = boost::program_options;

void extract_candidate_features(const po::variables_map &vm);

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
             "name of the table containing the truth match inputs. ")
        ("output_fname", po::value<std::string>(), 
             "output csv file name to store extracted result. ")
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
      std::cout << "Usage: ./extract_candidate_features ";
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
    extract_candidate_features(vm);

  } catch(std::exception& e) {

    std::cerr << "error: " << e.what() << "\n";
    return 1;

  } catch(...) {

    std::cerr << "Exception of unknown type!\n";
    return 1;
  }

  return 0;
}

void extract_candidate_features(const po::variables_map &vm) {

  // open database connection and populate fields
  std::string dbname = vm["dbname"].as<std::string>();
  std::string table_name = vm["table_name"].as<std::string>();
  int cursor_fetch_size = vm["cursor_fetch_size"].as<int>();

  PsqlReader psql;
  psql.open_connection("dbname="+dbname);
  psql.open_cursor(table_name, 
      { "eid", 
        "reco_n_vertices", "reco_n_edges", 
        "reco_from_vertices", "reco_to_vertices", "reco_lund_id", 
        "y_reco_idx", "b_reco_idx", "d_reco_idx", "c_reco_idx", 
        "h_reco_idx", "l_reco_idx", "gamma_reco_idx", 
        "ltrkidx", "htrkidx", "eselectorsmap", "muselectorsmap",
        "dmass"}, 
        cursor_fetch_size);

  int eid;
  int reco_n_vertices, reco_n_edges;
  std::vector<int> reco_from_vertices, reco_to_vertices, reco_lund_id;
  std::vector<int> y_reco_idx, b_reco_idx, d_reco_idx, c_reco_idx;
  std::vector<int> h_reco_idx, l_reco_idx, gamma_reco_idx;
  std::vector<int> ltrkidx, htrkidx, eselectorsmap, muselectorsmap;
  std::vector<float> dmass;

  // open output file and write title line
  std::string output_fname = vm["output_fname"].as<std::string>();
  std::ofstream fout; fout.open(output_fname);
  fout << "eid,y_tagb_idx,y_sigb_idx,b_d_idx,b_lh_idx,b_istag,";
  fout << "d_isdstar,d_dmode,d_dstarmode,d_dmass,";
  fout << "h_epid,h_mupid,l_epid,l_mupid" << std::endl;

  // main loop
  size_t n_records = 0;
  while (psql.next()) {

    ++n_records;

    // load record information
    pgstring_convert(psql.get("eid"), eid);
    pgstring_convert(psql.get("reco_n_vertices"), reco_n_vertices);
    pgstring_convert(psql.get("reco_n_edges"), reco_n_edges);
    pgstring_convert(psql.get("reco_from_vertices"), reco_from_vertices);
    pgstring_convert(psql.get("reco_to_vertices"), reco_to_vertices);
    pgstring_convert(psql.get("reco_lund_id"), reco_lund_id);
    pgstring_convert(psql.get("y_reco_idx"), y_reco_idx);
    pgstring_convert(psql.get("b_reco_idx"), b_reco_idx);
    pgstring_convert(psql.get("d_reco_idx"), d_reco_idx);
    pgstring_convert(psql.get("c_reco_idx"), c_reco_idx);
    pgstring_convert(psql.get("h_reco_idx"), h_reco_idx);
    pgstring_convert(psql.get("l_reco_idx"), l_reco_idx);
    pgstring_convert(psql.get("gamma_reco_idx"), gamma_reco_idx);
    pgstring_convert(psql.get("ltrkidx"), ltrkidx);
    pgstring_convert(psql.get("htrkidx"), htrkidx);
    pgstring_convert(psql.get("eselectorsmap"), eselectorsmap);
    pgstring_convert(psql.get("muselectorsmap"), muselectorsmap);
    pgstring_convert(psql.get("dmass"), dmass);


    // extract features
    RecoFeatureExtractor extractor;
    extractor.set_data(reco_n_vertices, reco_n_edges, 
                       reco_from_vertices, reco_to_vertices, reco_lund_id,
                       y_reco_idx, b_reco_idx, d_reco_idx, c_reco_idx,
                       h_reco_idx, l_reco_idx, gamma_reco_idx, 
                       ltrkidx, htrkidx, eselectorsmap, muselectorsmap, 
                       dmass);

    // write a line
    fout << eid << ",";
    fout << vector2pgstring(extractor.get_y_tagb_idx()) << ",";
    fout << vector2pgstring(extractor.get_y_sigb_idx()) << ",";
    fout << vector2pgstring(extractor.get_b_d_idx()) << ",";
    fout << vector2pgstring(extractor.get_b_lh_idx()) << ",";
    fout << vector2pgstring(extractor.get_b_is_tag()) << ",";
    fout << vector2pgstring(extractor.get_d_is_dstar()) << ",";
    fout << vector2pgstring(extractor.get_d_dmode()) << ",";
    fout << vector2pgstring(extractor.get_d_dstarmode()) << ",";
    fout << vector2pgstring(extractor.get_d_dmass()) << ",";
    fout << vector2pgstring(extractor.get_h_epid()) << ",";
    fout << vector2pgstring(extractor.get_h_mupid()) << ",";
    fout << vector2pgstring(extractor.get_l_epid()) << ",";
    fout << vector2pgstring(extractor.get_l_mupid());
    fout << std::endl;
  }

  // close file
  fout.close();

  // close database connection
  psql.close_cursor();
  psql.close_connection();

  std::cout << "processed " << n_records << " rows. " << std::endl;

}
