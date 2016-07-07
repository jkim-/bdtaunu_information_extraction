BEGIN;

CREATE MATERIALIZED VIEW event_labels_generic_augmented AS
SELECT 
  eid, mode_label, run, eventlabel,
  CASE WHEN eventlabel=4 THEN 3
       WHEN eventlabel=5 THEN 4
       ELSE eventlabel
  END
  AS coalesced_eventlabel
FROM event_labels_generic;

CREATE INDEX ON event_labels_generic_augmented (eid);

COMMIT;

VACUUM ANALYZE event_labels_generic_augmented;
