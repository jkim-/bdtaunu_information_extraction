#include "RecoFeatureExtractor.h"

const int D0Lund = 421;        
const int DcLund = 411;
const int Dstar0Lund = 423;
const int DstarcLund = 413;

RecoFeatureExtractor::RecoFeatureExtractor() 
  : d_catalogue_() { clear_cache(); }

RecoFeatureExtractor::~RecoFeatureExtractor() {}

void RecoFeatureExtractor::clear_cache() {
  g_.clear();
  idx2vtx_.clear();
  l_epid_.clear();
  l_mupid_.clear();
  h_epid_.clear();
  l_mupid_.clear();
  dmeson_mass_.clear();
  dmeson_mode_.clear();
  dstar_mode_.clear();
}

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
  

  clear_cache();

  construct_graph(g_, idx2vtx_, n_vertices, n_edges, from_vertices, to_vertices);
  populate_lund_id(g_, lund_id);
  populate_local_idx(g_, 
      { y_reco_idx, b_reco_idx, d_reco_idx, c_reco_idx, 
        h_reco_idx, l_reco_idx, gamma_reco_idx });

  extract_pid(ltrkidx, htrkidx, eselectorsmap, muselectorsmap);
  extract_dmeson_mass(d_reco_idx, dmass);
  extract_d_modes(d_reco_idx);

}

void RecoFeatureExtractor::cache_is_dstar(
    const std::vector<int> &d_reco_idx, 
    std::vector<int> &is_dstar) {

  is_dstar = std::vector<int>(d_reco_idx.size(), 0);
  for (int i = 0; i < d_reco_idx.size(); ++i) {
    int d_abslund = abs(g_[idx2vtx_[d_reco_idx[i]]].lund_id_);
    if (d_abslund == DstarcLund || d_abslund == Dstar0Lund) { 
      is_dstar[i] = 1; 
    }
  }
}

void RecoFeatureExtractor::assemble_decay_string(
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

  std::vector<int> is_dstar; cache_is_dstar(d_reco_idx, is_dstar);

  dmeson_mode_ = std::vector<int>(d_reco_idx.size(), -1);
  dstar_mode_ = std::vector<int>(d_reco_idx.size(), -1);

  std::vector<int> lund_list;
  for (int i = 0; i < d_reco_idx.size(); ++i) {

    assemble_decay_string(d_reco_idx[i], lund_list);

    if (!is_dstar[i]) { 
      dmeson_mode_[i] = d_catalogue_.get_d_mode(lund_list);
    } else {
      dstar_mode_[i] = d_catalogue_.get_dstar_mode(lund_list);
    }
  }

  for (int i = 0; i < d_reco_idx.size(); ++i) {
    if (is_dstar[i]) {

      Vertex u = idx2vtx_[d_reco_idx[i]];
      OutEdgeIter oe, oe_end;
      for (std::tie(oe, oe_end) = out_edges(u, g_); oe != oe_end; ++oe) {

        Vertex v = target(*oe, g_);
        int dau_abslund = abs(g_[v].lund_id_);
        if (dau_abslund == D0Lund || dau_abslund == DcLund) {
          dmeson_mode_[i] = dmeson_mode_[g_[v].local_idx_];
        }

      }
    }
  }

  return;
}

void RecoFeatureExtractor::extract_dmeson_mass(
    const std::vector<int> &d_reco_idx,
    const std::vector<float> &dmass) {

  assert(d_reco_idx.size() == dmass.size());

  std::vector<int> is_dstar(d_reco_idx.size(), 0);
  for (int i = 0; i < d_reco_idx.size(); ++i) {
    int d_abslund = abs(g_[idx2vtx_[d_reco_idx[i]]].lund_id_);
    if (d_abslund == DstarcLund || d_abslund == Dstar0Lund) { is_dstar[i] = 1; }
  }

  dmeson_mass_ = std::vector<float>(d_reco_idx.size(), -1);
  for (int i = 0; i < dmass.size(); ++i) {
    if (!is_dstar[i]) { dmeson_mass_[i] = dmass[i]; }
  }

  for (int i = 0; i < dmass.size(); ++i) {
    if (is_dstar[i]) { 

      Vertex u = idx2vtx_[d_reco_idx[i]];
      OutEdgeIter oe, oe_end;
      for (std::tie(oe, oe_end) = out_edges(u, g_); oe != oe_end; ++oe) {

        Vertex v = target(*oe, g_);
        int dau_abslund = abs(g_[v].lund_id_);
        if (dau_abslund == D0Lund || dau_abslund == DcLund) {
          dmeson_mass_[i] = dmeson_mass_[g_[v].local_idx_];
        }

      }
    }
  }

  return;
}

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

// epid: -1 for missing value, 0-6 for others. 
// mupid: -1 for missing value, 0-4 for others. 
void RecoFeatureExtractor::extract_pid(
    const std::vector<int> &ltrkidx, const std::vector<int> &htrkidx, 
    const std::vector<int> &eselectorsmap, 
    const std::vector<int> &muselectorsmap) {

  l_epid_ = std::vector<int>(ltrkidx.size(), -1);
  l_mupid_ = std::vector<int>(ltrkidx.size(), -1);
  for (size_t i = 0; i < ltrkidx.size(); ++i) {
    if (ltrkidx[i] >= 0) {
      l_epid_[i] = deduce_epid_levels(eselectorsmap[ltrkidx[i]]);
      l_mupid_[i] = deduce_mupid_levels(muselectorsmap[ltrkidx[i]]);
    }
  }

  h_epid_ = std::vector<int>(htrkidx.size(), -1);
  h_mupid_ = std::vector<int>(htrkidx.size(), -1);
  for (size_t i = 0; i < htrkidx.size(); ++i) {
    if (htrkidx[i] >= 0) {
      h_epid_[i] = deduce_epid_levels(eselectorsmap[htrkidx[i]]);
      h_mupid_[i] = deduce_mupid_levels(muselectorsmap[htrkidx[i]]);
    }
  }

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
