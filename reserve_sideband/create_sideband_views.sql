-- data

BEGIN;

CREATE MATERIALIZED VIEW sideband_data AS
SELECT 
  eid, 
  eextra_sideband,
--  mmiss2prime_sideband,
  sighp3_sideband,
--  (eextra_sideband=1 OR mmiss2prime_sideband=1 OR sighp3_sideband=1)::integer AS sideband
  (eextra_sideband=1 OR sighp3_sideband=1)::integer AS sideband
FROM reserved_data;

CREATE INDEX ON sideband_data (eid);

COMMIT;

VACUUM ANALYZE sideband_data;


-- generic

BEGIN;

CREATE MATERIALIZED VIEW sideband_generic AS
SELECT 
  eid, 
  S.eextra_sideband AS eextra_sideband,
--  S.mmiss2prime_sideband AS mmiss2prime_sideband,
  S.sighp3_sideband AS sighp3_sideband,
--  (eextra_sideband=1 OR mmiss2prime_sideband=1 OR sighp3_sideband=1)::integer AS sideband
  (eextra_sideband=1 OR sighp3_sideband=1)::integer AS sideband
FROM 
  (SELECT 
     eid,
     (eextra >= 3)::integer AS eextra_sideband,
--     (mmiss2prime <= -15)::integer AS mmiss2prime_sideband,
     (sig_hp3 >= 2.3)::integer AS sighp3_sideband
   FROM candidate_optimized_events_generic) AS S;

CREATE INDEX ON sideband_generic (eid);

COMMIT;

VACUUM ANALYZE sideband_generic;
