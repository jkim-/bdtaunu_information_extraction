CREATE TABLE feature_extraction_input_sigmc AS 
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
  framework_ntuples INNER JOIN graph_sigmc using (eid);

CREATE INDEX ON feature_extraction_input_sigmc (eid);
