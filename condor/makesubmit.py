import sys
import os
import glob
import string
#####
##   Parameters to be specified by the user
#####
# Output Dir Structure:
#   /publicfs/cms/data/TopQuark/ttHbb/JTW/2017_07_v1/ttHACIHEP/output/MC/<Process/DataBlock>/<subfile.root>



#analysis and task
analysis = "ttHbb"
taskname = "SL"
#for the queue
condor_dir_path    = "/publicfs/cms/user/joshuha/ttHACIHEP/condor/"
jobDir      = condor_dir_path+"/"+"Jobs"# Replaced/created on the fly
AnalyzerDir = condor_dir_path+"/"+"Analyzer"# Replaced/created on the fly
task        = analysis+"_"+taskname
analysis_dir_path = "/publicfs/cms/user/joshuha/ttHACIHEP/"
rootplizer = "SecondStep"


# Python dictionaries map directory of input files to
sample={
#"ttHbb":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/ttHTobb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/FullMorV2_ttHbb/170531_195706/0000/',
"ttHbb":'/publicfs/cms/data/TopQuark/ttHbb/JTW/crab_Moriond17/mc/ttHTobb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/TTHbb_MC_ttHbb_4/170621_142513/0000/',
#"ttH_nonbb":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/ttHToNonbb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/FullMorV2_ttHnobb/170530_161519/0000/',
#"ttjets_incl":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/FullMorV2_TT/170531_195914/0000/',
"ttjets_incl":'/publicfs/cms/data/TopQuark/ttHbb/JTW/crab_Moriond17/mc/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/TTHbb_MC_ttjets_4/170621_142726/0000/',
#"ttjets_DL":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/TTTo2L2Nu_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/FullMorV2_TTTo2L2Nu/170531_200535/0000/',
#"ttjets_SL":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/TTToSemilepton_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/FullMorV2_TTToSemilepton/170531_200121/0000/',
#"ttjets_bfilter":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/TTToSemilepton_ttbbFilter_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/FullMorV2_TTToSemilepton_ttbbFilter/170531_200331/0000/',
#"stop_schan":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/ST_s-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1/FullMorV2_amcSTs/170531_194132/0000/',
#"stop_tchan_top":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/ST_t-channel_top_4f_inclusiveDecays_TuneCUETP8M2T4_13TeV-powhegV2-madspin/FullMorV2_STt/170531_200742/0000/',
#"stop_tchan_antitop":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/ST_t-channel_antitop_4f_inclusiveDecays_TuneCUETP8M2T4_13TeV-powhegV2-madspin/FullMorV2_SaTt/170531_200952/0000/',
#"stop_tW_top":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M2T4/FullMorV2_ST/170531_201156/0000/',
#"stop_tW_antitop":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M2T4/FullMorV2_SaT/170531_201405/0000/',
#"ttW_qq":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/TTWJetsToQQ_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/FullMorV2_amcTTWJetsToQQ/170531_204858/0000/',
#"ttW_SL_ext1_a":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/FullMorV2_amcTTWJetsToLNuext2/170531_171903/0000/',
#"ttW_SL_ext1_b":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/FullMorV2_amcTTWJetsToLNuext2/170531_182459/0000/',
#"ttW_SL_ext2":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/FullMorV2_amcTTWJetsToLNuext1/170531_182712/0000/',
#"ttZ_qq":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/TTZToQQ_TuneCUETP8M1_13TeV-amcatnlo-pythia8/FullMorV2_amcTTZToQQ/170531_205113/0000/',
#"ttZ_DL_a":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8/FullMorV2_amcTTZToLLNuNu_M-10_ext1/170531_172318/0000/',
#"ttZ_DL_b":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8/FullMorV2_amcTTZToLLNuNu_M-10_ext1/170531_182920/0000/',
#"ZZ":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/ZZ_TuneCUETP8M1_13TeV-pythia8/FullMorV2_ZZ/170531_204447/0000/',
#"ZZ_ext":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/ZZ_TuneCUETP8M1_13TeV-pythia8/FullMorV2_ZZext1/170531_204652/0000/',
#"WZ":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/WZ_TuneCUETP8M1_13TeV-pythia8/FullMorV2_WZ/170531_204032/0000/',
#"WZ_ext":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/WZ_TuneCUETP8M1_13TeV-pythia8/FullMorV2_WZext1/170531_204237/0000/',
#"WW":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/WW_TuneCUETP8M1_13TeV-pythia8/FullMorV2_WW/170531_201612/0000/',
#"WW_ext":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/WW_TuneCUETP8M1_13TeV-pythia8/FullMorV2_WWext1/170531_201822/0000/',
#"WJetsToLNu_HT-70To100":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/WJetsToLNu_HT-70To100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/FullMorV2_WJetsToLNu_HT-70To100/170531_205737/0000/',
#"WJetsToLNu_HT-100To200":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/FullMorV2_WJetsToLNu_HT-100To200/170531_205941/0000/',
#"WJetsToLNu_HT-100To200_ext1":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/FullMorV2_WJetsToLNu_HT-100To200_ext1/170531_210151/0000/',
#"WJetsToLNu_HT-100To200_ext2":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/FullMorV2_WJetsToLNu_HT-100To200_ext2/170531_210357/0000/',
#"WJetsToLNu_HT-200To400":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/FullMorV2_WJetsToLNu_HT-200To400/170531_211103/0000/',
#"WJetsToLNu_HT-200To400_ext1":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/FullMorV2_WJetsToLNu_HT-200To400_ext1/170531_211309/0000/',
#"WJetsToLNu_HT-200To400_ext2":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/FullMorV2_WJetsToLNu_HT-200To400_ext2/170531_211512/0000/',
#"WJetsToLNu_HT-400To600":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/FullMorV2_WJetsToLNu_HT-400To600/170531_211716/0000/',
#"WJetsToLNu_HT-400To600_ext1":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/FullMorV2_WJetsToLNu_HT-400To600_ext1/170531_211917/0000/',
#"WJetsToLNu_HT-600To800":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/WJetsToLNu_HT-600To800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/FullMorV2_WJetsToLNu_HT-600To800/170531_212122/0000/',
#"WJetsToLNu_HT-600To800_ext1":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/WJetsToLNu_HT-600To800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/FullMorV2_WJetsToLNu_HT-600To800_ext1/170531_212324/0000/',
#"WJetsToLNu_HT-800To1200":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/WJetsToLNu_HT-800To1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/FullMorV2_WJetsToLNu_HT-800To1200/170531_212524/0000/',
#"WJetsToLNu_HT-800To1200_ext1":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/WJetsToLNu_HT-800To1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/FullMorV2_WJetsToLNu_HT-800To1200_ext1/170531_212726/0000/',
#"WJetsToLNu_HT-1200To2500":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/WJetsToLNu_HT-1200To2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/FullMorV2_WJetsToLNu_HT-1200To2500/170531_212927/0000/',
#"WJetsToLNu_HT-1200To2500_ext1":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/WJetsToLNu_HT-1200To2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/FullMorV2_WJetsToLNu_HT-1200To2500_ext1/170531_213128/0000/',
#"WJetsToLNu_HT-2500ToInf":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/WJetsToLNu_HT-2500ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/FullMorV2_WJetsToLNu_HT-2500ToInf/170531_213327/0000/',
#"WJetsToLNu_HT-2500ToInf_ext1":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/WJetsToLNu_HT-2500ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/FullMorV2_WJetsToLNu_HT-2500ToInf_ext1/170531_214059/0000/',
#"DY_M-10to50_HT":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/FullMorV2_DY_M10to50/170531_194340/0000/',
#"DY_M-50_HT_ext1":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/FullMorV2_DY_M50_ext1-v2/170531_194549/0000/',
#"DY_M-50_HT_ext2":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/FullMorV2_DY_M-50_ext2/170601_094234/0000/',
#"DY_M-5to50_HT-100to200":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/DYJetsToLL_M-5to50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/FullMorV2_DY_M-5to50_HT-100to200/170531_214300/0000/',
#"DY_M-5to50_HT-100to200_ext1":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/DYJetsToLL_M-5to50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/FullMorV2_DY_M-5to50_HT-100to200_ext1/170531_214508/0000/',
#"DY_M-5to50_HT-200to400":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/DYJetsToLL_M-5to50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/FullMorV2_DY_M-5to50_HT-200to400/170531_214711/0000/',
#"DY_M-5to50_HT-200to400_ext1":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/DYJetsToLL_M-5to50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/FullMorV2_DY_M-5to50_HT-200to400_ext1/170531_214914/0000/',
#"DY_M-5to50_HT-400to600":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/DYJetsToLL_M-5to50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/FullMorV2_DY_M-5to50_HT-400to600/170531_215117/0000/',
#"DY_M-5to50_HT-400to600_ext1":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/DYJetsToLL_M-5to50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/FullMorV2_DY_M-5to50_HT-400to600_ext1/170531_215319/0000/',
#"DY_M-5to50_HT-600toInf":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/DYJetsToLL_M-5to50_HT-600toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/FullMorV2_DY_M-5to50_HT-600toInf/170531_215521/0000/',
#"DY_M-5to50_HT-600toInf_ext1":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/DYJetsToLL_M-5to50_HT-600toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/FullMorV2_DY_M-5to50_HT-600toInf_ext1/170531_215720/0000/',
#"DYJetsToLL_M-5to50_HT-70to100":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/DYJetsToLL_M-5to50_HT-70to100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/FullMorV2_DY_M-5to50_HT-70to100/170601_093345/',
#"DY_M-50_HT-70to100":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/DYJetsToLL_M-50_HT-70to100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/FullMorV2_DY_M-50_HT-70to100/170531_215923/0000/',
#"DY_M-50_HT-100to200":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/DYJetsToLL_M-50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/FullMorV2_DY_M-50_HT-100to200/170531_220128/0000/',
#"DY_M-50_HT-100to200_ext1":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/DYJetsToLL_M-50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/FullMorV2_DY_M-50_HT-100to200_ext1/170531_220333/0000/',
#"DY_M-50_HT-200to400":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/FullMorV2_DY_M-50_HT-200to400/170531_221530/0000/',
#"DY_M-50_HT-200to400_ext1":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/FullMorV2_DY_M-50_HT-200to400_ext1/170531_221733/0000/',
#"DY_M-50_HT-400to600":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/DYJetsToLL_M-50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/FullMorV2_DY_M-50_HT-400to600/170531_221937/0000/',
#"DY_M-50_HT-400to600_ext1":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/DYJetsToLL_M-50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/FullMorV2_DY_M-50_HT-400to600_ext1/170531_222148/0000/',
#"DY_M-50_HT-600to800_v2":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/DYJetsToLL_M-50_HT-600to800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/FullMorV2_DY_M-50_HT-600to800_v2/170531_222354/0000/',
#"DY_M-50_HT-800to1200":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/DYJetsToLL_M-50_HT-800to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/FullMorV2_DY_M-50_HT-800to1200/170531_222555/0000/',
#"DY_M-50_HT-1200to2500":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/DYJetsToLL_M-50_HT-1200to2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/FullMorV2_DY_M-50_HT-1200to2500/170531_222759/0000/',
#"DY_M-50_HT-2500toInf":'/publicfs/cms/data/TopQuark/cms13TeV/FullMorV2/mc/DYJetsToLL_M-50_HT-2500toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/FullMorV2_DY_M-50_HT-2500toInf/170531_223005/0000/',
#"data_SEleBlockB1":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/data/SingleElectron/crab_FullMorV1_SEleBlockB1/170216_125915/0000/',
#"data_SEleBlockC1":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/data/SingleElectron/crab_FullMorV1_SEleBlockC1/170216_130855/0000/',
#"data_SEleBlockD1":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/data/SingleElectron/crab_FullMorV1_SEleBlockD1/170216_131209/0000/',
#"data_SEleBlockE1":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/data/SingleElectron/crab_FullMorV1_SEleBlockE1/170216_131445/0000/',
#"data_SEleBlockF1":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/data/SingleElectron/crab_FullMorV1_SEleBlockF1/170216_131713/0000/',
#"data_SEleBlockF2":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/data/SingleElectron/crab_FullMorV1_SEleBlockF2/170216_132117/0000/',
#"data_SEleBlockG1":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/data/SingleElectron/crab_FullMorV1_SEleBlockG1/170216_132450/0000/',
#"data_SEleBlockH1":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/data/SingleElectron/crab_FullMorV1_SEleBlockH1/170216_132719/0000/',
#"data_SEleBlockH2":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/data/SingleElectron/crab_FullMorV1_SEleBlockH2/170217_142948/0000/',
#"data_SMuBlockB1":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/data/SingleMuon/crab_FullMorV1_SMuBlockB1/170216_133716/0000/',
#"data_SMuBlockC1":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/data/SingleMuon/crab_FullMorV1_SMuBlockC1/170217_102735/0000/',
#"data_SMuBlockD1":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/data/SingleMuon/crab_FullMorV1_SMuBlockD1/170216_134614/0000/',
#"data_SMuBlockE1":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/data/SingleMuon/crab_FullMorV1_SMuBlockE1/170217_081050/0000/',
#"data_SMuBlockF1":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/data/SingleMuon/crab_FullMorV1_SMuBlockF1/170217_081305/0000/',
#"data_SMuBlockF2":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/data/SingleMuon/crab_FullMorV1_SMuBlockF2/170216_135711/0000/',
#"data_SMuBlockG1":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/data/SingleMuon/crab_FullMorV1_SMuBlockG1/170216_140118/0000/',
#"data_SMuBlockH1":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/data/SingleMuon/crab_FullMorV1_SMuBlockH1/170216_140404/0000/',
#"data_SMuBlockH2":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/data/SingleMuon/crab_FullMorV1_SMuBlockH2/170216_140830/0000/'
"data_SEleBlockB1_a":'/publicfs/cms/data/TopQuark/ttHbb/JTW/crab_Moriond17/data/SingleElectron/crab_TTHbb_SEleBlockB1/170621_143500/0000/',
"data_SEleBlockB1_b":'/publicfs/cms/data/TopQuark/ttHbb/JTW/crab_Moriond17/data/SingleElectron/crab_TTHbb_SEleBlockB1/170621_143500/0001/',
"data_SEleBlockC1":'/publicfs/cms/data/TopQuark/ttHbb/JTW/crab_Moriond17/data/SingleElectron/crab_TTHbb_SEleBlockC1/170621_143729/0000/',
"data_SEleBlockD1":'/publicfs/cms/data/TopQuark/ttHbb/JTW/crab_Moriond17/data/SingleElectron/crab_TTHbb_SEleBlockD1/170621_143954/0000/',
"data_SEleBlockE1":'/publicfs/cms/data/TopQuark/ttHbb/JTW/crab_Moriond17/data/SingleElectron/crab_TTHbb_SEleBlockE1/170621_144216/0000/',
"data_SEleBlockF1":'/publicfs/cms/data/TopQuark/ttHbb/JTW/crab_Moriond17/data/SingleElectron/crab_TTHbb_SEleBlockF1/170621_144427/0000/',
"data_SEleBlockF2":'/publicfs/cms/data/TopQuark/ttHbb/JTW/crab_Moriond17/data/SingleElectron/crab_TTHbb_SEleBlockF2/170621_144635/0000/',
"data_SEleBlockG1_a":'/publicfs/cms/data/TopQuark/ttHbb/JTW/crab_Moriond17/data/SingleElectron/crab_TTHbb_SEleBlockG1/170621_144846/0000/',
"data_SEleBlockG1_b":'/publicfs/cms/data/TopQuark/ttHbb/JTW/crab_Moriond17/data/SingleElectron/crab_TTHbb_SEleBlockG1/170621_144846/0001/',
"data_SEleBlockH1_a":'/publicfs/cms/data/TopQuark/ttHbb/JTW/crab_Moriond17/data/SingleElectron/crab_TTHbb_SEleBlockH1/170621_145056/0000/',
"data_SEleBlockH1_b":'/publicfs/cms/data/TopQuark/ttHbb/JTW/crab_Moriond17/data/SingleElectron/crab_TTHbb_SEleBlockH1/170621_145056/0001/',
"data_SEleBlockH2":'/publicfs/cms/data/TopQuark/ttHbb/JTW/crab_Moriond17/data/SingleElectron/crab_TTHbb_SEleBlockH2/170626_141046/0000/',
"data_SMuBlockB1_a":'/publicfs/cms/data/TopQuark/ttHbb/JTW/crab_Moriond17/data/SingleMuon/crab_TTHbb_SMuBlockB1/170621_145517/0000/',
"data_SMuBlockB1_b":'/publicfs/cms/data/TopQuark/ttHbb/JTW/crab_Moriond17/data/SingleMuon/crab_TTHbb_SMuBlockB1/170621_145517/0000/',
"data_SMuBlockC1":'/publicfs/cms/data/TopQuark/ttHbb/JTW/crab_Moriond17/data/SingleMuon/crab_TTHbb_SMuBlockC1/170621_145733/0000/',
"data_SMuBlockD1":'/publicfs/cms/data/TopQuark/ttHbb/JTW/crab_Moriond17/data/SingleMuon/crab_TTHbb_SMuBlockD1/170621_145947/0000/',
"data_SMuBlockE1":'/publicfs/cms/data/TopQuark/ttHbb/JTW/crab_Moriond17/data/SingleMuon/crab_TTHbb_SMuBlockE1/170621_150159/0000/',
"data_SMuBlockF1":'/publicfs/cms/data/TopQuark/ttHbb/JTW/crab_Moriond17/data/SingleMuon/crab_TTHbb_SMuBlockF1/170621_150407/0000/',
"data_SMuBlockF2":'/publicfs/cms/data/TopQuark/ttHbb/JTW/crab_Moriond17/data/SingleMuon/crab_TTHbb_SMuBlockF2/170621_150618/0000/',
"data_SMuBlockG1_a":'/publicfs/cms/data/TopQuark/ttHbb/JTW/crab_Moriond17/data/SingleMuon/crab_TTHbb_SMuBlockG1/170621_150829/0000/',
"data_SMuBlockG1_b":'/publicfs/cms/data/TopQuark/ttHbb/JTW/crab_Moriond17/data/SingleMuon/crab_TTHbb_SMuBlockG1/170621_150829/0001/',
"data_SMuBlockH1_a":'/publicfs/cms/data/TopQuark/ttHbb/JTW/crab_Moriond17/data/SingleMuon/crab_TTHbb_SMuBlockH1/170621_151043/0000/',
"data_SMuBlockH1_b":'/publicfs/cms/data/TopQuark/ttHbb/JTW/crab_Moriond17/data/SingleMuon/crab_TTHbb_SMuBlockH1/170621_151043/0001/',
"data_SMuBlockH2":'/publicfs/cms/data/TopQuark/ttHbb/JTW/crab_Moriond17/data/SingleMuon/crab_TTHbb_SMuBlockH2/170621_151255/0000/'
       }
sampleout={
"ttHbb": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_07_v1/ttHACIHEP/output/MC/',
#"ttH_nonbb": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
"ttjets_incl": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_07_v1/ttHACIHEP/output/MC/',
#"ttjets_DL": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"ttjets_SL": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"ttjets_bfilter": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"stop_schan": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"stop_tchan_top": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"stop_tchan_antitop": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"stop_tW_top": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"stop_tW_antitop": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"ttW_qq": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"ttW_SL_ext1": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"ttW_SL_ext2": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"ttZ_qq": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"ttZ_DL": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"ZZ": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"ZZ_ext": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"WZ": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"WZ_ext": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"WW": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
##"WW_ext": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"WJetsToLNu_HT-70To100":'/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"WJetsToLNu_HT-100To200":'/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"WJetsToLNu_HT-100To200_ext1":'/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"WJetsToLNu_HT-100To200_ext2":'/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"WJetsToLNu_HT-200To400":'/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"WJetsToLNu_HT-200To400_ext1":'/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"WJetsToLNu_HT-200To400_ext2":'/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"WJetsToLNu_HT-400To600":'/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"WJetsToLNu_HT-400To600_ext1":'/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"WJetsToLNu_HT-600To800":'/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"WJetsToLNu_HT-600To800_ext1":'/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"WJetsToLNu_HT-800To1200":'/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"WJetsToLNu_HT-800To1200_ext1":'/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"WJetsToLNu_HT-1200To2500":'/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"WJetsToLNu_HT-1200To2500_ext1":'/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"WJetsToLNu_HT-2500ToInf":'/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"WJetsToLNu_HT-2500ToInf_ext1":'/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"DY_M-10to50_HT":'/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"DY_M-50_HT_ext1":'/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"DY_M-50_HT_ext2":'/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"DY_M-5to50_HT-100to200":'/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"DY_M-5to50_HT-100to200_ext1":'/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"DY_M-5to50_HT-200to400":'/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"DY_M-5to50_HT-200to400_ext1":'/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"DY_M-5to50_HT-400to600":'/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"DY_M-5to50_HT-400to600_ext1":'/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"DY_M-5to50_HT-600toInf":'/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"DY_M-5to50_HT-600toInf_ext1":'/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"DYJetsToLL_M-5to50_HT-70to100":'/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"DY_M-50_HT-70to100":'/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"DY_M-50_HT-100to200":'/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"DY_M-50_HT-100to200_ext1":'/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"DY_M-50_HT-200to400":'/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"DY_M-50_HT-200to400_ext1":'/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"DY_M-50_HT-400to600":'/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"DY_M-50_HT-400to600_ext1":'/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"DY_M-50_HT-600to800_v2":'/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"DY_M-50_HT-800to1200":'/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"DY_M-50_HT-1200to2500":'/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
#"DY_M-50_HT-2500toInf":'/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_06/ttHACIHEP/output/MC/',
"data_SEleBlockB1_a": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_07_v1/ttHACIHEP/output/DATA/',
"data_SEleBlockB1_b": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_07_v1/ttHACIHEP/output/DATA/',
"data_SEleBlockC1": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_07_v1/ttHACIHEP/output/DATA/',
"data_SEleBlockD1": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_07_v1/ttHACIHEP/output/DATA/',
"data_SEleBlockE1": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_07_v1/ttHACIHEP/output/DATA/',
"data_SEleBlockF1": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_07_v1/ttHACIHEP/output/DATA/',
"data_SEleBlockF2": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_07_v1/ttHACIHEP/output/DATA/',
"data_SEleBlockG1_a": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_07_v1/ttHACIHEP/output/DATA/',
"data_SEleBlockG1_b": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_07_v1/ttHACIHEP/output/DATA/',
"data_SEleBlockH1_a": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_07_v1/ttHACIHEP/output/DATA/',
"data_SEleBlockH1_b": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_07_v1/ttHACIHEP/output/DATA/',
"data_SEleBlockH2": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_07_v1/ttHACIHEP/output/DATA/',
"data_SMuBlockB1_a": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_07_v1/ttHACIHEP/output/DATA/',
"data_SMuBlockB1_b": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_07_v1/ttHACIHEP/output/DATA/',
"data_SMuBlockC1": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_07_v1/ttHACIHEP/output/DATA/',
"data_SMuBlockD1": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_07_v1/ttHACIHEP/output/DATA/',
"data_SMuBlockE1": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_07_v1/ttHACIHEP/output/DATA/',
"data_SMuBlockF1": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_07_v1/ttHACIHEP/output/DATA/',
"data_SMuBlockF2": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_07_v1/ttHACIHEP/output/DATA/',
"data_SMuBlockG1_a": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_07_v1/ttHACIHEP/output/DATA/',
"data_SMuBlockG1_b": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_07_v1/ttHACIHEP/output/DATA/',
"data_SMuBlockH1_a": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_07_v1/ttHACIHEP/output/DATA/',
"data_SMuBlockH1_b": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_07_v1/ttHACIHEP/output/DATA/',
"data_SMuBlockH2": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_07_v1/ttHACIHEP/output/DATA/'
          }
#####
##   The script itself
#####
# Create output directory for output files and logs.
# 'popen' opens a pipe to or from a command.
# Return value is an open file object connected to the pipe,
# which can be read or written depending on whether mode is 'r' (default) or 'w'.
cshFilePath = jobDir+"/"+"sh"
logFilePath = jobDir+"/"+"log"
if os.path.exists(jobDir):
	os.popen('rm -fr '+jobDir)
if os.path.exists(AnalyzerDir):
        os.popen('rm -fr '+AnalyzerDir)
os.popen('mkdir -p '+cshFilePath)
os.popen('mkdir -p '+logFilePath)

# Compose bash script submit condor jobs via bash.
allJobFileName = "all.sh"
allJobFile      = file(allJobFileName,"w")
print >> allJobFile, "#!/bin/bash"
print >> allJobFile, "cd ",cshFilePath

# Performs hadd-ing of output files.
MergeFileName = "merge.sh"
MergeFile      = file(MergeFileName,"w")
MergeSourceFile = " "

# Prepares condor job submission file.
# All condor variables to be defined here.
# http://research.cs.wisc.edu/htcondor/manual/v7.6/condor_submit.html#man-condor-submit
def prepareSubmitJob(submitFileName,cshFileName, outPutFileName, errorFileName):
	cshFile      = file(submitFileName,"w")
	print >> cshFile, "Universe     = vanilla"
	print >> cshFile, "getenv       = true"
	print >> cshFile, "Executable   = ",cshFileName
	print >> cshFile, "Output       = ",outPutFileName
	print >> cshFile, "Error        = ",errorFileName
	print >> cshFile, "Queue"

# Prepares script used by condor to run analysis jobs.
# In this case we want it to run SecondStep pre-compiled code.
# This needs entirely redefining.
def prepareCshJob(input,output,submitFileName,analyzerpath):
    subFile = file(submitFileName,"w")
    print >> subFile, "#!/bin/bash"
    print >> subFile, "/bin/hostname"
    print >> subFile, "gcc -v"
    print >> subFile, "pwd"
	#print >> subFile, "setenv SCRAM_ARCH slc5_amd64_gcc462"
	#print >> subFile, "source /cvmfs/cms.cern.ch/cmsset_default.csh"
    print >> subFile, "source  /afs/ihep.ac.cn/soft/CMS/64bit/root/profile/rootenv-entry 6.08.02"
	#print >> subFile, "eval \`scramv1 runtime -sh\`"
    print >> subFile, "cd "+analyzerpath
	#print >> subFile, "cp ${jobDir}/getAhist.C ."
    print >> subFile, "./"+rootplizer+" "+input+" "+output


keys=sample.keys()

# Loop over all samples you want to run on.
for key,k in sample.iteritems():

    print 'Dict key = ' , key
    print 'Sample = ', k
    sampleName = key

    # Full input path + filename.
    rootDirectory   = k # +"/"+k    ## full input path to directory containing '.root' for subset of single individual samples.
    outputDirectory = sampleout[key]+"/"+key # path to output directory
    print 'output directory = ' , outputDirectory

    # Make directories used to run analysis and store output.
    AnalyzerSampleDir = AnalyzerDir + "/" + sampleName
    os.popen('mkdir -p ' + AnalyzerSampleDir)
    os.popen('mkdir -p ' + sampleout[key])
    os.popen('mkdir -p ' + outputDirectory)
    os.chdir(rootDirectory)

    # glob.glob() is a wrapper around a os.listdir()
    roots = glob.glob('*.root')

    # Sorted alphanumerically by name:
    roots.sort()
    nroot = len(roots)
    MergeSourceFile = ""
    for iroot in range(nroot):

        # Input filename (full-path):
        input  = rootDirectory+"/"+roots[iroot]

        # Use input files to name output (with edited suffix)
        # output = outputDirectory+"/"+roots[iroot].replace(".root","_rootplas")

        # Make temp directory in which the analysis is to be performed:
        analyzerpath = AnalyzerSampleDir+"/"+roots[iroot].replace(".root","")
        os.popen('mkdir -p '+analyzerpath)

        # Copy analysis code to analysis Directory
        analysis_code_path = analysis_dir_path+"/"+rootplizer
        command_cp_cc = 'cp '+ analysis_code_path + " " + analyzerpath
        os.system(command_cp_cc)

        MergeSourceFile += " "+roots[iroot].replace(".root","_rootplas")

        # Make the name of submitFile and InputFile matched
        name = roots[iroot].replace('.root','')
        submitFileName = sampleName+"_"+name+".submit"
        cshFileName = cshFilePath+"/"+sampleName+"_"+name+".sh"
        logFileName = logFilePath+"/"+sampleName+"_"+name+".log"
        errFileName = logFilePath+"/"+sampleName+"_"+name+".err"

        prepareSubmitJob(cshFilePath+"/"+submitFileName, cshFileName, logFileName, errFileName)
        prepareCshJob(input,outputDirectory,cshFileName,analyzerpath)
        print >> allJobFile, "condor_submit "+ submitFileName + " -group cms -name job@schedd01.ihep.ac.cn"


    print >> MergeFile, "cd ",outputDirectory
    print >> MergeFile, "hadd -f " + key + "_Merged_rootplas.root", " SS_OutTree*.root"

print >> allJobFile, "cd -"
