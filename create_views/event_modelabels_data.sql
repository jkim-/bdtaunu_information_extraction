BEGIN;

CREATE MATERIALIZED VIEW event_modelabels_data AS
SELECT eid, mode_label FROM framework_ntuples_data;

CREATE INDEX ON event_modelabels_data (eid);

COMMIT;

VACUUM ANALYZE event_modelabels_data;
