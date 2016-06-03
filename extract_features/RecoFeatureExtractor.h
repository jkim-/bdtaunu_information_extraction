#ifndef __RECOFEATUREEXTRACTOR_H__
#define __RECOFEATUREEXTRACTOR_H__

#include <boost/graph/adjacency_list.hpp>

#include "RecoDmodeCatalogue.h"

class RecoFeatureExtractor {

  public:
    struct VertexProperties {
      int idx_;
      int lund_id_;
      int local_idx_;
    };

    using Graph = boost::adjacency_list<
      boost::vecS, boost::vecS,
      boost::bidirectionalS, VertexProperties>;

    using Vertex = typename boost::graph_traits<Graph>::vertex_descriptor;
    using VertexIter = typename boost::graph_traits<Graph>::vertex_iterator;
    using OutEdgeIter = typename boost::graph_traits<Graph>::out_edge_iterator;

    using IntPropertyMap = boost::property_map<
      Graph, int VertexProperties::*>::type;

  public:
    RecoFeatureExtractor();
    ~RecoFeatureExtractor();

    void set_data(int n_vertices, int n_edges, 
                  const std::vector<int> &from_vertices, 
                  const std::vector<int> &to_vertices, 
                  const std::vector<int> &lund_id, 

                  const std::vector<int> &y_reco_idx,
                  const std::vector<int> &b_reco_idx,
                  const std::vector<int> &d_reco_idx,
                  const std::vector<int> &c_reco_idx,
                  const std::vector<int> &h_reco_idx,
                  const std::vector<int> &l_reco_idx,
                  const std::vector<int> &gamma_reco_idx,

                  const std::vector<int> &ltrkidx, 
                  const std::vector<int> &htrkidx, 
                  const std::vector<int> &eselectorsmap, 
                  const std::vector<int> &muselectorsmap, 

                  const std::vector<float> &dmass 
                  );

    const std::vector<int>& get_l_epid() const { return l_epid_; }
    const std::vector<int>& get_l_mupid() const { return l_mupid_; }
    const std::vector<int>& get_h_epid() const { return h_epid_; }
    const std::vector<int>& get_h_mupid() const { return h_mupid_; }
    const std::vector<float>& get_dmeson_mass() const { return dmeson_mass_; }
    const std::vector<int>& get_dmeson_mode() const { return dmeson_mode_; }
    const std::vector<int>& get_dstar_mode() const { return dstar_mode_; }

    Graph get_graph() const;
    IntPropertyMap get_idx_pm();
    IntPropertyMap get_lund_id_pm();
    IntPropertyMap get_local_idx_pm();

  private:
    void clear_cache();

    void construct_graph(Graph &g, std::vector<Vertex> &idx2vtx,
        int n_vertices, int n_edges,
        const std::vector<int> &from_vertices, 
        const std::vector<int> &to_vertices);

    void populate_lund_id(Graph &g,
        const std::vector<int> &lund_id);

    void populate_local_idx(Graph &g,
        const std::vector<std::vector<int>> &global_indices);

    void cache_is_dstar(
        const std::vector<int> &d_reco_idx, 
        std::vector<int> &is_dstar);

    void assemble_decay_string(int reco_idx, 
        std::vector<int> &lund_list);

    void extract_pid(
        const std::vector<int> &ltrkidx, const std::vector<int> &htrkidx, 
        const std::vector<int> &eselectorsmap, 
        const std::vector<int> &muselectorsmap);

    void extract_dmeson_mass(
        const std::vector<int> &d_reco_idx,
        const std::vector<float> &dmass);

    void extract_d_modes(const std::vector<int> &d_reco_idx);

  private:

    RecoDmodeCatalogue d_catalogue_;

    Graph g_;
    std::vector<Vertex> idx2vtx_;

    std::vector<int> l_epid_, l_mupid_;
    std::vector<int> h_epid_, h_mupid_;
    std::vector<float> dmeson_mass_;
    std::vector<int> dmeson_mode_;
    std::vector<int> dstar_mode_;

};

inline RecoFeatureExtractor::Graph 
RecoFeatureExtractor::get_graph() const { return g_; }

inline RecoFeatureExtractor::IntPropertyMap 
RecoFeatureExtractor::get_idx_pm() {
  return get(&VertexProperties::idx_, g_);
}

inline RecoFeatureExtractor::IntPropertyMap 
RecoFeatureExtractor::get_lund_id_pm() {
  return get(&VertexProperties::lund_id_, g_);
}

inline RecoFeatureExtractor::IntPropertyMap 
RecoFeatureExtractor::get_local_idx_pm() {
  return get(&VertexProperties::local_idx_, g_);
}

#endif
