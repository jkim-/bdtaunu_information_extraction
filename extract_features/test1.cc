#include <iostream>
#include <vector>
#include <fstream>

#include <boost/program_options.hpp>
#include <boost/graph/adjacency_list.hpp>

#include <PsqlReader.h>
#include <pgstring_convert.h>

#include "ParticleGraph.h"

struct VertexProperties {
  int idx_;
  int lund_id_;
  int local_idx_;
};

using Graph = boost::adjacency_list<
  boost::vecS, boost::vecS, 
  boost::bidirectionalS, VertexProperties>;

using IntPropertyMap = boost::property_map<
  Graph, int VertexProperties::*>::type;

int main() {

  // open database connection and populate fields
  std::string dbname = "testing";
  std::string table_name = "feature_extraction_input";
  int cursor_fetch_size = 5000;

  PsqlReader psql;
  psql.open_connection("dbname="+dbname);
  psql.open_cursor(table_name, 
      { "eid", 
        "reco_n_vertices", "reco_n_edges", 
        "reco_from_vertices", "reco_to_vertices", "reco_lund_id", 
        "y_reco_idx", "b_reco_idx", 
        "d_reco_idx", "c_reco_idx", 
        "h_reco_idx", "l_reco_idx", 
        "gamma_reco_idx" }, cursor_fetch_size);

  int eid;
  int reco_n_vertices, reco_n_edges;
  std::vector<int> reco_from_vertices, reco_to_vertices, reco_lund_id;
  std::vector<int> y_reco_idx, b_reco_idx, d_reco_idx, c_reco_idx;
  std::vector<int> h_reco_idx, l_reco_idx, gamma_reco_idx;

  // open output file and write title line
  std::string output_fname = "additional_features.csv";
  std::ofstream fout; fout.open(output_fname);
  fout << "eid,tag_l_epid,sig_h_epid";

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
    pgstring_convert(psql.get("b_reco_idx"), y_reco_idx);
    pgstring_convert(psql.get("d_reco_idx"), y_reco_idx);
    pgstring_convert(psql.get("c_reco_idx"), y_reco_idx);
    pgstring_convert(psql.get("h_reco_idx"), h_reco_idx);
    pgstring_convert(psql.get("l_reco_idx"), l_reco_idx);
    pgstring_convert(psql.get("gamma_reco_idx"), gamma_reco_idx);

    // build graph
    Graph g;
    construct_graph(g, reco_n_vertices, reco_n_edges, 
                    reco_from_vertices, reco_to_vertices);
    populate_lund_id(g, reco_lund_id);
    populate_local_idx(g, { y_reco_idx, b_reco_idx, d_reco_idx, c_reco_idx,
                            h_reco_idx, l_reco_idx, gamma_reco_idx });

  }

  // close file
  fout.close();

  // close database connection
  psql.close_cursor();
  psql.close_connection();

  std::cout << "processed " << n_records << " rows. " << std::endl;

  return 0;

}
