BEGIN;

CREATE TABLE event_weights_data AS SELECT eid FROM framework_ntuples_data;
ALTER TABLE event_weights_data ADD COLUMN weight numeric DEFAULT 1.0;
ALTER TABLE event_weights_data ALTER COLUMN weight DROP DEFAULT;
CREATE INDEX ON event_weights_data (eid);

COMMIT;

VACUUM ANALYZE event_weights_data;
