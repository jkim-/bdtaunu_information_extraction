BEGIN;

CREATE TABLE reserved_data AS 
SELECT eid, run, mode_label, platform, partition, upperid, lowerid
FROM framework_ntuples_data;

CREATE INDEX ON reserved_data (eid);

ALTER TABLE reserved_data ADD COLUMN 
unblinded_control integer DEFAULT 0;

CREATE TEMPORARY TABLE aux_random_numbers AS
SELECT eid, random() AS r FROM reserved_data;

CREATE INDEX ON aux_random_numbers (eid);

UPDATE reserved_data as R
SET unblinded_control=1 
FROM aux_random_numbers as A
WHERE R.eid=A.eid AND A.r < 0.01;

ALTER TABLE reserved_data ALTER unblinded_control DROP DEFAULT;

COMMIT;

VACUUM ANALYZE reserved_data;

CREATE MATERIALIZED VIEW unblinded_control_data AS 
SELECT eid FROM reserved_data WHERE unblinded_control=1;

VACUUM ANALYZE unblinded_control_data;
