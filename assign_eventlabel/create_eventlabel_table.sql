BEGIN;

DROP TABLE IF EXISTS event_labels_generic;

CREATE TABLE event_labels_generic AS
  SELECT eid,
         sp_label_events(b1_mctype, b2_mctype, 
                         b1tau_mctype, b2tau_mctype) AS eventlabel
  FROM bmctypes_generic;

CREATE INDEX ON event_labels_generic (eid);

COMMIT;

VACUUM ANALYZE event_labels_generic;
