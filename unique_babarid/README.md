Select unique records
---

These scripts determine the uniqueness of each records in terms of their "BaBar ID", which is a set
of 4 integers: platform, partition, upperID, and lowerID. 

By construction, eid's are unique but the BaBar ID's are not. However, it turns out that
the number of records whose Babar ID non-unique is very small (<300). 

+ `duplicate_records` is the table containing eid's and Babar ID's of records 
  whose Babar ID is not unique. 

+ `unique_eid_[data|generic|sigmc]` are the tables containing eid's and BaBarID's of valid (i.e. unique) 
records. We actually see that all duplicate records come from the generic MC.
