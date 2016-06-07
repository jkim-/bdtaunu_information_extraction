#! /usr/local/bin/python

import subprocess
import re

mc_modes = [ '11444', '11445', '11446', '11447' ]
run = [ '1', '2', '3', '4', '5', '6' ]

cond_p = { '11444' : 6.45e-5,
           '11445' : 12.04e-5,
           '11446' : 9.44e-5,
           '11447' : 24.78e-5 }


mc_dataset = [ (i, j) for i in mc_modes for j in run ]

data_Nbbar = []
lumi_template = 'lumi --dataset AllEventsSkim-Run{0}-OnPeak-R24a1 --dbname=bbkr24'
lumi_patt = re.compile('^Total *([0-9\.][0-9\.]*) \|')
for i in run:
    lumi_cmd  = lumi_template.format(i)
    print lumi_cmd

    lumi_output = subprocess.check_output(lumi_cmd.split())
    for line in lumi_output.strip().split('\n'):
        match = lumi_patt.search(line)
        if match:
            data_Nbbar.append(float(match.group(1)))

N_bbar = sum(data_Nbbar)
print
print 'Found {0} BBbar in the data.\n'.format(N_bbar)

mc_nevents = {}

bbkuser_template = ('BbkUser --dbname=bbkr24 '
                    '--dataset=SP-{0}-Run{1}-R24 '
                    'dataset events --summary')
bbkuser_patt = re.compile('^SP-[0-9][0-9]*-.* ([0-9][0-9]*)$')

for m in mc_modes:
    n_events = 0
    for r in run:
        bbkuser_cmd = bbkuser_template.format(m, r)
        print bbkuser_cmd
        bbkuser_output = subprocess.check_output(bbkuser_cmd.split())

        for line in bbkuser_output.strip().split('\n'):
            match = bbkuser_patt.search(line)
            if match:
                n_events += int(match.group(1))

    mc_nevents[m] = n_events

print
print 'Found the following event counts in each mc mode:'
print mc_nevents
print

mc_weights = {}
for m in mc_modes:
    w = N_bbar * cond_p[m] / mc_nevents[m]
    mc_weights[m] = w

print '{:<20}{:<20}{:<20}'.format('spmode', 'run', 'weight')
for m in mc_modes:
    for r in run:
        print '{:<20}{:<20}{:<20}'.format(m, r, mc_weights[m])
