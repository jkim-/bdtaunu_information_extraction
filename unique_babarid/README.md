Select unique records
---

These scripts determine the uniqueness of each records in terms of their "BaBar ID", which is a set
of 4 integers: platform, partition, upperID, and lowerID.  These scripts can be run immediately
following data insertion using root2postgres.

By construction eid's are unique but the BaBar ID's are known to have duplicates.  Since the number
of such records is small (<300), we will proceed by excluding such records when performing the
analysis.  

+ `duplicate_records` is the table containing eid's and Babar ID's of records that do **not** have
unique BaBar ID's.

+ `unique_eid_[data|generic|sigmc]` are the tables containing eid's and BaBarID's of valid (i.e. unique) 
records. We actually see that all duplicate records come from the generic MC.

+ `example_download.sql` is a script showing an example of how to select the valid records by eid to
download the data.   
