#include "McBTypeAnalyzer.h"
#include "extract_mcbtypes_constants.h"

McBTypeAnalyzer::McBTypeAnalyzer() :mcbtype_dict_() {
  clear_cache();
}

McBTypeAnalyzer::~McBTypeAnalyzer() {}

void McBTypeAnalyzer::clear_cache() { 
  b1_mctype_ = McBTypeCode::null;
  b2_mctype_ = McBTypeCode::null;
  return;
}

void McBTypeAnalyzer::daulunds_from_vertices(
    std::vector<int> &daulunds, 
    const std::vector<Vertex> &daughter_vertices) {

  daulunds.clear();
  for (const auto & v : daughter_vertices) {
    daulunds.push_back(lund_pm_[v]);
  }
}

McTauTypeCode McBTypeAnalyzer::get_tau_type(
    const McDecayModeSummary &summary) {

  // Assume tau_h unless e or mu is found
  McTauTypeCode tau_mctype = McTauTypeCode::tau_h;
  auto tau_it = summary.tau_modes().begin();
  std::vector<int> taudaulunds;
  daulunds_from_vertices(taudaulunds, tau_it->second);
  for (const auto &l : taudaulunds) {
    if (std::abs(l) == eLund) {
    tau_mctype = McTauTypeCode::tau_e;

    } else if (std::abs(l) == muLund) {
    tau_mctype = McTauTypeCode::tau_mu;
    }
  }
  return tau_mctype;
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
    b1tau_mctype_ = McTauTypeCode::NoTau;
    b2tau_mctype_ = McTauTypeCode::NoTau;

  // analyze b and tau modes
  } else {
    // b1
    auto b_it = summary.b_modes().begin();
    std::vector<int> bdaulunds;
    daulunds_from_vertices(bdaulunds, b_it->second);
    b1_mctype_ = mcbtype_dict_.find(bdaulunds);
    // look for b1tau
    if (std::find_if(bdaulunds.begin(), bdaulunds.end(), 
                     [] (const int l) { return (std::abs(l) == tauLund);}) 
         != bdaulunds.end()) {
      b1tau_mctype_ = get_tau_type(summary);
    } else {
      b1tau_mctype_ = McTauTypeCode::NoTau;
    }

    // b2
    ++b_it; bdaulunds.clear();
    daulunds_from_vertices(bdaulunds, b_it->second);
    b2_mctype_ = mcbtype_dict_.find(bdaulunds);
    // look for b2tau
    if (std::find_if(bdaulunds.begin(), bdaulunds.end(), 
                     [] (const int l) { return (std::abs(l) == tauLund);}) 
         != bdaulunds.end()) {
      b2tau_mctype_ = get_tau_type(summary);
    } else {
      b2tau_mctype_ = McTauTypeCode::NoTau;
    }

  }

  return; 
}
