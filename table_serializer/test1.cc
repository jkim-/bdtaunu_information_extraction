#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#include <PsqlReader.h>
#include <pgstring_utils.h>

namespace pu = pgstring_utils;

std::string header() {

  std::string s;

  s += "eid,";
  s += "cidx,";
  s += "mmiss2,";
  s += "mmiss2prime,";
  s += "eextra,";
  s += "costhetat,";
  s += "tag_lp3,";
  s += "tag_cosby,";
  s += "tag_costhetadl,";
  s += "tag_isbdstar,";
  s += "tag_dmass,";
  s += "tag_deltam,";
  s += "tag_costhetadsoft,";
  s += "tag_softp3magcm,";
  s += "sig_hp3,";
  s += "sig_cosby,";
  s += "sig_costhetadtau,";
  s += "sig_vtxb,";
  s += "sig_isbdstar,";
  s += "sig_dmass,";
  s += "sig_deltam,";
  s += "sig_costhetadsoft,";
  s += "sig_softp3magcm,";
  s += "sig_hmass,";
  s += "sig_vtxh,";
  s += "tag_dmode,";
  s += "tag_dstarmode,";
  s += "sig_dmode,";
  s += "sig_dstarmode,";
  s += "tag_l_epid,";
  s += "tag_l_mupid,";
  s += "sig_h_epid,";
  s += "sig_h_mupid,";

  s.pop_back();

  return s;
}

int main() {
  // open database connection and populate fields
  std::string dbname = "testing";
  std::string table_name = "serializer_inputs_sigmc";
  int cursor_fetch_size = 5000;

  PsqlReader psql;
  psql.open_connection("dbname="+dbname);
  psql.open_cursor(table_name, { 
        "eid", "ny",
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

  int eid, ny;
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

  // serialize
  psql.next();

  pu::string2type(psql.get("eid"), eid);
  pu::string2type(psql.get("ny"), ny);
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

  std::cout << header() << std::endl;

  std::string line;
  for (size_t cidx = 0; cidx < ny; ++cidx) {

    line.clear();

    line += pu::type2string(eid) + ","; // eid
    line += pu::type2string(cidx) + ","; // cidx
    line += pu::type2string(ybpairmmiss2[cidx]) + ","; // mmiss2
    line += pu::type2string(ybpairmmissprime2[cidx]) + ","; // mmiss2prime
    line += pu::type2string(ybpaireextra50[cidx]) + ","; // eextra
    line += pu::type2string(ybpaircosthetat[cidx]) + ","; // costhetat
    line += pu::type2string(ytagblp3magcm[cidx]) + ","; // tag_lp3
    line += pu::type2string(ytagbcosby[cidx]) + ","; // tag_cosby
    line += pu::type2string(ytagbcosthetadlcm[cidx]) + ","; // tag_costhetadl
    line += pu::type2string(d_isdstar[b_d_idx[y_tagb_idx[cidx]]]) + ",";  // tag_isbdstar
    line += pu::type2string(d_dmass[b_d_idx[y_tagb_idx[cidx]]]) + ",";  // tag_dmass
    line += pu::type2string(ytagbdstardeltam[cidx]) + ","; // tag_deltam
    line += pu::type2string(ytagbcosthetadsoftcm[cidx]) + ","; // tag_costhetadsoft
    line += pu::type2string(ytagbsoftp3magcm[cidx]) + ","; // tag_softp3magcm
    line += pu::type2string(ysigbhp3magcm[cidx]) + ","; // sig_hp3
    line += pu::type2string(ysigbcosby[cidx]) + ","; // sig_cosby
    line += pu::type2string(ysigbcosthetadtaucm[cidx]) + ","; // sig_costhetadtau
    line += pu::type2string(ysigbvtxprobb[cidx]) + ","; // sig_vtxb
    line += pu::type2string(d_isdstar[b_d_idx[y_sigb_idx[cidx]]]) + ",";  // sig_isbdstar
    line += pu::type2string(d_dmass[b_d_idx[y_sigb_idx[cidx]]]) + ",";  // sig_dmass
    line += pu::type2string(ysigbdstardeltam[cidx]) + ","; // sig_deltam
    line += pu::type2string(ysigbcosthetadsoftcm[cidx]) + ","; // sig_costhetadsoft
    line += pu::type2string(ysigbsoftp3magcm[cidx]) + ","; // sig_softp3magcm
    line += pu::type2string(ysigbhmass[cidx]) + ","; // sig_hmass
    line += pu::type2string(ysigbvtxprobh[cidx]) + ","; // sig_vtxh
    line += pu::type2string(d_dmode[b_d_idx[y_tagb_idx[cidx]]]) + ",";  // tag_dmode
    line += pu::type2string(d_dstarmode[b_d_idx[y_tagb_idx[cidx]]]) + ",";  // tag_dstarmode
    line += pu::type2string(d_dmode[b_d_idx[y_sigb_idx[cidx]]]) + ",";  // sig_dmode
    line += pu::type2string(d_dstarmode[b_d_idx[y_sigb_idx[cidx]]]) + ",";  // sig_dstarmode
    line += pu::type2string(l_epid[b_lh_idx[y_tagb_idx[cidx]]]) + ",";  // tag_l_epid
    line += pu::type2string(l_mupid[b_lh_idx[y_tagb_idx[cidx]]]) + ",";  // tag_l_mupid
    line += pu::type2string(h_epid[b_lh_idx[y_sigb_idx[cidx]]]) + ",";  // sig_h_epid
    line += pu::type2string(h_mupid[b_lh_idx[y_sigb_idx[cidx]]]) + ",";  // sig_h_mupid

    line.pop_back();

    std::cout << line << std::endl;
  }

  std::cout << std::endl;

  // clean up
  psql.close_cursor();
  psql.close_connection();

  return 0;
}
