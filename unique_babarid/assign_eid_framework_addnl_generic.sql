-- sp1235

BEGIN;

-- create indices and intermediate tables
CREATE INDEX framework_ntuples_addnl_cols_sp1235_babarid_idx 
ON framework_ntuples_addnl_cols_sp1235 (platform, partition, upperid, lowerid);

CREATE TEMPORARY TABLE unique_eid_sp1235 ON COMMIT DROP AS 
SELECT eid, platform, partition, upperid, lowerid 
FROM framework_ntuples_sp1235 
WHERE NOT EXISTS (
  SELECT 1 FROM duplicate_records 
  WHERE framework_ntuples_sp1235.eid=duplicate_records.eid
);
CREATE INDEX ON unique_eid_sp1235 (platform, partition, upperid, lowerid);


-- add a eid column 
ALTER TABLE framework_ntuples_addnl_cols_sp1235 ADD COLUMN eid bigint;

UPDATE framework_ntuples_addnl_cols_sp1235 AS t1
SET eid = t2.eid
FROM unique_eid_sp1235 AS t2
WHERE 
t1.platform = t2.platform AND
t1.partition = t2.partition AND 
t1.upperid = t2.upperid AND 
t1.lowerid = t2.lowerid;

-- create indices 
DROP INDEX framework_ntuples_addnl_cols_sp1235_babarid_idx;
CREATE INDEX ON framework_ntuples_addnl_cols_sp1235 (eid);

COMMIT;

VACUUM ANALYZE framework_ntuples_addnl_cols_sp1235;



-- sp1237


BEGIN;

-- create indices and intermediate tables
CREATE INDEX framework_ntuples_addnl_cols_sp1237_babarid_idx 
ON framework_ntuples_addnl_cols_sp1237 (platform, partition, upperid, lowerid);

CREATE TEMPORARY TABLE unique_eid_sp1237 ON COMMIT DROP AS 
SELECT eid, platform, partition, upperid, lowerid 
FROM framework_ntuples_sp1237 
WHERE NOT EXISTS (
  SELECT 1 FROM duplicate_records 
  WHERE framework_ntuples_sp1237.eid=duplicate_records.eid
);
CREATE INDEX ON unique_eid_sp1237 (platform, partition, upperid, lowerid);


-- add a eid column 
ALTER TABLE framework_ntuples_addnl_cols_sp1237 ADD COLUMN eid bigint;

UPDATE framework_ntuples_addnl_cols_sp1237 AS t1
SET eid = t2.eid
FROM unique_eid_sp1237 AS t2
WHERE 
t1.platform = t2.platform AND
t1.partition = t2.partition AND 
t1.upperid = t2.upperid AND 
t1.lowerid = t2.lowerid;

-- create indices 
DROP INDEX framework_ntuples_addnl_cols_sp1237_babarid_idx;
CREATE INDEX ON framework_ntuples_addnl_cols_sp1237 (eid);

COMMIT;

VACUUM ANALYZE framework_ntuples_addnl_cols_sp1237;





-- sp1005


BEGIN;

-- create indices and intermediate tables
CREATE INDEX framework_ntuples_addnl_cols_sp1005_babarid_idx 
ON framework_ntuples_addnl_cols_sp1005 (platform, partition, upperid, lowerid);

CREATE TEMPORARY TABLE unique_eid_sp1005 ON COMMIT DROP AS 
SELECT eid, platform, partition, upperid, lowerid 
FROM framework_ntuples_sp1005 
WHERE NOT EXISTS (
  SELECT 1 FROM duplicate_records 
  WHERE framework_ntuples_sp1005.eid=duplicate_records.eid
);
CREATE INDEX ON unique_eid_sp1005 (platform, partition, upperid, lowerid);


-- add a eid column 
ALTER TABLE framework_ntuples_addnl_cols_sp1005 ADD COLUMN eid bigint;

UPDATE framework_ntuples_addnl_cols_sp1005 AS t1
SET eid = t2.eid
FROM unique_eid_sp1005 AS t2
WHERE 
t1.platform = t2.platform AND
t1.partition = t2.partition AND 
t1.upperid = t2.upperid AND 
t1.lowerid = t2.lowerid;

-- create indices 
DROP INDEX framework_ntuples_addnl_cols_sp1005_babarid_idx;
CREATE INDEX ON framework_ntuples_addnl_cols_sp1005 (eid);

COMMIT;

VACUUM ANALYZE framework_ntuples_addnl_cols_sp1005;





-- sp998


BEGIN;

-- create indices and intermediate tables
CREATE INDEX framework_ntuples_addnl_cols_sp998_babarid_idx 
ON framework_ntuples_addnl_cols_sp998 (platform, partition, upperid, lowerid);

CREATE TEMPORARY TABLE unique_eid_sp998 ON COMMIT DROP AS 
SELECT eid, platform, partition, upperid, lowerid 
FROM framework_ntuples_sp998 
WHERE NOT EXISTS (
  SELECT 1 FROM duplicate_records 
  WHERE framework_ntuples_sp998.eid=duplicate_records.eid
);
CREATE INDEX ON unique_eid_sp998 (platform, partition, upperid, lowerid);


-- add a eid column 
ALTER TABLE framework_ntuples_addnl_cols_sp998 ADD COLUMN eid bigint;

UPDATE framework_ntuples_addnl_cols_sp998 AS t1
SET eid = t2.eid
FROM unique_eid_sp998 AS t2
WHERE 
t1.platform = t2.platform AND
t1.partition = t2.partition AND 
t1.upperid = t2.upperid AND 
t1.lowerid = t2.lowerid;

-- create indices 
DROP INDEX framework_ntuples_addnl_cols_sp998_babarid_idx;
CREATE INDEX ON framework_ntuples_addnl_cols_sp998 (eid);

COMMIT;

VACUUM ANALYZE framework_ntuples_addnl_cols_sp998;
