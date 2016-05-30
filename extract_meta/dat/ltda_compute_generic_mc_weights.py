#! /usr/local/bin/python

import subprocess
import re

xsec = { '1237' : 525.0,
         '1235' : 525.0,
         '998' : 2090.0,
         '1005' : 1300.0,
         '3429' : 940.0 }
run = [ '1', '2', '3', '4', '5', '6' ]
mc_modes = [ '998', '1005', '3429', '1235', '1237' ]

mc_dataset = [ (i, j) for i in mc_modes for j in run ] 

data_lumi = {}
lumi_template = 'lumi --dataset AllEventsSkim-Run{0}-OnPeak-R24a1-v03 --dbname=bbkr24'
lumi_patt = re.compile('^ Lumi *Processed *([1-9\.][0-9\.]*)')
for i in run:
    lumi_cmd  = lumi_template.format(i)
    print lumi_cmd

    lumi_output = subprocess.check_output(lumi_cmd.split())
    for line in lumi_output.strip().split('\n'):
        match = lumi_patt.search(line)
        if match:
            data_lumi[i] = float(match.group(1))


mc_nevents = []
bbkuser_template = ('BbkUser --dbname=bbkr24 '
                    '--dataset=SP-{0}-AllEventsSkim-Run{1}-R24a1-v03 '
                    'dataset events --summary')
bbkuser_patt = re.compile('^SP-[0-9][0-9]*-.* ([0-9][0-9]*)$')

for i in mc_dataset:
        bbkuser_cmd = bbkuser_template.format(i[0],i[1])
        print bbkuser_cmd

        bbkuser_output = subprocess.check_output(bbkuser_cmd.split())

        for line in bbkuser_output.strip().split('\n'):
            match = bbkuser_patt.search(line)
            if match:
                mc_nevents.append(int(match.group(1)))

mc_scale = []
for i, dataset in enumerate(mc_dataset):
    gen_lumi = mc_nevents[i] / xsec[dataset[0]]
    scale = gen_lumi / data_lumi[dataset[1]]
    mc_scale.append(scale)

print '{:<20}{:<20}{:<20}{:<20}'.format('spmode', 'run', 'scale', 'weight')
for i, dataset in enumerate(mc_dataset):
    print '{:<20}{:<20}{:<20}{:<20}'.format(dataset[0], dataset[1], mc_scale[i], 1 / mc_scale[i])
