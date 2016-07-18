#ifndef __EVENTTYPELABELER_H__
#define __EVENTTYPELABELER_H__

#include <vector>
#include <string>

#include "McLundGraph.h"
#include "McDecayModeCurator.h"

#include "McBTypeDict.h"

enum class EventTypeDssCode {
  Dtau = 1,
  Dstartau = 2,
  DssSL = 3,
  SLHad = 4,
  Cont = 5,
  null
};

EventTypeDssCode decide_evttypedss(McBTypeCode, McBTypeCode);

class EventTypeLabeler {

  public:
    using Graph = McLundGraph;
    using Vertex = McLundGraphTraits::vertex_descriptor;
    using ConstIntPropertyMap = ConstMcLundGraphIntPM;

  public:

    EventTypeLabeler();
    ~EventTypeLabeler();

    void analyze(const Graph &g, const McDecayModeSummary &summary);

    EventTypeDssCode evttypedss_code() const { return evttypedss_code_; }
    McBTypeCode b1_mctype() const { return b1_mctype_; }
    McBTypeCode b2_mctype() const { return b2_mctype_; }

  private:
    void clear_cache();

    void bdaulunds_from_vertices(
        std::vector<int>&, const std::vector<Vertex>&);

  private:
    McBTypeDict mcbtype_dict_;

    EventTypeDssCode evttypedss_code_;
    McBTypeCode b1_mctype_;
    McBTypeCode b2_mctype_;

    ConstIntPropertyMap lund_pm_;

};

#endif
