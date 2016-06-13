BEGIN;

ALTER TABLE event_meta 
  ADD COLUMN bad_graph integer DEFAULT 0;

CREATE TEMPORARY TABLE bad_event_ids ON COMMIT DROP AS 
SELECT 
  f.eid AS bad_eid
FROM 
  framework_ntuples_sp1235 AS f LEFT OUTER JOIN graph_sp1235 AS g 
    ON f.eid = g.eid 
  WHERE g.eid IS NULL;

CREATE INDEX ON bad_event_ids (bad_eid);

UPDATE event_meta 
  SET bad_graph = 1
  FROM bad_event_ids 
  WHERE bad_event_ids.bad_eid=event_meta.eid;

COMMIT;
