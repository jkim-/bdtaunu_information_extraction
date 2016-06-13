BEGIN;

CREATE MATERIALIZED VIEW event_labels_sigmc AS
SELECT eid, mode_label, eventlabel FROM framework_ntuples_sigmc;

CREATE INDEX ON event_labels_sigmc (eid);

COMMIT;

VACUUM ANALYZE event_labels_sigmc;
