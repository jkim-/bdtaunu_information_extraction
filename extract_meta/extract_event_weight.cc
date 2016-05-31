#include <iostream>
#include <fstream>
#include <vector>

#include <pgstring_convert.h>
#include <PsqlReader.h>

#include "MCWeightAssigner.h"

int main() {

  // open output csv file
  std::ofstream fout;
  fout.open("event_meta.csv");
  fout << "eid,event_weight" << std::endl;

  // open database connection and populate fields
  PsqlReader psql;
  psql.open_connection("dbname=testing");
  psql.open_cursor(
      "framework_ntuples_sp1235", 
      { "eid", "run", "mode_label" });

  // object that assigns event weights
  MCWeightAssigner mcw("dat/mc_weights.dat");

  // main loop to extract information 
  int eid, run, mode_label;
  while (psql.next()) {

    pgstring_convert(psql.get("eid"), eid);
    pgstring_convert(psql.get("run"), run);
    pgstring_convert(psql.get("mode_label"), mode_label);

    fout << eid << ",";
    fout << mcw.get_weight(mode_label, run);
    fout << std::endl;

  }

  // close database connection
  psql.close_cursor();
  psql.close_connection();

  // close output file
  fout.close();

  return 0;
}

