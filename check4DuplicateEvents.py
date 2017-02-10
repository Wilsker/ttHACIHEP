'''
========================================
        check4DuplicateEvents.py
========================================
    Joshuha Thomas-Wilsker, IHEP 2017
========================================
Code to check files output by BSMFramework for duplicate
event numbers (variable = EVENT_event).
'''

import ROOT, sys, math, os
workingDir = os.getcwd()
#inputDir = "input"
#inputFile = "ttHbb_MC.root"
#inputFile = "Data_RunsB-G_DL.root"
inputDir = "output"
inputFile = "ttHbb_MC_SS.root"

inputFullPath = os.path.join(workingDir,inputDir,inputFile)
tfile = ROOT.TFile(inputFullPath)

#ttree = tfile.Get("TNT/BOOM")
ttree = tfile.Get("BOOM")

evtList = []
dupList = []
for ev in range(ttree.GetEntries()):
    ttree.GetEntry(ev)
    if ttree.EVENT_event not in evtList:
        #print ttree.EVENT_event
        evtList.append(ttree.EVENT_event)
    else:
        print ttree.EVENT_event
        dupList.append(ttree.EVENT_event)

print 'Checked file name: '
print inputFullPath
print 'Number of duplicate EVENT_event entries found = ', len(dupList)
print 'List of all duplicated events: '
print dupList
