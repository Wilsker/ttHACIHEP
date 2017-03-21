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
workingDir = os.getcwd()
inputDir = '/publicfs/cms/data/TopQuark/ttHbb/JTW/2017_03/ttHACIHEP/output/'
inputFiles = ["MC/altTUNE/ttjets_altTUNE_Merged_rootplas", "ttHbb_Merged_rootplas"]

XS = {
    "ttjets_SL_Merged_rootplas":831.76,
    "ttHbb_Merged_rootplas":0.5071
}

BR = {
    "ttjets_SL_Merged_rootplas":1,
    "ttHbb_Merged_rootplas":0.5824
}

for file0 in inputFiles:
    inputFullPath = os.path.join(inputDir,file0)
    print 'Input file = ', inputFullPath
    tfile = ROOT.TFile(inputFullPath)
    tree = tfile.Get("BOOM")

    N0 = 0
    N0 = range(ttree.GetEntries())
    print '# Entries = ' , N0

    for key,value in XS.iteritems():
        if key in file0:
            print 'Got XS = ', value
    for key,value in BR.iteritems():
        if key in file0:
            print 'Got BR = ', value

    for events in range(ttree.GetEntries()):
        ttree.GetEntry(events)
