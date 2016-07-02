-- Create table containing unique babarID's
CREATE TABLE unique_eid_data AS
SELECT t1.eid, t1.platform, t1.partition, t1.upperid, t1.lowerid
FROM framework_ntuples_data t1
WHERE NOT EXISTS (
  SELECT 1 FROM duplicate_records t2
  WHERE t1.platform = t2.platform 
  AND t1.partition = t2.partition
  AND t1.upperid = t2.upperid
  AND t1.lowerid = t2.lowerid
);

CREATE INDEX ON unique_eid_data (platform, partition, upperid, lowerid);
CREATE INDEX ON unique_eid_data (eid);

VACUUM ANALYZE unique_eid_data;
