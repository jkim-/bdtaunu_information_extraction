#include <iostream>
#include <fstream>
#include <vector>

#include <PsqlReader.h>
#include <CsvWriter.h>
#include <pgstring_utils.h>

#include <boost/program_options.hpp>

namespace po = boost::program_options;
namespace pu = pgstring_utils;

int main() {

  // open output csv file
  CsvWriter csv;
  csv.open("upsilon_candidate_mclabel_sigmc.csv", {
      "eid", "cidx", "is_matched"
  });

  // open database connection and populate fields
  PsqlReader psql;
  psql.open_connection("dbname=testing");
  psql.open_cursor(
      "truth_match_sigmc", 
      { "eid", "y_match_status" });

  // main loop to extract information 
  int eid; 
  std::vector<int> y_match_status;
  while (psql.next()) {

    pu::string2type(psql.get("eid"), eid);
    pu::string2type(psql.get("y_match_status"), y_match_status);

    for (size_t i = 0; i < y_match_status.size(); ++i) {
      csv.set("eid", pu::type2string(eid));
      csv.set("cidx", pu::type2string(i));
      csv.set("is_matched", pu::type2string(y_match_status[i]));
      csv.commit();
    }
  }

  // close database connection
  psql.close_cursor();
  psql.close_connection();

  // close output file
  csv.close();

  return 0;
}

