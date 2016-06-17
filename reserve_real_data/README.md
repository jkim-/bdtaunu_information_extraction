Reserve real data
---

These scripts decide whether a particular datum from the detector data ("real data") belongs to an unblinded set. 

+ `reserved_data` is the table whose columns are indicators of whether a particular datum belongs to the sample indicated by the column name. 

+ The `unblinded_control` column indicates whether a datum is unblinded for the purpose of overall feature inspection. This sample is approximately 1\% of the detector data, and is obtained by uniform random sampling. 

+ Datum for which all columns are 0 remain blinded. These points consist of the sample where the measurement will be performed. 
