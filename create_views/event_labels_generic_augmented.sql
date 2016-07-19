BEGIN;

CREATE TEMPORARY VIEW bmctypes_all AS 
SELECT * FROM bmctypes_sp1235 UNION ALL
SELECT * FROM bmctypes_sp1237;

CREATE TEMPORARY TABLE grouped_dss_evttype AS 
SELECT 
  eid, 
  CASE WHEN 
        b1_mctype='Dstarstar_res' OR b2_mctype='Dstarstar_res' OR
        b1_mctype='Dstarstar_nonres' OR b2_mctype='Dstarstar_nonres' THEN 3
       ELSE 4
  END
  AS grouped_dss_evttype
FROM bmctypes_all;

CREATE INDEX ON grouped_dss_evttype (eid);

CREATE TABLE event_labels_generic_augmented AS
SELECT 
  eid, mode_label, run, eventlabel,

  CASE WHEN eventlabel=4 THEN 3
       WHEN eventlabel=5 THEN 4
       ELSE eventlabel
  END
  AS slhad_merged_eventlabel,

  CASE WHEN eventlabel=1 THEN 1
       WHEN eventlabel=2 THEN 2
       WHEN eventlabel=5 THEN 5
       WHEN grouped_dss_evttype is NULL THEN eventlabel
       ELSE grouped_dss_evttype
  END
  AS grouped_dss_evttype

FROM 
event_labels_generic LEFT OUTER JOIN grouped_dss_evttype USING (eid);

CREATE INDEX ON event_labels_generic_augmented (eid);

COMMIT;

VACUUM ANALYZE event_labels_generic_augmented;
