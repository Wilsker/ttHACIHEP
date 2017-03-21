'''
========================================
        add_lumiweight.py
========================================
    Joshuha Thomas-Wilsker, IHEP 2017
========================================
Simply adding MC weight to rescale process
specific samples to datas lumiosity.

sample weight = (xsec * BR)/ N(sim. events)
W = (XS * BR) / N0

Would then multiply by L(data) to rescale sample.
'''

import ROOT, sys, math, os
from array import array
workingDir = os.getcwd()
inputDir = '/publicfs/cms/data/TopQuark/ttHbb/JTW/2017_03/ttHACIHEP/output/'
inputFiles = ["MC/ttjets_altTUNE/ttjets_altTUNE_Merged_rootplas.root", "MC/ttHbb/ttHbb_Merged_rootplas.root"]

XS_dict = {
    "ttjets_altTUNE_Merged_rootplas":831.76,
    "ttHbb_Merged_rootplas":0.5071
}

BR_dict = {
    "ttjets_altTUNE_Merged_rootplas":1,
    "ttHbb_Merged_rootplas":0.5824
}

for file0 in inputFiles:
    inputFullPath = os.path.join(inputDir,file0)
    print 'Input file = ', inputFullPath
    if os.path.isfile(inputFullPath):
        print 'File exists'
    else: continue
    tfile = ROOT.TFile(inputFullPath)
    ttree = tfile.Get("BOOM")

    lumiweight_ = array('f',[0.])
    ttree.Branch("lumiweight",lumiweight_, "lumiweight/F")

    N0 = 0
    XS = 0
    BR = 0
    N0 = ttree.GetEntries()
    print '# Entries = ' , N0

    for key,value in XS_dict.iteritems():
        if key in file0:
            print 'Got XS = ', value
            XS = value
        else:
            print 'No XS found for this file.'
            print 'Moving to next file'
            continue

    for key,value in BR_dict.iteritems():
        if key in file0:
            print 'Got BR = ', value
            BR = value
        else:
            print 'No BR found for this file.'
            print 'Moving to next file'
            continue

    W = (XS*BR)/N0
    print 'Weight for this sample = ', W

    for events in range(ttree.GetEntries()):
        ttree.GetEntry(events)
        lumiweight_[0] = W
        ttree.Fill()
    tfile.Write()
