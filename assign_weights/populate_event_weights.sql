BEGIN;

CREATE TABLE event_weights_sigmc (
  eid bigint,
  weight numeric
);

\copy event_weights_sigmc FROM 'mcevent_weights_sigmc.csv' WITH CSV HEADER;

CREATE INDEX ON event_weights_sigmc (eid);

COMMIT;

VACUUM ANALYZE event_weights_sigmc;
