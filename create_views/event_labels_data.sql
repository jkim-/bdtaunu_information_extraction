BEGIN;

CREATE MATERIALIZED VIEW event_labels_data AS
SELECT eid, mode_label FROM framework_ntuples_data;

CREATE INDEX ON event_labels_data (eid);

COMMIT;

VACUUM ANALYZE event_labels_data;
