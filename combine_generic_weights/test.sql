BEGIN;

CREATE TEMPORARY VIEW aux_weights_sp1235 AS 
SELECT 
  eid,
  COALESCE(brf_correction_weight, 1.0) AS brf_correction_weight,
  COALESCE(cln_weight, 1.0) AS cln_weight,
  COALESCE(llswb1_weight, 1.0) AS llswb1_weight,
  COALESCE(llswb2_weight, 1.0) AS llswb2_weight
FROM
brf_weights_sp1235 
FULL OUTER JOIN 
formfactor_weights_sp1235 
USING (eid);

CREATE TEMPORARY VIEW aux_weights_sp1237 AS 
SELECT 
  eid,
  COALESCE(brf_correction_weight, 1.0) AS brf_correction_weight,
  COALESCE(cln_weight, 1.0) AS cln_weight,
  COALESCE(llswb1_weight, 1.0) AS llswb1_weight,
  COALESCE(llswb2_weight, 1.0) AS llswb2_weight
FROM
brf_weights_sp1237 
FULL OUTER JOIN 
formfactor_weights_sp1237 
USING (eid);

CREATE TEMPORARY TABLE aux_weights ON COMMIT DROP AS
  SELECT * FROM aux_weights_sp1237 UNION ALL
  SELECT * FROM aux_weights_sp1235;

CREATE INDEX ON aux_weights (eid);


CREATE TABLE event_weights_generic_augmented AS 
SELECT 
  eid,
  weight AS lumi_weight,
  COALESCE(cln_weight, 1.0) AS cln_weight,
  COALESCE(llswb1_weight, 1.0) AS llswb1_weight,
  COALESCE(llswb2_weight, 1.0) AS llswb2_weight
FROM 
event_weights_generic
LEFT OUTER JOIN 
aux_weights
USING (eid);

CREATE INDEX ON event_weights_generic_augmented (eid);

COMMIT;
