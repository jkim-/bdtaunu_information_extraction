CREATE MATERIALIZED VIEW feature_extraction_input_sp1237 AS 
SELECT
  eid, 
  reco_n_vertices,
  reco_n_edges,
  reco_from_vertices,
  reco_to_vertices,
  reco_lund_id,
  y_reco_idx,
  b_reco_idx,
  d_reco_idx,
  c_reco_idx,
  h_reco_idx,
  l_reco_idx,
  gamma_reco_idx, 
  ltrkidx, 
  htrkidx,
  eselectorsmap, 
  muselectorsmap, 
  dmass
FROM 
  framework_ntuples_sp1237 INNER JOIN graph_sp1237 using (eid);
