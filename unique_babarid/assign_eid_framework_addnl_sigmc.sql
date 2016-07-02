BEGIN;

-- create indices and intermediate tables
CREATE INDEX framework_ntuples_addnl_cols_sigmc_babarid_idx 
ON framework_ntuples_addnl_cols_sigmc (platform, partition, upperid, lowerid);

CREATE TEMPORARY TABLE unique_eid_sigmc ON COMMIT DROP AS 
SELECT eid, platform, partition, upperid, lowerid 
FROM framework_ntuples_sigmc 
WHERE NOT EXISTS (
  SELECT 1 FROM duplicate_records 
  WHERE framework_ntuples_sigmc.eid=duplicate_records.eid
);
CREATE INDEX ON unique_eid_sigmc (platform, partition, upperid, lowerid);


-- add a eid column 
ALTER TABLE framework_ntuples_addnl_cols_sigmc ADD COLUMN eid bigint;

UPDATE framework_ntuples_addnl_cols_sigmc AS t1
SET eid = t2.eid
FROM unique_eid_sigmc AS t2
WHERE 
t1.platform = t2.platform AND
t1.partition = t2.partition AND 
t1.upperid = t2.upperid AND 
t1.lowerid = t2.lowerid;

-- create indices 
DROP INDEX framework_ntuples_addnl_cols_sigmc_babarid_idx;
CREATE INDEX ON framework_ntuples_addnl_cols_sigmc (eid);

COMMIT;

VACUUM ANALYZE framework_ntuples_addnl_cols_sigmc;
