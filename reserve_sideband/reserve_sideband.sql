BEGIN;

-- eextra sideband
ALTER TABLE reserved_data ADD COLUMN 
eextra_sideband integer DEFAULT 0;

CREATE TEMPORARY TABLE temp_eextra_sideband ON COMMIT DROP AS
SELECT eid FROM candidate_optimized_events_data WHERE eextra >= 3;
CREATE INDEX ON temp_eextra_sideband (eid);

UPDATE reserved_data as R SET eextra_sideband=1 
FROM temp_eextra_sideband AS A WHERE R.eid=A.eid;

ALTER TABLE reserved_data ALTER eextra_sideband DROP DEFAULT;

-- mmiss2prime sideband
ALTER TABLE reserved_data ADD COLUMN 
mmiss2prime_sideband integer DEFAULT 0;

CREATE TEMPORARY TABLE temp_mmiss2prime_sideband ON COMMIT DROP AS
SELECT eid FROM candidate_optimized_events_data WHERE mmiss2prime <= -15;
CREATE INDEX ON temp_mmiss2prime_sideband (eid);

UPDATE reserved_data as R SET mmiss2prime_sideband=1 
FROM temp_mmiss2prime_sideband AS A WHERE R.eid=A.eid;

ALTER TABLE reserved_data ALTER mmiss2prime_sideband DROP DEFAULT;

-- sighp3 sideband
ALTER TABLE reserved_data ADD COLUMN 
sighp3_sideband integer DEFAULT 0;

CREATE TEMPORARY TABLE temp_sighp3_sideband ON COMMIT DROP AS
SELECT eid FROM candidate_optimized_events_data WHERE sig_hp3 >= 2.3;
CREATE INDEX ON temp_sighp3_sideband (eid);

UPDATE reserved_data as R SET sighp3_sideband=1 
FROM temp_sighp3_sideband AS A WHERE R.eid=A.eid;

ALTER TABLE reserved_data ALTER sighp3_sideband DROP DEFAULT;

COMMIT;

VACUUM ANALYZE reserved_data;
