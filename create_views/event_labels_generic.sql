BEGIN;

CREATE MATERIALIZED VIEW event_labels_generic AS
SELECT eid, mode_label, eventlabel FROM framework_ntuples_sp1237 UNION ALL
SELECT eid, mode_label, eventlabel FROM framework_ntuples_sp1235 UNION ALL
SELECT eid, mode_label, eventlabel FROM framework_ntuples_sp1005 UNION ALL
SELECT eid, mode_label, eventlabel FROM framework_ntuples_sp998;

CREATE INDEX ON event_labels_generic (eid);

COMMIT;

VACUUM ANALYZE event_labels_generic;
