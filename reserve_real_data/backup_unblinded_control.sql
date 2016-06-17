BEGIN;

CREATE TEMPORARY VIEW unblinded_control_sample AS 
SELECT * FROM reserved_data WHERE unblinded_control=1;

\copy (SELECT * FROM unblinded_control_sample) TO 'unblinded_control.csv' WITH CSV HEADER;
