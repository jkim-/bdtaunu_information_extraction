BEGIN;

CREATE TABLE upsilon_candidate_mclabels_sigmc (
  eid bigint,
  cidx integer,
  is_matched integer
);

\copy upsilon_candidate_mclabels_sigmc FROM 'upsilon_candidate_mclabels_sigmc.csv' WITH CSV HEADER;

CREATE INDEX ON upsilon_candidate_mclabels_sigmc (eid, cidx);

COMMIT;

VACUUM ANALYZE upsilon_candidate_mclabels_sigmc;
