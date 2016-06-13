BEGIN;

CREATE TABLE event_weights_{0} (
  eid bigint,
  weight numeric
);

\copy event_weights_{0} FROM 'mcevent_weights_{0}.csv' WITH CSV HEADER;

CREATE INDEX ON event_weights_{0} (eid);

COMMIT;

VACUUM ANALYZE event_weights_{0};
