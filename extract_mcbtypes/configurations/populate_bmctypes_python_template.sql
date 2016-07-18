BEGIN;

CREATE TABLE bmctypes_{0} (
  eid bigint,
  b1_mctype bmctype,
  b2_mctype bmctype
);

\copy bmctypes_{0} FROM 'bmctypes_{0}.csv' WITH CSV HEADER;

CREATE INDEX ON bmctypes_{0} (eid);

COMMIT;

VACUUM ANALYZE bmctypes_{0};
