Event weights for simulated data
---

+ Python scripts with the `ltda` prefix are run on the babar LTDA. They produce a columnar output that can be copy and pasted into the files `*_weights.dat`.

+ The weights are computed based on the following datasets:

  + Data: 

        AllEventsSkim-Run[1-6]-OnPeak-R24a1-v03

        AllEventsSkim-Run[1-6]-OffPeak-R24a1-v03

  + Generic MC:

        SP-[mode_num]-AllEventsSkim-Run[1-6]-R24a1-v03

        mode_num = { 1237, 1235, 1005, 998 }

  + Signal MC:

        SP-[mode_num]-Run[1-6]-R24

        mode_num = { 11444, 11445, 11446, 11447 }

+ For a more detailed explanation of how the weights are computed, go to the `doc` directory and type `make` to generate the documentation. 
