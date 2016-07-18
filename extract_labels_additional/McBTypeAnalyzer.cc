#include <ff_reweight_constants.h>

#include "McBTypeAnalyzer.h"

McBTypeAnalyzer::McBTypeAnalyzer() :mcbtype_dict_() {
  clear_cache();
}

McBTypeAnalyzer::~McBTypeAnalyzer() {}

void McBTypeAnalyzer::clear_cache() { 
  b1_mctype_ = McBTypeCode::null;
  b2_mctype_ = McBTypeCode::null;
  return;
}

void McBTypeAnalyzer::bdaulunds_from_vertices(
    std::vector<int> &bdaulunds, 
    const std::vector<Vertex> &daughter_vertices) {

  bdaulunds.clear();
  for (const auto & v : daughter_vertices) {
    bdaulunds.push_back(lund_pm_[v]);
  }
}

void McBTypeAnalyzer::analyze(
    const Graph &g, const McDecayModeSummary &summary) {

  // initialize cache
  clear_cache();
  lund_pm_ = get_lund_pm(g);

  // records without 2 B decays considered continuum
  if (summary.b_modes().size() != 2) { 
    b1_mctype_ = McBTypeCode::NoB;
    b2_mctype_ = McBTypeCode::NoB;

  // analyze b modes
  } else {
    auto it = summary.b_modes().begin();
    std::vector<int> bdaulunds;
    bdaulunds_from_vertices(bdaulunds, it->second);
    b1_mctype_ = mcbtype_dict_.find(bdaulunds);

    ++it; bdaulunds.clear();
    bdaulunds_from_vertices(bdaulunds, it->second);
    b2_mctype_ = mcbtype_dict_.find(bdaulunds);
  }

  return; 
}
