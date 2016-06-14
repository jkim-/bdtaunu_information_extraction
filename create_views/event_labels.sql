-- data
BEGIN;

CREATE MATERIALIZED VIEW event_labels_data AS
SELECT eid, mode_label, run FROM framework_ntuples_data;

CREATE INDEX ON event_labels_data (eid);

COMMIT;

VACUUM ANALYZE event_labels_data;


-- sigmc
BEGIN;

CREATE MATERIALIZED VIEW event_labels_sigmc AS
SELECT eid, mode_label, run, eventlabel FROM framework_ntuples_sigmc;

CREATE INDEX ON event_labels_sigmc (eid);

COMMIT;

VACUUM ANALYZE event_labels_sigmc;


-- generic
BEGIN;

CREATE MATERIALIZED VIEW event_labels_generic AS
SELECT eid, mode_label, run, eventlabel FROM framework_ntuples_sp1237 UNION ALL
SELECT eid, mode_label, run, eventlabel FROM framework_ntuples_sp1235 UNION ALL
SELECT eid, mode_label, run, eventlabel FROM framework_ntuples_sp1005 UNION ALL
SELECT eid, mode_label, run, eventlabel FROM framework_ntuples_sp998;

CREATE INDEX ON event_labels_generic (eid);

COMMIT;

VACUUM ANALYZE event_labels_generic;
