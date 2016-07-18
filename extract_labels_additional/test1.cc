#include <cmath>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#include <PsqlReader.h>
#include <pgstring_utils.h>

#include "McLundGraph.h"
#include "McDecayModeCurator.h"
#include "McBTypeAnalyzer.h"

namespace pu = pgstring_utils;

int main() {

  // open database connection and populate fields
  std::string dbname = "bdtaunuhad_lite";
  std::string table_name = "graph_sp1235";
  int cursor_fetch_size = 5000;

  PsqlReader psql;
  psql.open_connection("dbname="+dbname);
  psql.open_cursor(table_name, { 
        "eid", 
        "mc_n_vertices", "mc_n_edges",
        "mc_from_vertices", "mc_to_vertices", 
        "mc_lund_id"
      }, cursor_fetch_size);

  int eid, mc_n_vertices, mc_n_edges;
  std::vector<int> mc_from_vertices, mc_to_vertices, mc_lund_id;

  // initialize worker classes
  McLundGraphFactory graph_factory;
  McDecayModeCurator curator;
  McDecayModeSummary summary;
  McBTypeAnalyzer analyzer;

  std::ofstream fout("test.csv");
  fout << "b1_mctype,b2_mctype" << std::endl;

  // main loop
  const int max_records = 1000;
  int i = 0;
  while (psql.next() && i < max_records) {

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

    fout << static_cast<int>(analyzer.b1_mctype()) << ",";
    fout << static_cast<int>(analyzer.b2_mctype()) << std::endl;

    ++i;
  }
  
  // clean up
  psql.close_cursor();
  psql.close_connection();

  return 0;
}
