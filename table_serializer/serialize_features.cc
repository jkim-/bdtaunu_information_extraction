#include <iostream>
#include <vector>
#include <fstream>

#include <PsqlReader.h>
#include <CsvWriter.h>
#include <pgstring_utils.h>

#include <boost/program_options.hpp>

namespace po = boost::program_options;
namespace pu = pgstring_utils;

void serialize_features(const po::variables_map &vm);

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
             "name of the table containing all the unserialized features. ")
        ("event_output_fname", po::value<std::string>(), 
             "output csv file name to store the event serialization. ")
        ("candidate_output_fname", po::value<std::string>(), 
             "output csv file name to store the candidate serialization. ")
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
      std::cout << "Usage: ./serialize_features ";
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
    serialize_features(vm);

  } catch(std::exception& e) {

    std::cerr << "error: " << e.what() << "\n";
    return 1;

  } catch(...) {

    std::cerr << "Exception of unknown type!\n";
    return 1;
  }

  return 0;
}

void serialize_features(const po::variables_map &vm) {

  // open database connection and populate fields
  std::string dbname = vm["dbname"].as<std::string>();
  std::string table_name = vm["table_name"].as<std::string>();
  int cursor_fetch_size = vm["cursor_fetch_size"].as<int>();

  PsqlReader psql;
  psql.open_connection("dbname="+dbname);
  psql.open_cursor(table_name, { 
        "eid", "ny",
        "ntracks", "r2", "r2all",
        "ybpairmmiss2", "ybpairmmissprime2", 
        "ybpaireextra50", "ybpaircosthetat",
        "ytagblp3magcm", "ytagbcosby", "ytagbcosthetadlcm",
        "ytagbdstardeltam", "ytagbcosthetadsoftcm", "ytagbsoftp3magcm",
        "ysigbhp3magcm", "ysigbcosby", "ysigbcosthetadtaucm",
        "ysigbvtxprobb", "ysigbdstardeltam", "ysigbcosthetadsoftcm",
        "ysigbsoftp3magcm", "ysigbhmass", "ysigbvtxprobh",
        "y_tagb_idx", "y_sigb_idx",
        "b_istag", "b_d_idx", "b_lh_idx",
        "d_isdstar", "d_dmode", "d_dstarmode",
        "h_epid", "h_mupid", "l_epid", "l_mupid",
        "d_dmass"
      }, cursor_fetch_size);

  int eid, ny, ntracks;
  float r2, r2all;
  std::vector<float> ybpairmmiss2, ybpairmmissprime2; 
  std::vector<float> ybpaireextra50, ybpaircosthetat;
  std::vector<float> ytagblp3magcm, ytagbcosby, ytagbcosthetadlcm;
  std::vector<float> ytagbdstardeltam, ytagbcosthetadsoftcm, ytagbsoftp3magcm;
  std::vector<float> ysigbhp3magcm, ysigbcosby, ysigbcosthetadtaucm;
  std::vector<float> ysigbvtxprobb, ysigbdstardeltam, ysigbcosthetadsoftcm;
  std::vector<float> ysigbsoftp3magcm, ysigbhmass, ysigbvtxprobh;
  std::vector<int> y_tagb_idx, y_sigb_idx;
  std::vector<int> b_istag, b_d_idx, b_lh_idx;
  std::vector<int> d_isdstar, d_dmode, d_dstarmode;
  std::vector<int> h_epid, h_mupid, l_epid, l_mupid;
  std::vector<float> d_dmass;

  // open output file for writing
  std::string event_output_fname = vm["event_output_fname"].as<std::string>();
  std::string candidate_output_fname = vm["candidate_output_fname"].as<std::string>();

  CsvWriter event_csv;
  event_csv.open(event_output_fname, {
      "eid", "ny", "ntracks", "r2", "r2all"
  });

  CsvWriter cand_csv;
  cand_csv.open(candidate_output_fname, {
      "eid", "cidx", "mmiss2", "mmiss2prime", "eextra",
      "costhetat", "tag_lp3", "tag_cosby", "tag_costhetadl",
      "tag_isbdstar", "tag_dmass", "tag_deltam", "tag_costhetadsoft",
      "tag_softp3magcm", "sig_hp3", "sig_cosby", "sig_costhetadtau",
      "sig_vtxb", "sig_isbdstar", "sig_dmass", "sig_deltam",
      "sig_costhetadsoft", "sig_softp3magcm", "sig_hmass", "sig_vtxh",
      "tag_dmode", "tag_dstarmode", "sig_dmode", "sig_dstarmode",
      "tag_l_epid", "tag_l_mupid", "sig_h_epid", "sig_h_mupid" 
  });

  // main loop
  size_t n_records = 0;
  while (psql.next()) {

    ++n_records;

    // load inputs
    pu::string2type(psql.get("eid"), eid);
    pu::string2type(psql.get("ny"), ny);
    pu::string2type(psql.get("ntracks"), ntracks);
    pu::string2type(psql.get("r2"), r2);
    pu::string2type(psql.get("r2all"), r2all);
    pu::string2type(psql.get("ybpairmmiss2"), ybpairmmiss2);
    pu::string2type(psql.get("ybpairmmissprime2"), ybpairmmissprime2);
    pu::string2type(psql.get("ybpaireextra50"), ybpaireextra50);
    pu::string2type(psql.get("ybpaircosthetat"), ybpaircosthetat);
    pu::string2type(psql.get("ytagblp3magcm"), ytagblp3magcm);
    pu::string2type(psql.get("ytagbcosby"), ytagbcosby);
    pu::string2type(psql.get("ytagbcosthetadlcm"), ytagbcosthetadlcm);
    pu::string2type(psql.get("ytagbdstardeltam"), ytagbdstardeltam);
    pu::string2type(psql.get("ytagbcosthetadsoftcm"), ytagbcosthetadsoftcm);
    pu::string2type(psql.get("ytagbsoftp3magcm"), ytagbsoftp3magcm);
    pu::string2type(psql.get("ysigbhp3magcm"), ysigbhp3magcm);
    pu::string2type(psql.get("ysigbcosby"), ysigbcosby);
    pu::string2type(psql.get("ysigbcosthetadtaucm"), ysigbcosthetadtaucm);
    pu::string2type(psql.get("ysigbvtxprobb"), ysigbvtxprobb);
    pu::string2type(psql.get("ysigbdstardeltam"), ysigbdstardeltam);
    pu::string2type(psql.get("ysigbcosthetadsoftcm"), ysigbcosthetadsoftcm);
    pu::string2type(psql.get("ysigbsoftp3magcm"), ysigbsoftp3magcm);
    pu::string2type(psql.get("ysigbhmass"), ysigbhmass);
    pu::string2type(psql.get("ysigbvtxprobh"), ysigbvtxprobh);
    pu::string2type(psql.get("y_tagb_idx"), y_tagb_idx);
    pu::string2type(psql.get("y_sigb_idx"), y_sigb_idx);
    pu::string2type(psql.get("b_istag"), b_istag);
    pu::string2type(psql.get("b_d_idx"), b_d_idx);
    pu::string2type(psql.get("b_lh_idx"), b_lh_idx);
    pu::string2type(psql.get("d_isdstar"), d_isdstar);
    pu::string2type(psql.get("d_dmode"), d_dmode);
    pu::string2type(psql.get("d_dstarmode"), d_dstarmode);
    pu::string2type(psql.get("h_epid"), h_epid);
    pu::string2type(psql.get("h_mupid"), h_mupid);
    pu::string2type(psql.get("l_epid"), l_epid);
    pu::string2type(psql.get("l_mupid"), l_mupid);
    pu::string2type(psql.get("d_dmass"), d_dmass);


    // serialize event features
    event_csv.set("eid", pu::type2string(eid));
    event_csv.set("ny", pu::type2string(ny));
    event_csv.set("ntracks", pu::type2string(ntracks));
    event_csv.set("r2", pu::type2string(r2));
    event_csv.set("r2all", pu::type2string(r2all));
    event_csv.commit();

    // serialize candidate features
    for (int cidx = 0; cidx < ny; ++cidx) {

      cand_csv.set("eid", 
          pu::type2string(eid));
      cand_csv.set("cidx", 
          pu::type2string(cidx));
      cand_csv.set("mmiss2", 
          pu::type2string(ybpairmmiss2[cidx]));
      cand_csv.set("mmiss2prime", 
          pu::type2string(ybpairmmissprime2[cidx]));
      cand_csv.set("eextra", 
          pu::type2string(ybpaireextra50[cidx]));
      cand_csv.set("costhetat", 
          pu::type2string(ybpaircosthetat[cidx]));
      cand_csv.set("tag_lp3", 
          pu::type2string(ytagblp3magcm[cidx]));
      cand_csv.set("tag_cosby", 
          pu::type2string(ytagbcosby[cidx]));
      cand_csv.set("tag_costhetadl", 
          pu::type2string(ytagbcosthetadlcm[cidx]));
      cand_csv.set("tag_isbdstar", 
          pu::type2string(d_isdstar[b_d_idx[y_tagb_idx[cidx]]]));
      cand_csv.set("tag_dmass", 
          pu::type2string(d_dmass[b_d_idx[y_tagb_idx[cidx]]]));
      cand_csv.set("tag_deltam", 
          pu::type2string(ytagbdstardeltam[cidx]));
      cand_csv.set("tag_costhetadsoft", 
          pu::type2string(ytagbcosthetadsoftcm[cidx]));
      cand_csv.set("tag_softp3magcm", 
          pu::type2string(ytagbsoftp3magcm[cidx]));
      cand_csv.set("sig_hp3", 
          pu::type2string(ysigbhp3magcm[cidx]));
      cand_csv.set("sig_cosby", 
          pu::type2string(ysigbcosby[cidx]));
      cand_csv.set("sig_costhetadtau", 
          pu::type2string(ysigbcosthetadtaucm[cidx]));
      cand_csv.set("sig_vtxb", 
          pu::type2string(ysigbvtxprobb[cidx]));
      cand_csv.set("sig_isbdstar", 
          pu::type2string(d_isdstar[b_d_idx[y_sigb_idx[cidx]]]));
      cand_csv.set("sig_dmass", 
          pu::type2string(d_dmass[b_d_idx[y_sigb_idx[cidx]]]));
      cand_csv.set("sig_deltam", 
          pu::type2string(ysigbdstardeltam[cidx]));
      cand_csv.set("sig_costhetadsoft", 
          pu::type2string(ysigbcosthetadsoftcm[cidx]));
      cand_csv.set("sig_softp3magcm", 
          pu::type2string(ysigbsoftp3magcm[cidx]));
      cand_csv.set("sig_hmass", 
          pu::type2string(ysigbhmass[cidx]));
      cand_csv.set("sig_vtxh", 
          pu::type2string(ysigbvtxprobh[cidx]));
      cand_csv.set("tag_dmode", 
          pu::type2string(d_dmode[b_d_idx[y_tagb_idx[cidx]]]));
      cand_csv.set("tag_dstarmode", 
          pu::type2string(d_dstarmode[b_d_idx[y_tagb_idx[cidx]]]));
      cand_csv.set("sig_dmode", 
          pu::type2string(d_dmode[b_d_idx[y_sigb_idx[cidx]]]));
      cand_csv.set("sig_dstarmode", 
          pu::type2string(d_dstarmode[b_d_idx[y_sigb_idx[cidx]]]));
      cand_csv.set("tag_l_epid", 
          pu::type2string(l_epid[b_lh_idx[y_tagb_idx[cidx]]]));
      cand_csv.set("tag_l_mupid", 
          pu::type2string(l_mupid[b_lh_idx[y_tagb_idx[cidx]]]));
      cand_csv.set("sig_h_epid", 
          pu::type2string(h_epid[b_lh_idx[y_sigb_idx[cidx]]]));
      cand_csv.set("sig_h_mupid", 
          pu::type2string(h_mupid[b_lh_idx[y_sigb_idx[cidx]]]));

      cand_csv.commit();
    }
  }

  // close files
  event_csv.close();
  cand_csv.close();

  // close database connection
  psql.close_cursor();
  psql.close_connection();

  std::cout << "processed " << n_records << " rows. " << std::endl;

}
