"""
======================================
            makeTable.py
======================================
Description:
Generates synchronisation tables for dilepton and single lepton ttH analyses.
Code prints output to screen. To create .csv files required by sync.py, simply
re-direct printed output to .csv file:

    $>python makeTable.py > XXXX.csv

======================================
To Do:
        - Need to check all scale factors files in 'SF' directory are the correct files. Expecting update to 80x global tag.
        - Especially trigger SFs.
        - Uncomment electron trigger SFs section.
"""

import sys, math, os, csv, ROOT
from ROOT import TFile, TTree, gDirectory, gROOT, TCanvas, TF1, vector
from array import array
import datetime

workingDir = os.getcwd()
inputDir = "output"
#inputFile = 'synch_output_DATA_allRuns_SS.root'
inputFile = "SS_ttH_synch.root"
#inputFile = "ttjets_synch_SS.root"

if ("ttH" in inputFile): sample=1
elif ("ttjets" in inputFile): sample=2
elif ("RD" in inputFile or "Run" in inputFile or 'DATA' in inputFile): sample=0
else:
    print "Input file doesn't match any known sample name."
    print "Check file used from \'output/\' directory"

inputFullPath = os.path.join(workingDir,inputDir,inputFile)
if (os.path.isfile(inputFullPath) != True):
    print 'ERROR !!!! Input file:', inputFullPath,' does not exist! Exiting . . . '
    sys.exit(2)

# Output .csv
csvnom = inputFile[:-5]+'_synchTable.csv'
csv_writer = csv.writer(open(csvnom,'w'))
csv_writer.writerow(['run','lumi','event','is_e','is_mu','is_ee','is_emu','is_mumu','n_jets','n_btags','lep1_pt','lep1_iso','lep1_pdgId','lep2_pt','lep2_iso','lep2_pdgId','jet1_pt','jet1_eta','jet1_phi','jet1_jesSF','jet1_jesSF_up','jet1_jesSF_down','jet1_csv','jet2_pt','jet2_eta','jet2_phi','jet2_jesSF','jet2_jesSF_up','jet2_jesSF_down','jet2_csv','MET_pt','MET_phi','mll','ttHFCategory','n_interactions','puWeight','csvSF','csvSF_lf_up','csvSF_hf_down','csvSF_cErr1_down','triggerSF','lep_idSF','lep_isoSF','pdf_up','pdf_down','me_up','me_down','bdt_output','mem_output','dnn_ttH_output','dnn_ttbb_output'])

# === Output tuples
output_rootfile_dir = 'analysis_tuples/'
output_rootfile_fullpath = os.path.join(workingDir,output_rootfile_dir,inputFile)
out_tf = TFile(output_rootfile_fullpath,'recreate')
out_tt = TTree('physics','physics')

jet_0_pt = array('d', [ 0 ] )
out_tt.Branch('jet_0_pt',jet_0_pt,'jet_0_pt/d')


tf = TFile(inputFullPath)
tt = gDirectory.Get( 'BOOM' )

# >>>>>>> NEED to set using filenames
dataMU   = False
dataEL   = False
dataMUEL = False
data     = False
reHLT    = False
if(dataMU==True or dataEL==True or dataMUEL==True):
    data = True

print "Input tree has : ", tt.GetEntries() , " entries."
evtList = []
for ev in range(tt.GetEntries()):
    tt.GetEntry(ev)
    if(ev % 1000 == 0):
        print 'Index: ', ev

    ##INITIALIZE LEPTON VARIABLES
    muons_pt = tt.Muon_pt
    electrons_pt = tt.patElectron_pt
    SelLepton_pt = []
    SelLepton_id = []
    SelLepton_eta = []
    SelLepton_SCeta = []
    SelLepton_phi = []
    SelLepton_iso = []
    SelLepton_energy = []
    #if(tt.EVENT_event!=3222742 or tt.EVENT_event!=3278733 or tt.EVENT_event!=2602188):
    #    continue
    #print 'Event number = ', tt.EVENT_event
    #print '=================================='
#    print '=================================='

    #print "Event number: ", tt.EVENT_event

    ##MUON SELECTION
    for i in range(len(muons_pt)):
        '''print '>>>>> Muon <<<<<'
        print 'Muon # = ', i
        print 'Muon pt = ', muons_pt[i]
        print 'Muon ID = ' , tt.Muon_pdgId[i]
        print 'Muon eta = ', abs(tt.Muon_eta[i])
        print 'Is tight = ', tt.Muon_tight[i]
        print 'Muon relIsoDeltaBetaR04 = ', tt.Muon_relIsoDeltaBetaR04[i]'''
        if (muons_pt[i]>15 and math.fabs(tt.Muon_eta[i])<2.4 and tt.Muon_tight[i]==1 and tt.Muon_relIsoDeltaBetaR04[i]<0.25):
            SelLepton_pt.append(tt.Muon_pt[i])
            SelLepton_id.append(tt.Muon_pdgId[i])
            SelLepton_eta.append(tt.Muon_eta[i])
            SelLepton_SCeta.append(tt.Muon_eta[i])
            SelLepton_phi.append(tt.Muon_phi[i])
            SelLepton_iso.append(tt.Muon_relIsoDeltaBetaR04[i])
            SelLepton_energy.append(tt.Muon_energy[i])
            #print " Muon passed loose selection "

    #ELECTRON SELECTION
    for i in range(len(electrons_pt)):
        '''print '>>>>> Electron <<<<<'
        print 'Electron pt = ', tt.patElectron_pt[i]
        print 'Electron abs eta = ', math.fabs(tt.patElectron_eta[i])
        print 'Electron abs SC eta = ', abs(tt.patElectron_SCeta[i])
        print 'Electron abs dz = ',abs(tt.patElectron_gsfTrack_dz_pv[i])
        print 'Electron abs d0 = ', abs(tt.patElectron_d0[i])
        print 'Electron inCrack = ', tt.patElectron_inCrack[i]
        print 'Electron is medium = ', tt.patElectron_isPassMedium[i]'''
        d0cut = 0
        dzcut = 0
        if(abs(tt.patElectron_SCeta[i])<=1.479):#Barrel impact parameter cut
            d0cut = 0.05
            dzcut = 0.10
        if(abs(tt.patElectron_SCeta[i])>1.479):#End-cap impact parameter cut
            d0cut = 0.10
            dzcut = 0.20
        if (tt.patElectron_pt[i]>15 and math.fabs(tt.patElectron_eta[i])<2.4 and
            tt.patElectron_inCrack[i]==0 and tt.patElectron_isPassMedium[i]==1
            and abs(tt.patElectron_gsfTrack_dz_pv[i])<dzcut and
            abs(tt.patElectron_d0[i])<d0cut
            ):
            SelLepton_pt.append(tt.patElectron_pt[i])
            SelLepton_id.append(tt.patElectron_pdgId[i])
            SelLepton_eta.append(tt.patElectron_eta[i])
            SelLepton_SCeta.append(tt.patElectron_SCeta[i])
            SelLepton_phi.append(tt.patElectron_phi[i])
            SelLepton_iso.append(tt.patElectron_relIsoRhoEA[i])
            SelLepton_energy.append(tt.patElectron_energy[i])
            #print 'Passes loose electron selection'

    #INITIALIZE JET VARIABLES
    jet0_pt = -1
    jet0_eta = -1
    jet0_phi = -1
    jet0_csv = -1
    jet0_JecSF = -1
    jet0_JecSF_up = -1
    jet0_JecSF_down = -1
    jet0_JerSF = -1
    jet1_pt = -1
    jet1_eta = -1
    jet1_phi = -1
    jet1_csv = -1
    jet1_JecSF = -1
    jet1_JerSF = -1
    jet1_JecSF_up = -1
    jet1_JecSF_down = -1
    jet2_pt = -1
    jet2_eta = -1
    jet2_phi = -1
    jet2_csv = -1
    jet2_JecSF = -1
    jet2_JerSF = -1
    jet3_pt = -1
    jet3_eta = -1
    jet3_phi = -1
    jet3_csv = -1
    jet3_JecSF = -1
    jet3_JerSF = -1
    SelJet_pt = []
    SelJet_eta = []
    SelJet_phi = []
    SelJet_pfCombinedInclusiveSecondaryVertexV2BJetTags = []
    SelJet_JecSF = []
    SelJet_JecSFup = []
    SelJet_JecSFdown = []
    SelJet_JerSF = []
    SelTightJet_pt = []
    SelTightJet_eta = []
    SelTightJet_phi = []
    SelTightJet_pfCombinedInclusiveSecondaryVertexV2BJetTags = []
    SelTightJet_JecSF = []
    SelTightJet_JecSFup = []
    SelTightJet_JecSFdown = []
    SelTightJet_JerSF = []
    jets_pt = tt.Jet_pt

    ## LOOSE JET SELECTION
    for i in range(len(jets_pt)):
        jet_pt=(tt.Jet_Uncorr_pt[i]*tt.Jet_JesSF[i]*tt.Jet_JerSF[i])# Jets used must have JES and JER corrections applied to pt before cut.
        '''print '>>>>> Jet <<<<<'
        print 'Jet index = ', i
        print 'Uncorr. jet pt = ', tt.Jet_Uncorr_pt[i]
        print 'JES SF = ' , tt.Jet_JesSF[i]
        print 'JER SF = ', tt.Jet_JerSF[i]
        print 'Corrected Jet pt = ', jet_pt
        print 'abs jet eta = ', math.fabs(tt.Jet_eta[i])
        print 'Jet csv weight = ', tt.Jet_pfCombinedInclusiveSecondaryVertexV2BJetTags[i]
        print 'Jet neutral HAD E frac = ', tt.Jet_neutralHadEnergyFraction[i]
        print 'Jet charged HAD E frac = ',tt.Jet_chargedHadronEnergyFraction[i]
        print 'Jet neutral EM E frac = ', tt.Jet_neutralEmEnergyFraction[i]
        print 'Jet charged EM E frac = ', tt.Jet_chargedEmEnergyFraction[i]
        print 'Charged multiplicity = ', tt.Jet_chargedMultiplicity[i]'''
        # Jet kinematic & loose ID requirements
        if (jet_pt>20 and math.fabs(tt.Jet_eta[i])<2.4
            and tt.Jet_neutralHadEnergyFraction[i]<0.99
            and tt.Jet_chargedHadronEnergyFraction[i]>0.0
            and tt.Jet_neutralEmEnergyFraction[i]<0.99
            and tt.Jet_chargedEmEnergyFraction[i]<0.99
            and tt.Jet_numberOfConstituents[i]>1
            and tt.Jet_chargedMultiplicity[i]>0.0):
            deltaRJetLepBoolean = False
            #print 'Calculate dR(j,l) : '
            for j in range(len(SelLepton_pt)):
                #print 'lepton index = ', j
                deltaEta = SelLepton_eta[j]-tt.Jet_eta[i]
                deltaPhi = math.fabs(SelLepton_phi[j]-tt.Jet_phi[i])
                if(deltaPhi >= math.pi):
                    deltaPhi = 2*math.pi - deltaPhi
                dR_j_lep = math.sqrt(deltaEta*deltaEta + deltaPhi*deltaPhi)
                #print 'dR_j_lep = ', dR_j_lep
                if(dR_j_lep<=0.4):
                    deltaRJetLepBoolean = True
            if(deltaRJetLepBoolean==False):
                SelJet_pt.append(jet_pt)
                SelJet_eta.append(tt.Jet_eta[i])
                SelJet_phi.append(tt.Jet_phi[i])
                SelJet_pfCombinedInclusiveSecondaryVertexV2BJetTags.append(tt.Jet_pfCombinedInclusiveSecondaryVertexV2BJetTags[i])
                SelJet_JecSF.append(tt.Jet_JesSF[i])
                SelJet_JecSFup.append(tt.Jet_JesSFup[i]/tt.Jet_JesSF[i])
                SelJet_JecSFdown.append(tt.Jet_JesSFdown[i]/tt.Jet_JesSF[i])
                SelJet_JerSF.append(tt.Jet_JerSF[i])
                #print 'Passes loose jet selection'

    ##Tight JET SELECTION
    for i in range(len(SelJet_pt)):
        if(SelJet_pt[i]>30):
            SelTightJet_pt.append(SelJet_pt[i])
            SelTightJet_eta.append(SelJet_eta[i])
            SelTightJet_phi.append(SelJet_phi[i])
            SelTightJet_pfCombinedInclusiveSecondaryVertexV2BJetTags.append(SelJet_pfCombinedInclusiveSecondaryVertexV2BJetTags[i])
            SelTightJet_JecSF.append(SelJet_JecSF[i])
            SelTightJet_JecSFup.append(SelJet_JecSFup[i])
            SelTightJet_JecSFdown.append(SelJet_JecSFdown[i])
            SelTightJet_JerSF.append(SelJet_JerSF[i])
            #print 'Passes tight jet selection'

    #BTAG FOR SINGLE AND DILEPTON EVENTS
    nBCSVM = 0
    nBCSVM_SL = 0
    for i in range(len(SelTightJet_pt)):
        if (SelTightJet_pfCombinedInclusiveSecondaryVertexV2BJetTags[i]>=0.8484):
            nBCSVM_SL=nBCSVM_SL+1
    nBCSVM_DL = 0
    for i in range(len(SelJet_pt)):
        if (SelJet_pfCombinedInclusiveSecondaryVertexV2BJetTags[i]>=0.8484):
            nBCSVM_DL=nBCSVM_DL+1

    ##MET CORRECTION
    MET = tt.Met_type1PF_pt
    MET_phi = tt.Met_type1PF_phi

    ##DEFINE VARIABLES
    lep0_pt = -1
    lep0_id = -1
    lep0_eta = -1
    lep0_SCeta = -1
    lep0_phi = -1
    lep0_iso = -1
    lep1_pt = -1
    lep1_id = -1
    lep1_eta = -1
    lep1_SCeta = -1
    lep1_phi = -1
    lep1_iso = -1
    is_e = 0
    is_mu = 0
    is_ee = 0
    is_emu = 0
    is_mumu = 0
    triggerSF = 1
    lepIDSF = 1
    lepIsoSF = 1
    lep1IDSF  = 1
    lep1IsoSF = 1
    lep2IDSF  = 1
    lep2IsoSF = 1
    Q2_up = tt.EVENT_Q2tthbbWeightUp
    Q2_down = tt.EVENT_Q2tthbbWeightDown
    pdf_up = tt.EVENT_PDFtthbbWeightUp
    pdf_down = tt.EVENT_PDFtthbbWeightDown
    mll = -1
    mll_pass = 1
    met_pass = 1

    ##DILEPTON SELECTION
    #print 'Dilepton Selection'
    #print "len(SelLepton_pt) = " , len(SelLepton_pt)
    dilepSelection = True
    if (len(SelLepton_pt) == 2):
        lepCouplePt=-99
        for i in range(len(SelLepton_pt)):
            for j in range(i+1,len(SelLepton_pt)):
                lep0 = ROOT.TLorentzVector(0, 0, 0, 0)
                lep0.SetPtEtaPhiE(SelLepton_pt[i], SelLepton_eta[i], SelLepton_phi[i], SelLepton_energy[i])
                lep1 = ROOT.TLorentzVector(0, 0, 0, 0)
                lep1.SetPtEtaPhiE(SelLepton_pt[j], SelLepton_eta[j], SelLepton_phi[j], SelLepton_energy[j])
                mll = (lep0+lep1).M()
                #print "(lep0+lep1).M() = " , (lep0+lep1).M()
                if(SelLepton_pt[i]<25 and SelLepton_pt[j]<25):
                    dilepSelection=False
                if(SelLepton_id[i]*SelLepton_id[j]>0):
                    dilepSelection=False
                if((lep0+lep1).M()<20):
                    mll_pass=0
                    dilepSelection=False
                if(math.fabs(SelLepton_id[i])==math.fabs(SelLepton_id[j]) and (lep0+lep1).M()>76 and (lep0+lep1).M()<106):
                    mll_pass=0
                    dilepSelection=False
                if(math.fabs(SelLepton_id[i])==math.fabs(SelLepton_id[j]) and MET<40):
                    met_pass=0
                    dilepSelection=False
                if(dilepSelection==True and lepCouplePt<SelLepton_pt[i]+SelLepton_pt[j]):
                    lep0_pt = SelLepton_pt[i]
                    lep0_id = SelLepton_id[i]
                    lep0_eta = SelLepton_eta[i]
                    lep0_SCeta = SelLepton_SCeta[i]
                    lep0_phi = SelLepton_phi[i]
                    lep0_iso = SelLepton_iso[i]
                    lep0_energy = SelLepton_energy[i]
                    lep1_pt = SelLepton_pt[j]
                    lep1_id = SelLepton_id[j]
                    lep1_eta = SelLepton_eta[j]
                    lep1_SCeta = SelLepton_SCeta[j]
                    lep1_phi = SelLepton_phi[j]
                    lep1_iso = SelLepton_iso[j]
                    lep1_energy = SelLepton_energy[i]
                    lepCouplePt = SelLepton_pt[i]+SelLepton_pt[j]
                    lep_PROV_pt  = lep0_pt
                    lep_PROV_eta = lep0_eta
                    lep_PROV_phi = lep0_phi
                    lep_PROV_iso = lep0_iso
                    lep_PROV_id  = lep0_id
                    if(lep0_pt<lep1_pt):
                        lep0_pt  = lep1_pt
                        lep0_eta = lep1_eta
                        lep0_phi = lep1_phi
                        lep0_iso = lep1_iso
                        lep0_id  = lep1_id
                        lep1_pt  = lep_PROV_pt
                        lep1_eta = lep_PROV_eta
                        lep1_phi = lep_PROV_phi
                        lep1_iso = lep_PROV_iso
                        lep1_id  = lep_PROV_id
        if(lep0_pt<25):
            dilepSelection=False

    ##SELECT SINGLE-LEPTON EVENT
    NumberOfJets=-1
    SingleLeptonEvent=False
    METFilters=False
    electron=False
    muon=False
    jets=False


    if (data==True):
        # HACK removed MET filters not in first production
        #if(tt.Flag_goodVertices==1 and tt.Flag_globalTightHalo2016Filter==1 and tt.Flag_HBHENoiseFilter==1 and tt.Flag_HBHENoiseIsoFilter==1 and tt.Flag_EcalDeadCellTriggerPrimitiveFilter==1 and tt.Flag_eeBadScFilter==1):# tt.EVENT_filterBadGlobalMuonTagger==1 and tt.EVENT_filtercloneGlobalMuonTagger==1
        if(tt.Flag_goodVertices==1 and tt.Flag_HBHENoiseFilter==1 and tt.Flag_HBHENoiseIsoFilter==1 and tt.Flag_EcalDeadCellTriggerPrimitiveFilter==1 and tt.Flag_eeBadScFilter==1):# tt.EVENT_filterBadGlobalMuonTagger==1 and tt.EVENT_filtercloneGlobalMuonTagger==1
            METFilters=True
    else:
        # HACK removed MET filters not in first production
        #if(tt.Flag_goodVertices==1 and tt.Flag_globalTightHalo2016Filter==1 and tt.Flag_HBHENoiseFilter==1 and tt.Flag_HBHENoiseIsoFilter==1 and tt.Flag_EcalDeadCellTriggerPrimitiveFilter==1):# tt.EVENT_filterBadGlobalMuonTagger==1 and tt.EVENT_filtercloneGlobalMuonTagger==1
        if(tt.Flag_goodVertices==1 and tt.Flag_HBHENoiseFilter==1 and tt.Flag_HBHENoiseIsoFilter==1 and tt.Flag_EcalDeadCellTriggerPrimitiveFilter==1):# tt.EVENT_filterBadGlobalMuonTagger==1 and tt.EVENT_filtercloneGlobalMuonTagger==1
            METFilters=True
    if(len(SelTightJet_pt)>=4 and nBCSVM_SL>=2):
        jets=True
    if(len(SelLepton_pt)==1 and (math.fabs(SelLepton_id[0])==13 and SelLepton_pt[0]>26 and math.fabs(SelLepton_eta[0])<2.1 and SelLepton_iso[0]<0.15 and (tt.HLT_IsoMu24==1 or tt.HLT_IsoTkMu24==1))):
        muon = True
    if(len(SelLepton_pt)==1 and (math.fabs(SelLepton_id[0])==11 and SelLepton_pt[0]>30 and math.fabs(SelLepton_eta[0])<2.1 and tt.HLT_Ele27_eta2p1_WPTight_Gsf==1)):
        electron = True


    if((electron==True or muon==True) and jets==True and METFilters==True):
        if(electron==True):
            is_e=1
        if(muon==True):
            is_mu=1
        SingleLeptonEvent=True
        nBCSVM=nBCSVM_SL
        NumberOfJets=len(SelTightJet_pt)
        lep0_pt = SelLepton_pt[0]
        lep0_id = SelLepton_id[0]
        lep0_eta = SelLepton_eta[0]
        lep0_phi = SelLepton_phi[0]
        lep0_iso = SelLepton_iso[0]
        lep0_energy = SelLepton_energy[0]
        jet0_pt = SelTightJet_pt[0]
        jet0_eta = SelTightJet_eta[0]
        jet0_phi = SelTightJet_phi[0]
        jet0_csv = SelTightJet_pfCombinedInclusiveSecondaryVertexV2BJetTags[0]
        jet0_JecSF = SelTightJet_JecSF[0]
        jet0_JecSF_up = SelTightJet_JecSFup[0]
        jet0_JecSF_down = SelTightJet_JecSFdown[0]
        jet0_JerSF = SelTightJet_JerSF[0]
        jet1_pt = SelTightJet_pt[1]
        jet1_eta = SelTightJet_eta[1]
        jet1_phi = SelTightJet_phi[1]
        jet1_csv = SelTightJet_pfCombinedInclusiveSecondaryVertexV2BJetTags[1]
        jet1_JecSF = SelTightJet_JecSF[1]
        jet1_JerSF = SelTightJet_JerSF[1]
        jet1_JecSF_up = SelTightJet_JecSFup[1]
        jet1_JecSF_down = SelTightJet_JecSFdown[1]
        jet2_pt = SelTightJet_pt[2]
        jet2_eta = SelTightJet_eta[2]
        jet2_phi = SelTightJet_phi[2]
        jet2_csv = SelTightJet_pfCombinedInclusiveSecondaryVertexV2BJetTags[2]
        jet2_JecSF = SelTightJet_JecSF[2]
        jet2_JerSF = SelTightJet_JerSF[2]
        jet3_pt = SelTightJet_pt[3]
        jet3_eta = SelTightJet_eta[3]
        jet3_phi = SelTightJet_phi[3]
        jet3_csv = SelTightJet_pfCombinedInclusiveSecondaryVertexV2BJetTags[3]
        jet3_JecSF = SelTightJet_JecSF[3]
        jet3_JerSF = SelTightJet_JerSF[3]
        if(data==False and electron==True):
            f = ROOT.TFile("SF/eleTrig_SF.root")
            h = f.Get("h_eleTrig_SF")
            X=-99
            Y=-99
            for i in range(1,h.GetXaxis().GetNbins()+2):
                if(SelLepton_pt[0]<h.GetXaxis().GetBinLowEdge(i)):
                    X=i-1
                    break
            if(SelLepton_pt[0]>=h.GetXaxis().GetBinLowEdge(h.GetXaxis().GetNbins()+1)):
                    X=(h.GetXaxis().GetNbins()+1)-1
            for i in range(1,h.GetYaxis().GetNbins()+2):
                if(SelLepton_SCeta[0]<h.GetYaxis().GetBinLowEdge(i)):
                    Y=i-1
                    break
            triggerSF = h.GetBinContent(X,Y)
        if(data==False and muon==True):
            f = ROOT.TFile("SF/SingleMuonTrigger_Z_RunCD_Reco76X_Feb15.root")
            h1 = f.Get("runD_IsoMu20_OR_IsoTkMu20_HLTv4p2_PtEtaBins/pt_abseta_ratio")
            X1=-99
            Y1=-99
            for i in range(1,h1.GetXaxis().GetNbins()+2):
                if(SelLepton_pt[0]<h1.GetXaxis().GetBinLowEdge(i)):
                    X1=i-1
                    break
            if(SelLepton_pt[0]>=h1.GetXaxis().GetBinLowEdge(h1.GetXaxis().GetNbins()+1)):
                    X1=(h1.GetXaxis().GetNbins()+1)-1
            for i in range(1,h1.GetYaxis().GetNbins()+2):
                if(math.fabs(SelLepton_SCeta[0])<h1.GetYaxis().GetBinLowEdge(i)):
                    Y1=i-1
                    break
            h2 = f.Get("runD_IsoMu20_OR_IsoTkMu20_HLTv4p3_PtEtaBins/pt_abseta_ratio")
            X2=-99
            Y2=-99
            for i in range(1,h2.GetXaxis().GetNbins()+2):
                if(SelLepton_pt[0]<h2.GetXaxis().GetBinLowEdge(i)):
                    X2=i-1
                    break
            if(SelLepton_pt[0]>=h2.GetXaxis().GetBinLowEdge(h2.GetXaxis().GetNbins()+1)):
                    X2=(h2.GetXaxis().GetNbins()+1)-1
            for i in range(1,h2.GetYaxis().GetNbins()+2):
                if(math.fabs(SelLepton_SCeta[0])<h2.GetYaxis().GetBinLowEdge(i)):
                    Y2=i-1
                    break
            triggerSF = (717.3*h1.GetBinContent(X1,Y1)+1808.5*h2.GetBinContent(X2,Y2))/(717.3+1808.5)
        if(data==False and electron==True):###### Electron ID and reconstruction SFs.
            f1 = ROOT.TFile("SF/ScaleFactor_GsfElectronToRECO_passingTrigWP80.txt.egamma_SF2D.root") # "Triggering electrons MVA-ID WPs. Scale factors for 76X", WP80 SF file
            f2 = ROOT.TFile("SF/eleRECO.txt.egamma_SF2D.root") # "Reconstruction efficiency (track finding using SC as probe). Scale factors for 76X"
            h1 = f1.Get("EGamma_SF2D")
            h2 = f2.Get("EGamma_SF2D")
            X1=-99
            X2=-99
            Y1=-99
            Y2=-99
            for i in range(1,h1.GetXaxis().GetNbins()+2):
                if(SelLepton_SCeta[0]<h1.GetXaxis().GetBinLowEdge(i)):
                    X1=i-1
                    break
            for i in range(1,h1.GetYaxis().GetNbins()+2):
                if(SelLepton_pt[0]<h1.GetYaxis().GetBinLowEdge(i)):
                    Y1=i-1
                    break
            if(SelLepton_pt[0]>=h1.GetYaxis().GetBinLowEdge(h1.GetYaxis().GetNbins()+1)):
                    Y1=(h1.GetYaxis().GetNbins()+1)-1
            for i in range(1,h2.GetXaxis().GetNbins()+2):
                if(SelLepton_SCeta[0]<h2.GetXaxis().GetBinLowEdge(i)):
                    X2=i-1
                    break
            for i in range(1,h2.GetYaxis().GetNbins()+2):
                if(SelLepton_pt[0]<h2.GetYaxis().GetBinLowEdge(i)):
                    Y2=i-1
                    break
            if(SelLepton_pt[0]>=h2.GetYaxis().GetBinLowEdge(h2.GetYaxis().GetNbins()+1)):
                    Y2=(h2.GetYaxis().GetNbins()+1)-1
            lepIDSF = h1.GetBinContent(X1,Y1)
            lepIsoSF = h2.GetBinContent(X2,Y2)
        if(data==False and muon==True): ###### Muon ID and Iso SFs.
            f1 = ROOT.TFile("SF/MuonID_Z_RunBCD_prompt80X_7p65.root") # From /afs/cern.ch/cms/Physics/muon/ReferenceEfficiencies/Run2016/25ns/proviSFs_7p65/
            f2 = ROOT.TFile("SF/MuonIso_Z_RunBCD_prompt80X_7p65.root") # From /afs/cern.ch/cms/Physics/muon/ReferenceEfficiencies/Run2016/25ns/proviSFs_7p65/
            h1 = f1.Get("MC_NUM_TightIDandIPCut_DEN_genTracks_PAR_pt_spliteta_bin1/abseta_pt_ratio")
            h2 = f2.Get("MC_NUM_TightRelIso_DEN_TightID_PAR_pt_spliteta_bin1/abseta_pt_ratio")
            X1=-99
            X2=-99
            Y1=-99
            Y2=-99
            for i in range(1,h1.GetXaxis().GetNbins()+2):
                if(math.fabs(SelLepton_SCeta[0])<h1.GetXaxis().GetBinLowEdge(i)):
                    X1=i-1
                    break
            for i in range(1,h1.GetYaxis().GetNbins()+2):
                if(SelLepton_pt[0]<h1.GetYaxis().GetBinLowEdge(i)):
                    Y1=i-1
                    break
            if(SelLepton_pt[0]>=h1.GetYaxis().GetBinLowEdge(h1.GetYaxis().GetNbins()+1)):
                    Y1=(h1.GetYaxis().GetNbins()+1)-1
            for i in range(1,h2.GetXaxis().GetNbins()+2):
                if(math.fabs(SelLepton_SCeta[0])<h2.GetXaxis().GetBinLowEdge(i)):
                    X2=i-1
                    break
            for i in range(1,h2.GetYaxis().GetNbins()+2):
                if(SelLepton_pt[0]<h2.GetYaxis().GetBinLowEdge(i)):
                    Y2=i-1
                    break
            if(SelLepton_pt[0]>=h2.GetYaxis().GetBinLowEdge(h2.GetYaxis().GetNbins()+1)):
                    Y2=(h2.GetYaxis().GetNbins()+1)-1
            lepIDSF = h1.GetBinContent(X1,Y1)
            lepIsoSF = h2.GetBinContent(X2,Y2)







    #=================== SELECT DILEPTON EVENT ======================
    DoubleLeptonEvent=False
    triggerForDL=False
    MuoMuo=False
    EleEle=False
    EleMuo=False
    jetsDL=False

    if(len(SelLepton_pt)==2 and math.fabs(SelLepton_id[0])==11 and math.fabs(SelLepton_id[1])==11 and tt.HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ==1):
        EleEle=True
    if(len(SelLepton_pt)==2 and math.fabs(SelLepton_id[0])==13 and math.fabs(SelLepton_id[1])==13 and (tt.HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL==1 or tt.HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL==1 or tt.HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ==1 or tt.HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ==1)):
        MuoMuo=True
    if(len(SelLepton_pt)==2 and math.fabs(SelLepton_id[0])==11 and math.fabs(SelLepton_id[1])==13 and (tt.HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL==1 or tt.HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL==1 or tt.HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ==1 or tt.HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ==1)):
        EleMuo=True
    if(len(SelLepton_pt)==2 and math.fabs(SelLepton_id[0])==13 and math.fabs(SelLepton_id[1])==11 and (tt.HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL==1 or tt.HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL==1 or tt.HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ==1 or tt.HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ==1)):
        EleMuo=True

    if((dataMU==True and MuoMuo==True) or (dataEL==True and EleEle==True) or (dataMUEL==True and EleMuo==True) or (data==False and (MuoMuo==True or EleEle==True or EleMuo==True))):
    #if((data==True and (MuoMuo==True or EleEle==True or EleMuo==True)) or (data==False and (MuoMuo==True or EleEle==True or EleMuo==True))):
        triggerForDL=True
    if(len(SelTightJet_pt)>=2 and SelTightJet_pt[0]>30 and SelTightJet_pt[1]>30 and nBCSVM_DL>=1):
        jetsDL=True
    if(triggerForDL and jetsDL==True and dilepSelection==True and METFilters==True):
        if(math.fabs(SelLepton_id[0])==11 and math.fabs(SelLepton_id[1])==11):
            is_ee = 1
        if(math.fabs(SelLepton_id[0])==13 and math.fabs(SelLepton_id[1])==13):
            is_mumu = 1
        if((math.fabs(SelLepton_id[0])==11 and math.fabs(SelLepton_id[1])==13) or (math.fabs(SelLepton_id[0])==13 and math.fabs(SelLepton_id[1])==11)):
            is_emu = 1
        DoubleLeptonEvent=True
        nBCSVM=nBCSVM_DL
        NumberOfJets=len(SelJet_pt)
        jet0_pt = SelJet_pt[0]
        jet0_eta = SelJet_eta[0]
        jet0_phi = SelJet_phi[0]
        jet0_JecSF = SelJet_JecSF[0]
        jet0_JecSF_up = SelJet_JecSFup[0]
        jet0_JecSF_down = SelJet_JecSFdown[0]
        jet0_JerSF = SelJet_JerSF[0]
        jet0_csv = SelJet_pfCombinedInclusiveSecondaryVertexV2BJetTags[0]
        jet1_pt = SelJet_pt[1]
        jet1_eta = SelJet_eta[1]
        jet1_phi = SelJet_phi[1]
        jet1_csv = SelJet_pfCombinedInclusiveSecondaryVertexV2BJetTags[1]
        jet1_JecSF = SelJet_JecSF[1]
        jet1_JerSF = SelJet_JerSF[1]
        jet1_JecSF_up = SelJet_JecSFup[1]
        jet1_JecSF_down = SelJet_JecSFdown[1]
        if(len(SelJet_pt)>2):
            jet2_pt = SelJet_pt[2]
            jet2_eta = SelJet_eta[2]
            jet2_phi = SelJet_phi[2]
            jet2_csv = SelJet_pfCombinedInclusiveSecondaryVertexV2BJetTags[2]
            jet2_JecSF = SelJet_JecSF[2]
            jet2_JerSF = SelJet_JerSF[2]
        if(len(SelJet_pt)>3):
            jet3_pt = SelJet_pt[3]
            jet3_eta = SelJet_eta[3]
            jet3_phi = SelJet_phi[3]
            jet3_csv = SelJet_pfCombinedInclusiveSecondaryVertexV2BJetTags[3]
            jet3_JecSF = SelJet_JecSF[3]
            jet3_JerSF = SelJet_JerSF[3]
        if(data==False and MuoMuo==True and math.fabs(lep0_SCeta)<=1.2 and math.fabs(lep1_SCeta)<=1.2):
            triggerSF = -1.930048
        if(data==False and MuoMuo==True and math.fabs(lep0_SCeta)<=1.2 and math.fabs(lep1_SCeta)> 1.2):
            triggerSF = 0.933031
        if(data==False and MuoMuo==True and math.fabs(lep0_SCeta)> 1.2 and math.fabs(lep1_SCeta)<=1.2):
            triggerSF = 0.951115
        if(data==False and MuoMuo==True and math.fabs(lep0_SCeta)> 1.2 and math.fabs(lep1_SCeta)> 1.2):
            triggerSF = 0.933961
        if(data==False and EleEle==True and math.fabs(lep0_SCeta)<=1.2 and math.fabs(lep1_SCeta)<=1.2):
            triggerSF = 0.957835
        if(data==False and EleEle==True and math.fabs(lep0_SCeta)<=1.2 and math.fabs(lep1_SCeta)> 1.2):
            triggerSF = 0.949873
        if(data==False and EleEle==True and math.fabs(lep0_SCeta)> 1.2 and math.fabs(lep1_SCeta)<=1.2):
            triggerSF = 0.979874
        if(data==False and EleEle==True and math.fabs(lep0_SCeta)> 1.2 and math.fabs(lep1_SCeta)> 1.2):
            triggerSF = 1.002550
        if(data==False and EleMuo==True and ((math.fabs(SelLepton_id[0])==11 and math.fabs(SelLepton_SCeta[0])<=1.2 and math.fabs(SelLepton_SCeta[1])<=1.2) or (math.fabs(SelLepton_id[1])==11 and math.fabs(SelLepton_SCeta[1])<=1.2 and math.fabs(SelLepton_SCeta[0])<=1.2))):
            triggerSF = 0.959893
        if(data==False and EleMuo==True and ((math.fabs(SelLepton_id[0])==11 and math.fabs(SelLepton_SCeta[0])<=1.2 and math.fabs(SelLepton_SCeta[1])> 1.2) or (math.fabs(SelLepton_id[1])==11 and math.fabs(SelLepton_SCeta[1])<=1.2 and math.fabs(SelLepton_SCeta[0])> 1.2))):
            triggerSF = 0.971933
        if(data==False and EleMuo==True and ((math.fabs(SelLepton_id[0])==11 and math.fabs(SelLepton_SCeta[0])> 1.2 and math.fabs(SelLepton_SCeta[1])<=1.2) or (math.fabs(SelLepton_id[1])==11 and math.fabs(SelLepton_SCeta[1])> 1.2 and math.fabs(SelLepton_SCeta[0])<=1.2))):
            triggerSF = 0.978487
        if(data==False and EleMuo==True and ((math.fabs(SelLepton_id[0])==11 and math.fabs(SelLepton_SCeta[0])> 1.2 and math.fabs(SelLepton_SCeta[1])> 1.2) or (math.fabs(SelLepton_id[1])==11 and math.fabs(SelLepton_SCeta[1])> 1.2 and math.fabs(SelLepton_SCeta[0])> 1.2))):
            triggerSF = 0.988945
        if(data==False and math.fabs(SelLepton_id[0])==11):
            f1 = ROOT.TFile("SF/ScaleFactor_GsfElectronToRECO_passingTrigWP80.txt.egamma_SF2D.root")
            f2 = ROOT.TFile("SF/eleRECO.txt.egamma_SF2D.root")
            h1 = f1.Get("EGamma_SF2D")
            h2 = f2.Get("EGamma_SF2D")
            X1=-99
            X2=-99
            Y1=-99
            Y2=-99
            for i in range(1,h1.GetXaxis().GetNbins()+2):
                if(SelLepton_SCeta[0]<h1.GetXaxis().GetBinLowEdge(i)):
                    X1=i-1
                    break
            for i in range(1,h1.GetYaxis().GetNbins()+2):
                if(SelLepton_pt[0]<h1.GetYaxis().GetBinLowEdge(i)):
                    Y1=i-1
                    break
            if(SelLepton_pt[0]>=h1.GetYaxis().GetBinLowEdge(h1.GetYaxis().GetNbins()+1)):
                    Y1=(h1.GetYaxis().GetNbins()+1)-1
            for i in range(1,h2.GetXaxis().GetNbins()+2):
                if(SelLepton_SCeta[0]<h2.GetXaxis().GetBinLowEdge(i)):
                    X2=i-1
                    break
            for i in range(1,h2.GetYaxis().GetNbins()+2):
                if(SelLepton_pt[0]<h2.GetYaxis().GetBinLowEdge(i)):
                    Y2=i-1
                    break
            if(SelLepton_pt[0]>=h2.GetYaxis().GetBinLowEdge(h2.GetYaxis().GetNbins()+1)):
                    Y2=(h2.GetYaxis().GetNbins()+1)-1
            lep1IDSF = h1.GetBinContent(X1,Y1)
            lep1IsoSF = h2.GetBinContent(X2,Y2)
        if(data==False and math.fabs(SelLepton_id[1])==11):
            f1 = ROOT.TFile("SF/ScaleFactor_GsfElectronToRECO_passingTrigWP80.txt.egamma_SF2D.root")
            f2 = ROOT.TFile("SF/eleRECO.txt.egamma_SF2D.root")
            h1 = f1.Get("EGamma_SF2D")
            h2 = f2.Get("EGamma_SF2D")
            X1=-99
            X2=-99
            Y1=-99
            Y2=-99
            for i in range(1,h1.GetXaxis().GetNbins()+2):
                if(SelLepton_SCeta[1]<h1.GetXaxis().GetBinLowEdge(i)):
                    X1=i-1
                    break
            for i in range(1,h1.GetYaxis().GetNbins()+2):
                if(SelLepton_pt[1]<h1.GetYaxis().GetBinLowEdge(i)):
                    Y1=i-1
                    break
            if(SelLepton_pt[1]>=h1.GetYaxis().GetBinLowEdge(h1.GetYaxis().GetNbins()+1)):
                    Y1=(h1.GetYaxis().GetNbins()+1)-1
            for i in range(1,h2.GetXaxis().GetNbins()+2):
                if(SelLepton_SCeta[1]<h2.GetXaxis().GetBinLowEdge(i)):
                    X2=i-1
                    break
            for i in range(1,h2.GetYaxis().GetNbins()+2):
                if(SelLepton_pt[1]<h2.GetYaxis().GetBinLowEdge(i)):
                    Y2=i-1
                    break
            if(SelLepton_pt[1]>=h2.GetYaxis().GetBinLowEdge(h2.GetYaxis().GetNbins()+1)):
                    Y2=(h2.GetYaxis().GetNbins()+1)-1
            lep2IDSF = h1.GetBinContent(X1,Y1)
            lep2IsoSF = h2.GetBinContent(X2,Y2)
        if(data==False and math.fabs(SelLepton_id[0])==13):
            f1 = ROOT.TFile("SF/MuonID_Z_RunBCD_prompt80X_7p65.root")
            f2 = ROOT.TFile("SF/MuonIso_Z_RunBCD_prompt80X_7p65.root")
            h1 = f1.Get("MC_NUM_TightIDandIPCut_DEN_genTracks_PAR_pt_spliteta_bin1/abseta_pt_ratio")
            h2 = f2.Get("MC_NUM_TightRelIso_DEN_TightID_PAR_pt_spliteta_bin1/abseta_pt_ratio")
            X1=-99
            X2=-99
            Y1=-99
            Y2=-99
            for i in range(1,h1.GetXaxis().GetNbins()+2):
                if(math.fabs(SelLepton_SCeta[0])<h1.GetXaxis().GetBinLowEdge(i)):
                    X1=i-1
                    break
            for i in range(1,h1.GetYaxis().GetNbins()+2):
                if(SelLepton_pt[0]<h1.GetYaxis().GetBinLowEdge(i)):
                    Y1=i-1
                    break
            if(SelLepton_pt[0]>=h1.GetYaxis().GetBinLowEdge(h1.GetYaxis().GetNbins()+1)):
                    Y1=(h1.GetYaxis().GetNbins()+1)-1
            if(SelLepton_pt[0]<20):
                    Y1=1
            for i in range(1,h2.GetXaxis().GetNbins()+2):
                if(math.fabs(SelLepton_SCeta[0])<h2.GetXaxis().GetBinLowEdge(i)):
                    X2=i-1
                    break
            for i in range(1,h2.GetYaxis().GetNbins()+2):
                if(SelLepton_pt[0]<h2.GetYaxis().GetBinLowEdge(i)):
                    Y2=i-1
                    break
            if(SelLepton_pt[0]>=h2.GetYaxis().GetBinLowEdge(h2.GetYaxis().GetNbins()+1)):
                    Y2=(h2.GetYaxis().GetNbins()+1)-1
            if(SelLepton_pt[0]<20):
                    Y2=1
            lep1IDSF = h1.GetBinContent(X1,Y1)
            lep1IsoSF = h2.GetBinContent(X2,Y2)
        if(data==False and math.fabs(SelLepton_id[1])==13):
            f1 = ROOT.TFile("SF/MuonID_Z_RunBCD_prompt80X_7p65.root")
            f2 = ROOT.TFile("SF/MuonIso_Z_RunBCD_prompt80X_7p65.root")
            h1 = f1.Get("MC_NUM_TightIDandIPCut_DEN_genTracks_PAR_pt_spliteta_bin1/abseta_pt_ratio")
            h2 = f2.Get("MC_NUM_TightRelIso_DEN_TightID_PAR_pt_spliteta_bin1/abseta_pt_ratio")
            X1=-99
            X2=-99
            Y1=-99
            Y2=-99
            for i in range(1,h1.GetXaxis().GetNbins()+2):
                if(math.fabs(SelLepton_SCeta[1])<h1.GetXaxis().GetBinLowEdge(i)):
                    X1=i-1
                    break
            for i in range(1,h1.GetYaxis().GetNbins()+2):
                if(SelLepton_pt[1]<h1.GetYaxis().GetBinLowEdge(i)):
                    Y1=i-1
                    break
            if(SelLepton_pt[1]>=h1.GetYaxis().GetBinLowEdge(h1.GetYaxis().GetNbins()+1)):
                    Y1=(h1.GetYaxis().GetNbins()+1)-1
            if(SelLepton_pt[1]<20):
                    Y1=1
            for i in range(1,h2.GetXaxis().GetNbins()+2):
                if(math.fabs(SelLepton_SCeta[1])<h2.GetXaxis().GetBinLowEdge(i)):
                    X2=i-1
                    break
            for i in range(1,h2.GetYaxis().GetNbins()+2):
                if(SelLepton_pt[1]<h2.GetYaxis().GetBinLowEdge(i)):
                    Y2=i-1
                    break
            if(SelLepton_pt[1]>=h2.GetYaxis().GetBinLowEdge(h2.GetYaxis().GetNbins()+1)):
                    Y2=(h2.GetYaxis().GetNbins()+1)-1
            if(SelLepton_pt[1]<20):
                    Y2=1
            lep2IDSF = h1.GetBinContent(X1,Y1)
            lep2IsoSF = h2.GetBinContent(X2,Y2)
        lepIDSF =lep1IDSF*lep2IDSF
        lepIsoSF=lep1IsoSF*lep2IsoSF



    #if(tt.EVENT_filterBadGlobalMuonTagger==True and is_mu==1):
    '''if(len(SelLepton_pt)>=1):
        print '============= MET Filters ============='
        print 'Flag_goodVertices = ', tt.Flag_goodVertices
        print 'Flag_globalTightHalo2016Filter = ', tt.Flag_globalTightHalo2016Filter
        print 'Flag_HBHENoiseFilter = ', tt.Flag_HBHENoiseFilter
        print 'Flag_HBHENoiseIsoFilter = ', tt.Flag_HBHENoiseIsoFilter
        print '(NOT SUGGESTED for MC) Flag_eeBadScFilter = ', tt.Flag_eeBadScFilter
        print 'Flag_EcalDeadCellTriggerPrimitiveFilter = ', tt.Flag_EcalDeadCellTriggerPrimitiveFilter
        print 'EVENT_filterBadGlobalMuonTagger = ', tt.EVENT_filterBadGlobalMuonTagger
        print 'EVENT_filtercloneGlobalMuonTagger = ', tt.EVENT_filtercloneGlobalMuonTagger
        print '======================================='
        print "# Leptons", len(SelLepton_pt)
        print '# tight jets = ', len(SelTightJet_pt)
        print '# loose jets = ', len(SelJet_pt)
        print 'nBCSVM_SL = ', nBCSVM_SL
        print 'nBCSVM_DL = ', nBCSVM_DL
        print '============= Triggers ============='
        print 'HLT_IsoMu24 = ', tt.HLT_IsoMu24
        print 'HLT_IsoTkMu24 = ', tt.HLT_IsoTkMu24
        print 'HLT_Ele27_eta2p1_WPTight_Gsf = ', tt.HLT_Ele27_eta2p1_WPTight_Gsf
        print 'HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL = ', tt.HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL
        print 'HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL = ', tt.HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL
        print 'HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ = ', tt.HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ
        print 'HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ = ', tt.HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ
        print 'HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL = ', tt.HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL
        print 'HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL = ', tt.HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL
        print 'HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ = ', tt.HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ
        print 'HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ = ', tt.HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ
        print 'HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL = ', tt.HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL
        print 'HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL = ', tt.HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL
        print 'HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ = ', tt.HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ
        print 'HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ = ', tt.HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ
        print '============= Selection Flags ============='
        print 'is electron = ', electron
        print 'is muon = ', muon
        print 'is jets SL = ', jets
        print 'is jets DL = ', jetsDL
        print 'MET filters = ', METFilters
        print 'triggerForDL = ', triggerForDL
        print 'dilepSelection = ' , dilepSelection
        print 'EleEle = ', EleEle
        print 'EleMuo = ', EleMuo
        print 'MuoMuo = ', MuoMuo

        print "SingleLeptonEvent = " , SingleLeptonEvent
        print "DoubleLeptonEvent = " , DoubleLeptonEvent
        print "========================== Next event ==================================="'''

    ttHFCategory = -99
    if(sample!=0):
        ttHFCategory =(tt.ttHFCategory)
    #if(SingleLeptonEvent or DoubleLeptonEvent):
        jet_0_pt[0] = jet0_pt
        arr = [int(tt.EVENT_run), int(tt.EVENT_lumiBlock), int(tt.EVENT_event),
               #int(is_e), int(is_mu), int(is_ee), int(is_emu), int(is_mumu),
               int(tt.is_e), int(tt.is_mu), int(is_ee), int(is_emu), int(is_mumu),
               int(NumberOfJets), int(nBCSVM),
               lep0_pt, lep0_iso, lep0_id,
               lep1_pt, lep1_iso, lep1_id,
               jet0_pt, jet0_eta, jet0_phi, jet0_JecSF, jet0_JecSF_up, jet0_JecSF_down, jet0_csv,
               jet1_pt, jet1_eta, jet1_phi, jet1_JecSF, jet1_JecSF_up, jet1_JecSF_down, jet1_csv,
               MET, MET_phi,
               mll,
               int(ttHFCategory),
               int(tt.trueInteractions),#n_interactions = true # primary vertices. PileupSummaryInfo->getTrueNumInteractions()
               float(tt.PUWeight),
               float(tt.bWeight),
               float(tt.bWeightLFup),
               float(tt.bWeightHFdown),
               float(tt.bWeightCErr1down),
               triggerSF,
               lepIDSF,
               lepIsoSF,
               pdf_up, pdf_down,
               Q2_up,Q2_down,
               float(tt.BDT),
               int(-1),# MEM placeholder
               int(-1),# dnn ttH output placeholder
               int(-1),# dnn ttbb output placeholder
               #int(tt.BJetness_numleps),
               #float(tt.BJetness_avip3dval),
               #float(tt.BJetness_avip3dsig),
               #float(tt.BJetness_avsip3dsig),
               #float(tt.BJetness_avip1dsig)
               ]

        s = ""
        for i in range(len(arr)):
            if not isinstance(arr[i], int):
                s += str(round(arr[i], 4)) + ","
            else:
                s += str(arr[i]) + ","
        #print 'Fill event in .csv file: '
        #print arr
        csv_writer.writerow(arr)
        out_tt.Fill()
        #print s[:-1]
out_tf.Write()
out_tf.Close()
print "Output written to: ", csvnom
print "FIN!"
