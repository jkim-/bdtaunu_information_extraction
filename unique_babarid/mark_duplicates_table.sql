CREATE TEMP TABLE babarid AS
SELECT eid, platform, partition, upperid, lowerid
FROM framework_ntuples_data
UNION ALL
SELECT eid, platform, partition, upperid, lowerid
FROM framework_ntuples_sp998
UNION ALL
SELECT eid, platform, partition, upperid, lowerid
FROM framework_ntuples_sp1005
UNION ALL
SELECT eid, platform, partition, upperid, lowerid
FROM framework_ntuples_sp1235
UNION ALL
SELECT eid, platform, partition, upperid, lowerid
FROM framework_ntuples_sp1237
UNION ALL
SELECT eid, platform, partition, upperid, lowerid
FROM framework_ntuples_sigmc;

CREATE INDEX ON babarid (platform, partition, upperid, lowerid);

CREATE TABLE duplicate_records AS
SELECT * FROM
(SELECT platform, partition, upperid, lowerid, count(*)
FROM babarid 
GROUP BY platform, partition, upperid, lowerid
HAVING count(*) > 1
) AS sub
INNER JOIN babarid 
USING (platform, partition, upperid, lowerid);
