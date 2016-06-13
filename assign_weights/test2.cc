#include <iostream>
#include <fstream>
#include <vector>

#include <PsqlReader.h>
#include <CsvWriter.h>
#include <pgstring_utils.h>

#include <boost/program_options.hpp>

#include "MCWeightAssigner.h"

namespace po = boost::program_options;
namespace pu = pgstring_utils;

int main() {

  // open output csv file
  CsvWriter csv;
  csv.open("mcevent_weights_sigmc.csv", {
      "eid", "weight"
  });

  // open database connection and populate fields
  PsqlReader psql;
  psql.open_connection("dbname=testing");
  psql.open_cursor(
      "framework_ntuples_sigmc", 
      { "eid", "run", "mode_label" });

  // object that assigns event weights
  MCWeightAssigner mcw("dat/mc_weights.dat");

  // main loop to extract information 
  int eid, run, mode_label;
  while (psql.next()) {

    pu::string2type(psql.get("eid"), eid);
    pu::string2type(psql.get("run"), run);
    pu::string2type(psql.get("mode_label"), mode_label);

    csv.set("eid", pu::type2string(eid));
    csv.set("weight", pu::type2string(mcw.get_weight(mode_label, run)));
    csv.commit();

  }

  // close database connection
  psql.close_cursor();
  psql.close_connection();

  // close output file
  csv.close();

  return 0;
}

