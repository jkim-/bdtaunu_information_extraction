/*
-- Create table containing unique babarID's
CREATE TABLE unique_eid_sigmc AS
SELECT t1.platform, t1.partition, t1.upperid, t1.lowerid
FROM framework_ntuples_addnl_cols_sigmc t1
WHERE NOT EXISTS (
  SELECT 1 FROM duplicate_records t2
  WHERE t1.platform = t2.platform 
  AND t1.partition = t2.partition
  AND t1.upperid = t2.upperid
  AND t1.lowerid = t2.lowerid
);

CREATE INDEX ON unique_eid_sigmc (platform, partition, upperid, lowerid);

-- Create temporary table containing eid's of the unique babarID's
CREATE TEMP TABLE unique_eid AS
SELECT eid, platform, partition, upperid, lowerid 
FROM framework_ntuples_sigmc
INNER JOIN unique_addnl_cols_sigmc
USING (platform, partition, upperid, lowerid);

ALTER TABLE unique_eid_sigmc ADD COLUMN eid int DEFAULT NULL;

--?? what is happening
UPDATE unique_eid_sigmc t1 
SET eid = t2.eid
FROM framework_ntuples_sigmc t2
WHERE t1.platform = t2.platform 
AND t1.partition = t2.partition
AND t1.upperid = t2.upperid
AND t1.lowerid = t2.lowerid;

CREATE INDEX ON unique_eid_sigmc (eid);
*/

-- Create table containing unique babarID's
CREATE TABLE unique_eid_sigmc AS
SELECT t1.eid, t1.platform, t1.partition, t1.upperid, t1.lowerid
FROM framework_ntuples_sigmc t1
WHERE NOT EXISTS (
  SELECT 1 FROM duplicate_records t2
  WHERE t1.platform = t2.platform 
  AND t1.partition = t2.partition
  AND t1.upperid = t2.upperid
  AND t1.lowerid = t2.lowerid
);

CREATE INDEX ON unique_eid_sigmc (platform, partition, upperid, lowerid);
CREATE INDEX ON unique_eid_sigmc (eid);
