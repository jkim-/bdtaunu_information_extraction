BEGIN;

CREATE TABLE bmctypes_dssdpipi (
  eid bigint,
  b1_mctype bmctype,
  b2_mctype bmctype
);

\copy bmctypes_dssdpipi FROM 'bmctypes_dssdpipi.csv' WITH CSV HEADER;

CREATE INDEX ON bmctypes_dssdpipi (eid);

COMMIT;

VACUUM ANALYZE bmctypes_dssdpipi;
