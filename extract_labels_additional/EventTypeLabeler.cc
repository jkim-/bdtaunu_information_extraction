#include <ff_reweight_constants.h>

#include "EventTypeLabeler.h"

EventTypeLabeler::EventTypeLabeler() :mcbtype_dict_() {
  clear_cache();
}

EventTypeLabeler::~EventTypeLabeler() {}

void EventTypeLabeler::clear_cache() { 
  evttypedss_code_ = EventTypeDssCode::null;
  b1_mctype_ = McBTypeCode::null;
  b2_mctype_ = McBTypeCode::null;
  return;
}

void EventTypeLabeler::bdaulunds_from_vertices(
    std::vector<int> &bdaulunds, 
    const std::vector<Vertex> &daughter_vertices) {

  bdaulunds.clear();
  for (const auto & v : daughter_vertices) {
    bdaulunds.push_back(lund_pm_[v]);
  }
}

void EventTypeLabeler::analyze(
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

  // decide event types
  evttypedss_code_ = decide_evttypedss(b1_mctype_, b2_mctype_);

  return; 
}

EventTypeDssCode decide_evttypedss(McBTypeCode b1mctype, McBTypeCode b2mctype) {

  if (b1mctype == McBTypeCode::NoB || b2mctype == McBTypeCode::NoB) {
    return EventTypeDssCode::Cont;
  }

  if (b1mctype == McBTypeCode::Dtau || b2mctype == McBTypeCode::Dtau) {
    return EventTypeDssCode::Dtau;
  }

  if (b1mctype == McBTypeCode::Dstartau || b2mctype == McBTypeCode::Dstartau) {
    return EventTypeDssCode::Dstartau;
  }

  if (b1mctype == McBTypeCode::Dstarstar_res || b2mctype == McBTypeCode::Dstarstar_res ||
      b1mctype == McBTypeCode::Dstarstar_nonres || b2mctype == McBTypeCode::Dstarstar_nonres) {
    return EventTypeDssCode::DssSL;
  }

  return EventTypeDssCode::SLHad;

}
