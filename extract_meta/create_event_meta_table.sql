CREATE TABLE event_meta (
  eid bigint,
  event_weight real
);

\copy event_meta FROM 'event_meta.csv' WITH CSV HEADER;

CREATE INDEX ON event_meta (eid);
