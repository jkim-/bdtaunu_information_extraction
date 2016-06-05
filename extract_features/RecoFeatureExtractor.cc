#include "RecoFeatureExtractor.h"

// lund id and particle species mappings
namespace {

  const int D0Lund = 421;        
  const int DcLund = 411;
  const int Dstar0Lund = 423;
  const int DstarcLund = 413;
  const int rhoLund = 213;
  const int piLund = 211;
  const int eLund = 11;
  const int muLund = 13;

  // decide if lund id is a D or D*
  bool is_d_or_dstar(int lund_id) {
    int abslund = abs(lund_id);
    if (abslund == Dstar0Lund || abslund == DstarcLund ||
        abslund == D0Lund || abslund == DcLund) {
      return true; 
    } else {
      return false; 
    }
  }

  // decide if lund id is a D*
  bool is_dstar(int lund_id) {
    int abslund = abs(lund_id);
    if (abslund == Dstar0Lund || abslund == DstarcLund) {
      return true; 
    } else {
      return false; 
    }
  }

  // decide if lund id is a D
  bool is_dmeson(int lund_id) {
    int abslund = abs(lund_id);
    if (abslund == D0Lund || abslund == DcLund) {
      return true; 
    } else {
      return false; 
    }
  }

  // decide if lund id is e or mu (tau is not included)
  bool is_lepton(int lund_id) {
    int abslund = abs(lund_id);
    if (abslund == eLund || abslund == muLund) {
      return true; 
    } else {
      return false; 
    }
  }

  // decide if lund id is a rho
  inline bool is_rho(int lund_id) {
    return (abs(lund_id) == rhoLund) ? true : false;
  }

  // decide if lund id is a charged pion
  inline bool is_pi(int lund_id) {
    return (abs(lund_id) == piLund) ? true : false;
  }

}


RecoFeatureExtractor::RecoFeatureExtractor() 
  : d_catalogue_() { clear_cache(); }

RecoFeatureExtractor::~RecoFeatureExtractor() {}


// clear data caches
void RecoFeatureExtractor::clear_cache() {
  g_.clear();
  idx2vtx_.clear();
  l_epid_.clear(); l_mupid_.clear();
  h_epid_.clear(); h_mupid_.clear();
  d_is_dstar_.clear();
  d_dmass_.clear();
  d_dmode_.clear();
  d_dstarmode_.clear();
  b_is_tag_.clear();
  b_d_idx_.clear();
  b_lh_idx_.clear();
  y_tagb_idx_.clear();
  y_sigb_idx_.clear();
}


// take special note that features that other features 
// depend on are computed first
void RecoFeatureExtractor::set_data(

    int n_vertices, int n_edges, 
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

) {
  

  // clear out data caches before populating them with new results
  clear_cache();

  // build the reconstruction graph. required to compute certain features
  build_reconstruction_graph(n_vertices, n_edges, 
      from_vertices, to_vertices, lund_id, 
      { y_reco_idx, b_reco_idx, d_reco_idx, c_reco_idx, 
        h_reco_idx, l_reco_idx, gamma_reco_idx } );

  // extract features for candidates in the l, h, gamma blocks
  extract_lhg_features(ltrkidx, htrkidx, eselectorsmap, muselectorsmap);

  // extract features for candidates in the d block
  extract_d_features(d_reco_idx, dmass);

  // extract features for candidates in the b block
  extract_b_features(b_reco_idx);

  // extract features for candidates in the y block
  extract_y_features(y_reco_idx);

}


// Extract features for the y block
// --------------------------------

void RecoFeatureExtractor::extract_y_features(
    const std::vector<int> &y_reco_idx) {

  // for each y candidate, save the tag and sig b's 
  // reco block indices
  extract_y_b_idx(y_reco_idx);
}

void RecoFeatureExtractor::extract_y_b_idx(
    const std::vector<int> &y_reco_idx) {

  // clear cache. -1 indicates absence. 
  // none should be -1 at the end of this routine. 
  y_tagb_idx_ = std::vector<int>(y_reco_idx.size(), -1);
  y_sigb_idx_ = std::vector<int>(y_reco_idx.size(), -1);

  // for each y candidate, loop through its daughters and save 
  // the relevante informatiion accordingly
  for (size_t i = 0; i < y_reco_idx.size(); ++i) {

    Vertex u = idx2vtx_[y_reco_idx[i]];

    // loop through daughters. all are b candidates. 
    OutEdgeIter oe, oe_end; 
    for (std::tie(oe, oe_end) = out_edges(u, g_); oe != oe_end; ++oe) {

      int b_idx = g_[target(*oe, g_)].local_idx_;
      if (b_is_tag_[b_idx]) {
        y_tagb_idx_[i] = b_idx;
      } else {
        y_sigb_idx_[i] = b_idx;
      }

    }
  }
}

// Extract features for the b block
// --------------------------------

void RecoFeatureExtractor::extract_b_features(
    const std::vector<int> &b_reco_idx) {

  // decide if a b candidate is tag or sig
  extract_b_is_tag(b_reco_idx);

  // find the following:
  // + b's d daughter's index in its reconstruction block.
  // + b's l or h daughter's index in its reconstruction block.
  //   the index corresponds to l if the b is a tag, h otherwise. 
  extract_b_daughter_local_indices(b_reco_idx);
}

void RecoFeatureExtractor::extract_b_is_tag(
    const std::vector<int> &b_reco_idx) {

  // clear cache. default is that it is sig. 
  b_is_tag_ = std::vector<int>(b_reco_idx.size(), 0);

  // loop over daughters and check for the presence of a lepton. 
  Vertex u; OutEdgeIter oe, oe_end; 
  for (size_t i = 0; i < b_reco_idx.size(); ++i) {

    bool found_lepton = false;

    u = idx2vtx_[b_reco_idx[i]];
    for (std::tie(oe, oe_end) = out_edges(u, g_); oe != oe_end; ++oe) {
      if (is_lepton(g_[target(*oe, g_)].lund_id_)) { found_lepton = true; }
    }

    if (found_lepton) { b_is_tag_[i] = 1; }
  }
}

void RecoFeatureExtractor::extract_b_daughter_local_indices(
    const std::vector<int> &b_reco_idx) {

  // clear cache. -1 indicates absence. 
  // none should be -1 at the end of this routine. 
  b_d_idx_ = std::vector<int>(b_reco_idx.size(), -1);
  b_lh_idx_ = std::vector<int>(b_reco_idx.size(), -1);

  // loop over each b candidate 
  for (size_t i = 0; i < b_reco_idx.size(); ++i) {

    // u: vertex for the b candidate
    Vertex u = idx2vtx_[b_reco_idx[i]];

    // loop over b candidate daughters and save the 
    // relevant information according to their identities
    OutEdgeIter oe, oe_end;
    for (std::tie(oe, oe_end) = out_edges(u, g_); oe != oe_end; ++oe) {

      // v: vertex for b candidate primary daughters
      Vertex v = target(*oe, g_);

      // encounter D daughter: save its index
      if (is_d_or_dstar(g_[v].lund_id_)) {
        b_d_idx_[i] = g_[v].local_idx_;

      // encounter a rho daughter: continue search in its daughters
      } else if (is_rho(g_[v].lund_id_)) {

        // loop over the rho's daughters and search for a charged pi
        OutEdgeIter oer, oer_end; 
        for (std::tie(oer, oer_end) = out_edges(v, g_); 
             oer != oer_end; ++oer) {
          Vertex w = target(*oer, g_);
          if (is_pi(g_[w].lund_id_)) {
            b_lh_idx_[i] = g_[w].local_idx_;
          }
        }

      // if not D or rho, must be a lepton or charged pi: save its index
      } else {
        b_lh_idx_[i] = g_[v].local_idx_;
      }
    }
  }
}


// Extract features for the d block
// --------------------------------

void RecoFeatureExtractor::extract_d_features(
    const std::vector<int> &d_reco_idx, 
    const std::vector<float> &dmass) {

  // decide if d candidate is D or a D* 
  extract_d_is_dstar(d_reco_idx);

  // extract d meson mass. for D* candidates, it's the D daughter's mass. 
  extract_d_dmass(d_reco_idx, dmass);

  // extract d candidate decay modes. 
  extract_d_modes(d_reco_idx);
}


void RecoFeatureExtractor::extract_d_is_dstar(
    const std::vector<int> &d_reco_idx) {

  // clear cache. default is not a D* 
  d_is_dstar_ = std::vector<int>(d_reco_idx.size(), 0);

  // decide based on the absolute lund id 
  for (size_t i = 0; i < d_reco_idx.size(); ++i) {
    if (is_dstar(g_[idx2vtx_[d_reco_idx[i]]].lund_id_)) {
      d_is_dstar_[i] = 1;
    }
  }
}


void RecoFeatureExtractor::extract_d_dmass(
    const std::vector<int> &d_reco_idx,
    const std::vector<float> &dmass) {

  if (d_reco_idx.size() != dmass.size()) {
    throw std::invalid_argument(
        "RecoFeatureExtractor::extract_d_dmass(): d_reco_idx.size() "
        "must agree with dmass.size(). "
    );
  }

  // first pass: fill D meson masses. 
  d_dmass_ = std::vector<float>(d_reco_idx.size(), -1);
  for (size_t i = 0; i < dmass.size(); ++i) {
    if (!d_is_dstar_[i]) { d_dmass_[i] = dmass[i]; }
  }

  // second pass: for each D* meson, fill its D daughter's  mass. 
  for (size_t i = 0; i < dmass.size(); ++i) {

    if (d_is_dstar_[i]) { 

      // vertex for the D*
      Vertex u = idx2vtx_[d_reco_idx[i]];

      // loop through its daughters
      OutEdgeIter oe, oe_end;
      for (std::tie(oe, oe_end) = out_edges(u, g_); oe != oe_end; ++oe) {

        Vertex v = target(*oe, g_);
        if (is_dmeson(g_[v].lund_id_)) {
          d_dmass_[i] = d_dmass_[g_[v].local_idx_];
        }
      }
    }
  }

  return;
}

void RecoFeatureExtractor::assemble_d_decay_string(
    int reco_idx, 
    std::vector<int> &lund_list) {

  lund_list.clear();

  // store mother lund
  Vertex u = idx2vtx_[reco_idx];
  lund_list.push_back(g_[u].lund_id_);

  // store daughter lund
  OutEdgeIter oe, oe_end;
  for (std::tie(oe, oe_end) = out_edges(u, g_); oe != oe_end; ++oe) {
    lund_list.push_back(g_[target(*oe, g_)].lund_id_);
  }
}

void RecoFeatureExtractor::extract_d_modes(
    const std::vector<int> &d_reco_idx) {

  // clear cache
  d_dmode_ = std::vector<int>(d_reco_idx.size(), -1);
  d_dstarmode_ = std::vector<int>(d_reco_idx.size(), -1);

  // first pass: deduce the decay mode for the D candidate. 
  std::vector<int> lund_list;
  for (size_t i = 0; i < d_reco_idx.size(); ++i) {

    // get the d decay lund id vector
    assemble_d_decay_string(d_reco_idx[i], lund_list);

    // lookup the mode depending on the D meson type. 
    if (!d_is_dstar_[i]) { 
      d_dmode_[i] = d_catalogue_.get_d_mode(lund_list);
    } else {
      d_dstarmode_[i] = d_catalogue_.get_dstar_mode(lund_list);
    }
  }


  // second pass: for D* candidates, its D meson mode will be the mode
  // of its D daughter. (the mode of the D* itself computed in first pass)
  for (size_t i = 0; i < d_reco_idx.size(); ++i) {

    if (d_is_dstar_[i]) {

      // vertex corresponding to this D*
      Vertex u = idx2vtx_[d_reco_idx[i]];

      // loop over its daughters
      OutEdgeIter oe, oe_end;
      for (std::tie(oe, oe_end) = out_edges(u, g_); oe != oe_end; ++oe) {

        Vertex v = target(*oe, g_);

        // if the daughter is a D meson, copy its dmode over
        if (is_dmeson(g_[v].lund_id_)) {
          d_dmode_[i] = d_dmode_[g_[v].local_idx_];
        }
      }
    }
  }

  return;
}


// Extract features for l, h, and gamma blocks
// -------------------------------------------

void RecoFeatureExtractor::extract_lhg_features(
    const std::vector<int> &ltrkidx, 
    const std::vector<int> &htrkidx, 
    const std::vector<int> &eselectorsmap, 
    const std::vector<int> &muselectorsmap) {

  // extract pid information for l and h block candidates 
  extract_pid(ltrkidx, htrkidx, eselectorsmap, muselectorsmap);

}

namespace {

  // Level 0: pass no electron KM selectors. 
  // Levels 1-6: the higher the more electron KM selectors passed. 
  int deduce_epid_levels(int epid_bitmap) {

    int epid_level = 0;

    // + 6 is the offset for the first eKM selector. 
    // + cycle through 6 levels:
    //   SuperLoose, VeryLoose, Loose, Tight, VeryTight, SuperTight. 
    for (unsigned i = 0; i <= 5; ++i) {
      epid_level += (epid_bitmap >> (6 + i)) & 0x1;
    }

    return epid_level;
  }

  // Level 0: pass no muon bdt fake rate selectors. 
  // Levels 1-4: the higher the more muon bdt fake rate selectors passed. 
  int deduce_mupid_levels(int mupid_bitmap) {

    int mupid_level = 0;

    // + 20 is the offset for the first muBDTFakeRate selector. 
    // + cycle through 4 levels:
    //   VeryLoose, Loose, Tight, VeryTight. 
    for (unsigned i = 0; i <= 3; ++i) {
      mupid_level += (mupid_bitmap >> (20 + i)) & 0x1;
    }

    return mupid_level;
  }

  // given pid cache, selectors maps, and the track map, assign pid levels.
  void populate_pid(
      std::vector<int> &epid, std::vector<int> &mupid, 
      const std::vector<int> &eselectorsmap, 
      const std::vector<int> &muselectorsmap, 
      const std::vector<int> &trkidx) {

    // zero out cache. initialze to -1 since these 
    // correspond to missing values
    epid = std::vector<int>(trkidx.size(), -1);
    mupid = std::vector<int>(trkidx.size(), -1);

    // loop through each candidate and assign pid levels
    for (size_t i = 0; i < trkidx.size(); ++i) {

      // only set the map value if the candidate maps to a valid track
      if (trkidx[i] >= 0) {
        epid[i] = deduce_epid_levels(eselectorsmap[trkidx[i]]);
        mupid[i] = deduce_mupid_levels(muselectorsmap[trkidx[i]]);
      }
    }
  }

}

// epid: -1 for missing value, 0-6 for others. 
// mupid: -1 for missing value, 0-4 for others. 
void RecoFeatureExtractor::extract_pid(
    const std::vector<int> &ltrkidx, const std::vector<int> &htrkidx, 
    const std::vector<int> &eselectorsmap, 
    const std::vector<int> &muselectorsmap) {
  populate_pid(l_epid_, l_mupid_, eselectorsmap, muselectorsmap, ltrkidx);
  populate_pid(h_epid_, h_mupid_, eselectorsmap, muselectorsmap, htrkidx);
}


// Graph building
// --------------

// build the reconstruction graph and attach all internal attributes
void RecoFeatureExtractor::build_reconstruction_graph(
    int n_vertices, int n_edges, 
    const std::vector<int> &from_vertices, 
    const std::vector<int> &to_vertices, 
    const std::vector<int> &lund_id, 
    const std::vector<std::vector<int>> &recoblock_global_indices) {

  // construct the graph itself
  construct_graph(g_, idx2vtx_, n_vertices, n_edges, 
                  from_vertices, to_vertices);

  // attach internal properties 
  populate_lund_id(g_, lund_id);
  populate_local_idx(g_, recoblock_global_indices);
}


void RecoFeatureExtractor::construct_graph(
    Graph &g, std::vector<Vertex> &idx2vtx,
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
  idx2vtx.clear();

  // establish a mapping between vertex index and vertex descriptors
  std::vector<Vertex> vertex_map(n_vertices);

  // insert vertices and bind internal properties
  for (int i = 0; i < n_vertices; ++i) {
    Vertex u = boost::add_vertex(g);
    vertex_map[i] = u;
    g[u].idx_ = i;
  }
  idx2vtx = vertex_map;

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
    const std::vector<std::vector<int>> &recoblock_global_indices) {
  for (const auto &index_v : recoblock_global_indices) {
    for (size_t i = 0; i < index_v.size(); ++i) {
      g[idx2vtx_[index_v[i]]].local_idx_ = i;
    }
  }
}


