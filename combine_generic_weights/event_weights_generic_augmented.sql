BEGIN;

-- Create temporary tables containing weights that only apply to BBbar generic
------------------------------------------------------------------------------

-- branching fraction weights and form factor weights for sp1235
CREATE TEMPORARY VIEW aux_weights_sp1235 AS 
SELECT * 
FROM
brf_weights_sp1235 
FULL OUTER JOIN 
formfactor_weights_sp1235 
USING (eid);

-- branching fraction weights and form factor weights for sp1237
CREATE TEMPORARY VIEW aux_weights_sp1237 AS 
SELECT *
FROM
brf_weights_sp1237 
FULL OUTER JOIN 
formfactor_weights_sp1237 
USING (eid);

-- combine to single BBbar table
CREATE TEMPORARY TABLE aux_weights_bbar ON COMMIT DROP AS
  SELECT * FROM aux_weights_sp1237 UNION ALL
  SELECT * FROM aux_weights_sp1235;

CREATE INDEX ON aux_weights_bbar (eid);


-- Join the luminosity weights with the BBbar weights and the continuum weights
------------------------------------------------------------------------------

CREATE TABLE event_weights_generic_augmented AS 
SELECT 
  eid,
  weight AS lumi_weight,

  COALESCE(cln_weight, 1.0) AS cln_weight,
  COALESCE(linearq2_weight, 1.0) AS linearq2_weight,
  COALESCE(llswb1_weight, 1.0) AS llswb1_weight,
  COALESCE(llswb2_weight, 1.0) AS llswb2_weight,

  COALESCE(brf_correction_weight, 1.0) AS brf_correction_weight,
  COALESCE(b1_brf_mode, 'null') AS b1_brf_mode,
  COALESCE(b2_brf_mode, 'null') AS b2_brf_mode,

  COALESCE(comb_logre_weight, 1.0) AS sideband_comb_logre_weight,

  COALESCE(logre_density_weight, 1.0) AS continuum_logre_density_weight,
  COALESCE(logre_normalization_weight, 1.0) AS continuum_logre_normalization_weight,
  COALESCE(gbdt300_density_weight, 1.0) AS continuum_gbdt300_density_weight,
  COALESCE(gbdt300_normalization_weight, 1.0) AS continuum_gbdt300_normalization_weight

FROM 
  ( 
    (event_weights_generic LEFT OUTER JOIN aux_weights_bbar USING (eid)) 
    LEFT OUTER JOIN 
    sideband_scaled_weights USING (eid)
  )
  LEFT OUTER JOIN
  continuum_offpeak_scaled_weights
  USING (eid);

CREATE INDEX ON event_weights_generic_augmented (eid);

COMMIT;

VACUUM ANALYZE event_weights_generic_augmented;
