-- generic
BEGIN;

CREATE MATERIALIZED VIEW candidate_optimized_events_generic AS 
SELECT * FROM 
  (optimal_upsilon_candidates_generic 
   INNER JOIN 
   upsilon_candidates_generic USING (eid, cidx))
  INNER JOIN
  event_level_features_generic USING (eid);

CREATE INDEX ON candidate_optimized_events_generic (eid);

COMMIT;

VACUUM ANALYZE candidate_optimized_events_generic;

-- data
BEGIN;

CREATE MATERIALIZED VIEW candidate_optimized_events_data AS 
SELECT * FROM 
  (optimal_upsilon_candidates_data 
   INNER JOIN 
   upsilon_candidates_data USING (eid, cidx))
  INNER JOIN
  event_level_features_data USING (eid);

CREATE INDEX ON candidate_optimized_events_data (eid);

COMMIT;

VACUUM ANALYZE candidate_optimized_events_data;

-- sigmc
BEGIN;

CREATE MATERIALIZED VIEW candidate_optimized_events_sigmc AS 
SELECT * FROM 
  (optimal_upsilon_candidates_sigmc 
   INNER JOIN 
   upsilon_candidates_sigmc USING (eid, cidx))
  INNER JOIN
  event_level_features_sigmc USING (eid);

CREATE INDEX ON candidate_optimized_events_sigmc (eid);

COMMIT;

VACUUM ANALYZE candidate_optimized_events_sigmc;
