#include "RecoFeatureExtractor.h"

RecoFeatureExtractor::RecoFeatureExtractor() {}

RecoFeatureExtractor::~RecoFeatureExtractor() {}

void RecoFeatureExtractor::set_data(
    int n_vertices, int n_edges, 
    const std::vector<int> &from_vertices, 
    const std::vector<int> &to_vertices, 
    const std::vector<int> &lund_id, 
    const std::vector<std::vector<int>> &global_indices) {

  clear_cache();

  construct_graph(g_, n_vertices, n_edges, from_vertices, to_vertices);
  populate_lund_id(g_, lund_id);
  populate_local_idx(g_, global_indices);

}


void RecoFeatureExtractor::clear_cache() {
  g_.clear();
}

void RecoFeatureExtractor::construct_graph(Graph &g, 
    int n_vertices, int n_edges,
    const std::vector<int> &from_vertices, 
    const std::vector<int> &to_vertices) {

  // check for argument consistency
  if (from_vertices.size() != static_cast<unsigned>(n_edges)) {
    throw std::invalid_argument(
        "RecoFeatureExtractor::construct_graph(): n_vertices and " 
        "from_vertices.size() must agree. "
    );
  }

  if (to_vertices.size() != from_vertices.size()) {
    throw std::invalid_argument(
        "RecoFeatureExtractor::construct_graph(): from_vertices.size() "
        "must agree with to_vertices.size(). "
    );
  }

  // clear the graph 
  g.clear();

  // establish a mapping between vertex index and vertex descriptors
  std::vector<Vertex> vertex_map(n_vertices);

  // insert vertices and bind internal properties
  for (int i = 0; i < n_vertices; ++i) {
    Vertex u = boost::add_vertex(g);
    vertex_map[i] = u;
    g[u].idx_ = i;
  }

  // insert edges
  for (int i = 0; i < n_edges; ++i) {
    boost::add_edge(vertex_map[from_vertices[i]], 
                    vertex_map[to_vertices[i]], g);
  }
}

void RecoFeatureExtractor::populate_lund_id(Graph &g,
    const std::vector<int> &lund_id) {

  // check for argument consistency
  if (lund_id.size() != num_vertices(g)) {
    throw std::invalid_argument(
        "RecoFeatureExtractor::populate_lund_id(): "
        "lund_id.size() must agree with num_vertices(g). "
    );
  }

  // populate attributes
  VertexIter vi, vi_end;
  for (std::tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi) {
    g[*vi].lund_id_ = lund_id[g[*vi].idx_];
  }

}

void RecoFeatureExtractor::populate_local_idx(Graph &g,
    const std::vector<std::vector<int>> &global_indices) {

  std::vector<Vertex> vertex_map(num_vertices(g));
  VertexIter vi, vi_end;
  for (std::tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi) {
    vertex_map[g[*vi].idx_] = *vi;
  }

  for (const auto &index_v : global_indices) {
    for (int i = 0; i < index_v.size(); ++i) {
      g[vertex_map[index_v[i]]].local_idx_ = i;
    }
  }

}
