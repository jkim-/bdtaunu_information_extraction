#ifndef __RECOFEATUREEXTRACTOR_H__
#define __RECOFEATUREEXTRACTOR_H__

#include <boost/graph/adjacency_list.hpp>

class RecoFeatureExtractor {

  public:
    struct InternalVertexProperties {
      int idx_;
      int lund_id_;
      int local_idx_;
    };

    using Graph = boost::adjacency_list<
      boost::vecS, boost::vecS,
      boost::bidirectionalS, InternalVertexProperties>;

    using Vertex = typename boost::graph_traits<Graph>::vertex_descriptor;
    using VertexIter = typename boost::graph_traits<Graph>::vertex_iterator;

    using IntPropertyMap = boost::property_map<
      Graph, int InternalVertexProperties::*>::type;

  public:
    RecoFeatureExtractor();
    ~RecoFeatureExtractor();

    void set_data(int n_vertices, int n_edges, 
                  const std::vector<int> &from_vertices, 
                  const std::vector<int> &to_vertices, 
                  const std::vector<int> &lund_id, 
                  const std::vector<std::vector<int>> &global_indices);

    Graph get_graph() const;
    IntPropertyMap get_idx_pm();
    IntPropertyMap get_lund_id_pm();
    IntPropertyMap get_local_idx_pm();

  private:
    void clear_cache();

    void construct_graph(Graph &g, 
        int n_vertices, int n_edges,
        const std::vector<int> &from_vertices, 
        const std::vector<int> &to_vertices);

    void populate_lund_id(Graph &g,
        const std::vector<int> &lund_id);

    void populate_local_idx(Graph &g,
        const std::vector<std::vector<int>> &global_indices);

  private:
    Graph g_;
};

inline RecoFeatureExtractor::Graph 
RecoFeatureExtractor::get_graph() const { return g_; }

inline RecoFeatureExtractor::IntPropertyMap 
RecoFeatureExtractor::get_idx_pm() {
  return get(&InternalVertexProperties::idx_, g_);
}

inline RecoFeatureExtractor::IntPropertyMap 
RecoFeatureExtractor::get_lund_id_pm() {
  return get(&InternalVertexProperties::lund_id_, g_);
}

inline RecoFeatureExtractor::IntPropertyMap 
RecoFeatureExtractor::get_local_idx_pm() {
  return get(&InternalVertexProperties::local_idx_, g_);
}

#endif
