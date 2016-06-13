BEGIN;

CREATE MATERIALIZED VIEW event_weights_generic AS
SELECT eid, weight FROM event_weights_sp1237 UNION ALL
SELECT eid, weight FROM event_weights_sp1235 UNION ALL
SELECT eid, weight FROM event_weights_sp1005 UNION ALL
SELECT eid, weight FROM event_weights_sp998;

CREATE INDEX ON event_weights_generic (eid);

COMMIT;

VACUUM ANALYZE event_weights_generic;
