#ifndef __MCLUNDGRAPH_H__
#define __MCLUNDGRAPH_H__

#include <vector>
#include <unordered_map>

#include <boost/graph/adjacency_list.hpp>

// McLundGraph definition
// -----------------------

// bundled internal property attached to each vertex of an McLundGraph
struct McLundGraphVtxProp {
  int idx_;
  int lund_id_;
};

// McLundGraph definition
using McLundGraph = boost::adjacency_list<
      boost::listS, boost::listS,
      boost::bidirectionalS, McLundGraphVtxProp>;

// Associated typedefs
// -------------------

// graph property traits class
using McLundGraphTraits = typename boost::graph_traits<McLundGraph>;

// property map typedefs

using McLundGraphIntPM = 
  typename boost::property_map<McLundGraph, int McLundGraphVtxProp::*>::type;

using ConstMcLundGraphIntPM = 
  typename boost::property_map<
    McLundGraph, int McLundGraphVtxProp::*>::const_type;

// Associated non-member functions
// -------------------------------
    
inline McLundGraphIntPM get_idx_pm(McLundGraph &g) { 
  return get(&McLundGraphVtxProp::idx_, g); 
}
    
inline ConstMcLundGraphIntPM get_idx_pm(const McLundGraph &g) { 
  return get(&McLundGraphVtxProp::idx_, g); 
}

inline McLundGraphIntPM get_lund_pm(McLundGraph &g) { 
  return get(&McLundGraphVtxProp::lund_id_, g); 
}

inline ConstMcLundGraphIntPM get_lund_pm(const McLundGraph &g) { 
  return get(&McLundGraphVtxProp::lund_id_, g); 
}


// Graph creation class
// --------------------

class McLundGraphFactory {

  private:

    using Graph = McLundGraph;
    using Vertex = McLundGraphTraits::vertex_descriptor;
    using VertexIter = McLundGraphTraits::vertex_iterator;
    using InEdgeIter = McLundGraphTraits::in_edge_iterator;
    using OutEdgeIter = McLundGraphTraits::out_edge_iterator;

  public: 

    McLundGraphFactory() {};
    ~McLundGraphFactory() {};

    // construct and return a McLundGraph implied 
    // by the given the arguments  
    Graph create_graph(int n_vertices, int n_edges,
      const std::vector<int> &from_vertices,
      const std::vector<int> &to_vertices,
      const std::vector<int> &lund_id);


  private:
    void construct_graph(Graph &g,
        int n_vertices, int n_edges,
        const std::vector<int> &from_vertices,
        const std::vector<int> &to_vertices);

    void populate_attributes(Graph &g, 
        const std::vector<int> &lund_id);

};

#endif
