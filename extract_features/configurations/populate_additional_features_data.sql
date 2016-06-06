BEGIN;

CREATE TABLE additional_features_data (
  eid integer, 
  y_tagb_idx integer[],
  y_sigb_idx integer[],
  b_d_idx integer[],
  b_lh_idx integer[],
  b_istag integer[],
  d_isdstar integer[],
  d_dmode integer[],
  d_dstarmode integer[],
  d_dmass real[],
  h_epid integer[],
  h_mupid integer[],
  l_epid integer[],
  l_mupid integer[]
);

\copy additional_features_data FROM 'additional_features_data.csv' WITH CSV HEADER;

DROP MATERIALIZED VIEW feature_extraction_input_data;

CREATE INDEX ON additional_features_data (eid);

COMMIT;
