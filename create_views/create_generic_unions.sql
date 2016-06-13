CREATE VIEW upsilon_candidates_generic AS
  SELECT * FROM upsilon_candidates_sp1237 UNION ALL
  SELECT * FROM upsilon_candidates_sp1235 UNION ALL
  SELECT * FROM upsilon_candidates_sp1005 UNION ALL
  SELECT * FROM upsilon_candidates_sp998;

CREATE VIEW event_level_features_generic AS
  SELECT * FROM event_level_features_sp1237 UNION ALL
  SELECT * FROM event_level_features_sp1235 UNION ALL
  SELECT * FROM event_level_features_sp1005 UNION ALL
  SELECT * FROM event_level_features_sp998;
