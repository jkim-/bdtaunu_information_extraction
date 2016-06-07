CREATE MATERIALIZED VIEW serializer_inputs_sigmc AS 
SELECT 
  eid, 
  ny, 

  ybpairmmiss2,
  ybpairmmissprime2, 
  ybpaireextra50, 
  ybpaircosthetat,
  ytagblp3magcm,
  ytagbcosby,
  ytagbcosthetadlcm,

  ytagbdstardeltam,
  ytagbcosthetadsoftcm,
  ytagbsoftp3magcm,
  ysigbhp3magcm,
  ysigbcosby,
  ysigbcosthetadtaucm,
  ysigbvtxprobb,
  ysigbdstardeltam,
  ysigbcosthetadsoftcm,
  ysigbsoftp3magcm,
  ysigbhmass,
  ysigbvtxprobh,

  y_tagb_idx,
  y_sigb_idx,
  b_istag,
  b_d_idx,
  b_lh_idx,
  d_isdstar,
  d_dmass,
  d_dmode,
  d_dstarmode,
  h_epid,
  h_mupid,
  l_epid,
  l_mupid

FROM 
  framework_ntuples_sigmc 
  INNER JOIN 
  additional_features_sigmc 
  USING (eid);
