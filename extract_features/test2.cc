#include <iostream>
#include <vector>
#include <fstream>

#include <PsqlReader.h>
#include <pgstring_convert.h>

#include "RecoFeatureExtractor.h"
#include "ParticleGraphWriter.h"

int main() {

  // open database connection and populate fields
  std::string dbname = "testing";
  std::string table_name = "feature_extraction_input_sigmc";
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

  psql.next();

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

  // extract features
  RecoFeatureExtractor extractor;
  extractor.set_data(reco_n_vertices, reco_n_edges, 
                     reco_from_vertices, reco_to_vertices, reco_lund_id,
                     { y_reco_idx, b_reco_idx, d_reco_idx, c_reco_idx,
                       h_reco_idx, l_reco_idx, gamma_reco_idx });

  auto graph_writer = make_lund_id_writer(extractor.get_lund_id_pm(), "../dat/pdt.dat");
  graph_writer.set_property("color", "red");
  print_graph(std::cout, extractor.get_graph(), extractor.get_idx_pm(), graph_writer);

  /*auto graph_writer = make_basic_graph_writer(extractor.get_local_idx_pm());
  graph_writer.set_property("color", "red");
  print_graph(std::cout, extractor.get_graph(), extractor.get_idx_pm(), graph_writer);*/

  // close database connection
  psql.close_cursor();
  psql.close_connection();

  return 0;

}
