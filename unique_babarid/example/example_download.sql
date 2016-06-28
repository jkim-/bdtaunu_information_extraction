CREATE TEMP TABLE eid_sp11444 AS
SELECT eid, platform, partition, upperid, lowerid FROM 
unique_eid_sigmc
INNER JOIN (SELECT eid from event_labels_sigmc WHERE mode_label = 11444) AS sq
USING (eid);

CREATE TEMP TABLE addnl_cols_with_eid AS
SELECT eid, mcp3, mccosth, mcphi, mcenergy FROM
(framework_ntuples_addnl_cols_sigmc
INNER JOIN eid_sp11444
USING (platform, partition, upperid, lowerid))
;

CREATE INDEX ON addnl_cols_with_eid (eid);

CREATE TEMP TABLE all_cols AS
SELECT * FROM
(SELECT eid, mcmass, mcp3cm, mccosthcm, mcphicm, mcenergycm FROM framework_ntuples_sigmc) AS q
INNER JOIN addnl_cols_with_eid
USING (eid);

--\copy (SELECT eid, mcmass, mcp3, mcp3cm, mccosth, mccosthcm, mcphi, mcphicm, mcenergy, mcenergycm FROM all_cols) TO 'sigmc.csv' WITH CSV HEADER;
\copy (SELECT * FROM all_cols) TO 'sigmc.csv' WITH CSV HEADER;
