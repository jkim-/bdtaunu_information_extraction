BEGIN;

CREATE MATERIALIZED VIEW event_modelabels_generic AS
SELECT eid, mode_label FROM framework_ntuples_sp1237 UNION ALL
SELECT eid, mode_label FROM framework_ntuples_sp1235 UNION ALL
SELECT eid, mode_label FROM framework_ntuples_sp1005 UNION ALL
SELECT eid, mode_label FROM framework_ntuples_sp998;

CREATE INDEX ON event_modelabels_generic (eid);

COMMIT;

VACUUM ANALYZE event_modelabels_generic;
