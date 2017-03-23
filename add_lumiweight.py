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

#====== Uncomment before committing to git -> For running on IHEP farm.
inputDir = '/publicfs/cms/data/TopQuark/ttHbb/JTW/2017_03/ttHACIHEP/output/MC/'
inputFiles = ["ttHbb/ttHbb_Merged_rootplas.root",
              "ttH_nonbb/ttH_nonbb_Merged_rootplas.root",
              "ttjets_altTUNE/ttjets_altTUNE_Merged_rootplas.root",
              "stop_schan/stop_schan_Merged_rootplas.root",
              "stop_tchan/stop_tchan_Merged_rootplas.root",
              "stop_tchan_antitop/stop_tchan_antitop_Merged_rootplas.root",
              "stop_tW_antitop/stop_tW_antitop_Merged_rootplas.root",
              "stop_tw_top/stop_tw_top_Merged_rootplas.root",
              "ttjets_bfilter/ttjets_bfilter_Merged_rootplas.root",
              "ttjets_DL/ttjets_DL_Merged_rootplas.root",
              "ttjets_SL/ttjets_SL_Merged_rootplas.root",
              "ttWjets/ttWjets_Merged_rootplas.root",
              "ttWjets_SL_ext1/ttWjets_SL_ext1_Merged_rootplas.root",
              "ttWjets_SL_ext2/ttWjets_SL_ext2_Merged_rootplas.root",
              "ttZ_DL/ttZ_DL_Merged_rootplas.root",
              "ttZ_qq/ttZ_qq_Merged_rootplas.root",
              "WW/WW_Merged_rootplas.root",
              "WW_ext/WW_ext_Merged_rootplas.root",
              "WZ/WZ_Merged_rootplas.root",
              "WZ_ext/WZ_ext_Merged_rootplas.root",
              "ZZ/ZZ_Merged_rootplas.root",
              "ZZ_ext/ZZ_ext_Merged_rootplas.root"
              ]

#========= Ucomment here for testing on lxplus
#inputDir = '/afs/cern.ch/work/j/jthomasw/private/IHEP/CMSSW/CMSSW_8_0_26_patch2/src/TTH/ttHACIHEP/output/'
#inputFiles = ["ttHbb_Merged_rootplas.root","ttjets_altTUNE_Merged_rootplas.root"]

XS_dict = {
    "ttHbb_Merged_rootplas":0.5071,
    "ttH_nonbb_Merged_rootplas":0.5071,
    "ttjets_altTUNE_Merged_rootplas":831.76,
    "stop_schan_Merged_rootplas":11.36,
    "stop_tchan_Merged_rootplas":136.02,
    "stop_tchan_antitop_Merged_rootplas":80.95,
    "stop_tW_antitop_Merged_rootplas":35.85,
    "stop_tw_top_Merged_rootplas":35.85,
    "ttjets_bfilter_Merged_rootplas":1,#Non-trivial dataset extension (see luminosity weight)
    "ttjets_DL_Merged_rootplas":831.76,
    "ttjets_SL_Merged_rootplas":831.76,
    "ttWjets_Merged_rootplas":0.4062,
    "ttWjets_SL_ext1_Merged_rootplas":0.2043,
    "ttWjets_SL_ext2_Merged_rootplas":0.2043,
    "ttZ_DL_Merged_rootplas":0.2529,
    "ttZ_qq_Merged_rootplas":0.5297,
    "WW_Merged_rootplas":118.7,
    "WW_ext_Merged_rootplas":118.7,
    "WZ_Merged_rootplas":(0.106 + 0.0663),
    "WZ_ext_Merged_rootplas":(0.106 + 0.0663),
    "ZZ_Merged_rootplas":0.0719,
    "ZZ_ext_Merged_rootplas":0.0719,
}

BR_dict = {
    "ttHbb_Merged_rootplas":0.5824,
    "ttH_nonbb_Merged_rootplas":0.5824,
    "ttjets_altTUNE_Merged_rootplas":1,
    "ttjets_SL_Merged_rootplas":2*0.6741*(1-0.6741),
    "ttjets_bfilter_Merged_rootplas":1,#Non-trivial dataset extension (see luminosity weight)
    "ttjets_DL_Merged_rootplas":(1-0.6741)**2,
    "stop_schan_Merged_rootplas":(1-0.6741),
    "stop_tchan_Merged_rootplas":1,
    "stop_tchan_antitop_Merged_rootplas":1,
    "stop_tw_top_Merged_rootplas":1,
    "stop_tW_antitop_Merged_rootplas":1,
    "WW_Merged_rootplas":1,
    "WW_ext_Merged_rootplas":1,
    "WZ_Merged_rootplas":3**2/(1-0.6741)/(3*0.033658),
    "WZ_ext_Merged_rootplas":3**2/(1-0.6741)/(3*0.033658),
    "ZZ_Merged_rootplas":(3 / (3*0.033658))**2 / 2,
    "ZZ_ext_Merged_rootplas":(3 / (3*0.033658))**2 / 2,
    "ttWjets_SL_ext1_Merged_rootplas":1,
    "ttWjets_SL_ext2_Merged_rootplas":1,
    "ttWjets_Merged_rootplas":1,
    "ttZ_DL_Merged_rootplas":1,
    "ttZ_qq_Merged_rootplas":1
}

for file0 in inputFiles:
    inputFullPath = os.path.join(inputDir,file0)
    print 'Input file = ', inputFullPath
    if os.path.isfile(inputFullPath):
        print 'File exists'
    else: continue

    for key,value in XS_dict.iteritems():
        if key in file0:
            output_tfile_name = key

    # Construct new variable.
    lumiweight_ = array('f',[0.])

    tfile = ROOT.TFile(inputFullPath,'Update')
    ttree = tfile.Get("BOOM")

    b_lumiweight = ttree.Branch("lumiweight",lumiweight_, "lumiweight/F")

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
            #print 'key does not match this file name.'
            #print 'Moving to next file'
            continue

    for key,value in BR_dict.iteritems():
        if key in file0:
            print 'Got BR = ', value
            BR = value
        else:
            #print 'key does not match this file name.'
            #print 'Moving to next file'
            continue

    W = (XS*BR)/N0
    print 'Weight for this sample = ', W
    for events in range(ttree.GetEntries()):
        ttree.GetEntry(events)
        lumiweight_[0] = W
        b_lumiweight.Fill()
    tfile.Write()
    tfile.Save()
    tfile.Close()
