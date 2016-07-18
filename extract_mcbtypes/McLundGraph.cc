#include "McLundGraph.h"

McLundGraphFactory::Graph 
McLundGraphFactory::create_graph(int n_vertices, int n_edges,
  const std::vector<int> &from_vertices,
  const std::vector<int> &to_vertices,
  const std::vector<int> &lund_id) {

  // initialize empty graph. will build on this incrementally
  Graph g;

  // build the graph structure 
  construct_graph(g, n_vertices, n_edges, from_vertices, to_vertices);

  // attach internal properties
  populate_attributes(g, lund_id);

  return g;

}

void McLundGraphFactory::construct_graph(
    Graph &g,
    int n_vertices, int n_edges,
    const std::vector<int> &from_vertices,
    const std::vector<int> &to_vertices) {

  // check for argument consistency
  assert(from_vertices.size() == static_cast<unsigned>(n_edges));
  assert(from_vertices.size() == to_vertices.size());

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

void McLundGraphFactory::populate_attributes(Graph &g, 
    const std::vector<int> &lund_id) {

  assert(lund_id.size() == num_vertices(g));

  // populate attributes
  VertexIter vi, vi_end;
  for (std::tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi) {
    g[*vi].lund_id_ = lund_id[g[*vi].idx_];
  }
}
