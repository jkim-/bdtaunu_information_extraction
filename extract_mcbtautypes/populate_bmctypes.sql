BEGIN;

CREATE TABLE bmctypes_sp1235 (
  eid bigint,
  b1_mctype bmctype,
  b2_mctype bmctype
);

\copy bmctypes_sp1235 FROM 'bmctypes_sp1235.csv' WITH CSV HEADER;

CREATE INDEX ON bmctypes_sp1235 (eid);

COMMIT;

VACUUM ANALYZE bmctypes_sp1235;
