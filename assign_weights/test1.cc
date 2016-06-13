#include <iostream>
#include <fstream>
#include <vector>

#include <pgstring_convert.h>
#include <PsqlReader.h>

#include "MCWeightAssigner.h"

int main() {

  MCWeightAssigner mcw("dat/mc_weights.dat");

  // open database connection and populate fields
  PsqlReader psql;
  psql.open_connection("dbname=testing");
  psql.open_cursor(
      "framework_ntuples", 
      { "eid", "run", "mode_label" });

  int eid;
  int run, mode_label;

  psql.next();

  pgstring_convert(psql.get("eid"), eid);
  pgstring_convert(psql.get("run"), run);
  pgstring_convert(psql.get("mode_label"), mode_label);

  std::cout << run << " " << mode_label << " " << mcw.get_weight(mode_label, run) << std::endl;

  // close database connection
  psql.close_cursor();
  psql.close_connection();

  return 0;
}

