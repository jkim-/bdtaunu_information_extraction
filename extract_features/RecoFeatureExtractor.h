#ifndef __RECOFEATUREEXTRACTOR_H__
#define __RECOFEATUREEXTRACTOR_H__

#include <boost/graph/adjacency_list.hpp>

#include "RecoDmodeCatalogue.h"

// RecoFeatureExtractor computes additional features 
// to attach to reconstructed particles. 
//
// usage:
//
//     // create an extractor object. ready to accept data. 
//     RecoFeatureExtractor extractor;
//
//     // apply inputs to the extractor.
//     // automatically computes all the features of interest. 
//     extractor.set_data(...);
//
//     // get the features of interest
//     extractor.get_b_is_tagb(); 
//     extractor.get_l_epid(); 
//     ... 
//
class RecoFeatureExtractor {

  public:

    // internal properties attached at each vertex
    // idx_: global index in the reconstruction graph
    // lund_id_: lund id of the particle
    // local_idx_: local index in its candidate block (as in btatuplemaker). 
    struct VertexProperties {
      int idx_;        
      int lund_id_;   
      int local_idx_;
    };

    // reconstruction graph typedefs
    using Graph = boost::adjacency_list<
      boost::vecS, boost::vecS,
      boost::bidirectionalS, VertexProperties>;
    using Vertex = typename boost::graph_traits<Graph>::vertex_descriptor;
    using VertexIter = typename boost::graph_traits<Graph>::vertex_iterator;
    using OutEdgeIter = typename boost::graph_traits<Graph>::out_edge_iterator;
    using IntPropertyMap = boost::property_map<
      Graph, int VertexProperties::*>::type;

  public:

    // constructs and initializes an object into a state ready to inputs
    RecoFeatureExtractor();
    ~RecoFeatureExtractor();

    // main method that accepts inputs and computes the features. 
    // calling this method also runs all feature computations. 
    void set_data(

                  // parameters required to build the reconstruction graph.
                  int n_vertices, int n_edges, 
                  const std::vector<int> &from_vertices, 
                  const std::vector<int> &to_vertices, 
                  const std::vector<int> &lund_id, 

                  // parameters required to assign specific reconstructed 
                  // candidates to graph vertices. 
                  const std::vector<int> &y_reco_idx,
                  const std::vector<int> &b_reco_idx,
                  const std::vector<int> &d_reco_idx,
                  const std::vector<int> &c_reco_idx,
                  const std::vector<int> &h_reco_idx,
                  const std::vector<int> &l_reco_idx,
                  const std::vector<int> &gamma_reco_idx,

                  // parameters required to extract features based on 
                  // track properties. 
                  const std::vector<int> &ltrkidx, 
                  const std::vector<int> &htrkidx, 
                  const std::vector<int> &eselectorsmap, 
                  const std::vector<int> &muselectorsmap, 

                  // parameters required to extract D candidate features
                  const std::vector<float> &dmass 
    );

    // access extracted features based on the inputs 
    // of the previous `set_data` call
    const std::vector<int>& get_l_epid() const { return l_epid_; }
    const std::vector<int>& get_l_mupid() const { return l_mupid_; }
    const std::vector<int>& get_h_epid() const { return h_epid_; }
    const std::vector<int>& get_h_mupid() const { return h_mupid_; }
    const std::vector<float>& get_d_dmass() const { return d_dmass_; }
    const std::vector<int>& get_d_dmode() const { return d_dmode_; }
    const std::vector<int>& get_d_dstarmode() const { return d_dstarmode_; }
    const std::vector<int>& get_b_is_tag() const { return b_is_tag_; }
    const std::vector<int>& get_b_d_idx() const { return b_d_idx_; }
    const std::vector<int>& get_b_lh_idx() const { return b_lh_idx_; }
    const std::vector<int>& get_y_tagb_idx() const { return y_tagb_idx_; }
    const std::vector<int>& get_y_sigb_idx() const { return y_sigb_idx_; }

    // access reconstruction graph and its properties
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

    void extract_d_dmass(
        const std::vector<int> &d_reco_idx,
        const std::vector<float> &dmass);

    void extract_d_modes(const std::vector<int> &d_reco_idx);

    void extract_b_is_tag(const std::vector<int> &b_reco_idx);

    void extract_b_daughter_local_indices(
        const std::vector<int> &b_reco_idx);

    void extract_y_b_idx(const std::vector<int> &y_reco_idx);

  private:

    RecoDmodeCatalogue d_catalogue_;

    Graph g_;
    std::vector<Vertex> idx2vtx_;

    std::vector<int> l_epid_, l_mupid_;
    std::vector<int> h_epid_, h_mupid_;
    std::vector<float> d_dmass_;
    std::vector<int> d_dmode_;
    std::vector<int> d_dstarmode_;
    std::vector<int> b_is_tag_;
    std::vector<int> b_d_idx_;
    std::vector<int> b_lh_idx_;
    std::vector<int> y_tagb_idx_;
    std::vector<int> y_sigb_idx_;

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
