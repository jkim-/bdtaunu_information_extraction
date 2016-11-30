BEGIN;

DROP TABLE IF EXISTS bmctypes_generic;

CREATE TABLE bmctypes_generic (
  eid          bigint,
  b1_mctype    bmctype,
  b2_mctype    bmctype,
  b1tau_mctype taumctype,
  b2tau_mctype taumctype
);

\copy bmctypes_generic FROM 'bmctypes_generic.csv' WITH CSV HEADER;

CREATE INDEX ON bmctypes_generic (eid);

COMMIT;

VACUUM ANALYZE bmctypes_generic;
