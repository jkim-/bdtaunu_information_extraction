#ifndef __MCBTYPEANALYZER_H__
#define __MCBTYPEANALYZER_H__

#include <vector>
#include <string>

#include "McLundGraph.h"
#include "McDecayModeCurator.h"
#include "McBTypeDict.h"

class McBTypeAnalyzer {

  public:
    using Graph = McLundGraph;
    using Vertex = McLundGraphTraits::vertex_descriptor;
    using ConstIntPropertyMap = ConstMcLundGraphIntPM;

  public:

    McBTypeAnalyzer();
    ~McBTypeAnalyzer();

    void analyze(const Graph &g, const McDecayModeSummary &summary);

    McBTypeCode b1_mctype() const { return b1_mctype_; }
    McBTypeCode b2_mctype() const { return b2_mctype_; }

  private:
    void clear_cache();

    void bdaulunds_from_vertices(
        std::vector<int>&, const std::vector<Vertex>&);

  private:
    McBTypeDict mcbtype_dict_;

    McBTypeCode b1_mctype_;
    McBTypeCode b2_mctype_;

    ConstIntPropertyMap lund_pm_;

};

#endif
