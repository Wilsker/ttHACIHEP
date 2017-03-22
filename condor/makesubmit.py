import sys
import os
import glob
import string
#####
##   Parameters to be specified by the user
#####
# Output Dir Structure:
#   /publicfs/cms/data/TopQuark/ttHbb/JTW/2017_03/ttHACIHEP/output/MC/<Process/DataBlock>/<subfile.root>



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
"ttHbb":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/mc/ttHTobb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/crab_FullMorV1_ttHbb/170216_130523/0000/',
"ttjets_DL":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/mc/TTTo2L2Nu_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/crab_FullMorV1_TTTo2L2Nu/170216_131744/0000/',
"ttjets_SL":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/mc/TTToSemilepton_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/crab_FullMorV1_TTToSemilepton/170216_131212/0000/',
"ttjets_bfilter":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/mc/TTToSemilepton_ttbbFilter_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/crab_FullMorV1_TTToSemilepton_ttbbFilter/170217_064724/0000/',
"ttWjets_qq":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/mc/TTWJetsToQQ_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/crab_FullMorV1_amcTTWJetsToQQ/170217_065640/0000/',
"ttZ_qq":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/mc/TTZToQQ_TuneCUETP8M1_13TeV-amcatnlo-pythia8/crab_FullMorV1_amcTTZToQQ/170216_141058/0000/',
"ttZ_DL":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/mc/TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8/crab_FullMorV1_amcTTZToLLNuNu_M-10_ext1/170216_140702/0000/',
"ttH_nonbb":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/mc/ttHToNonbb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/crab_FullMorV1_ttHnobb/170216_105930/0000/',
"ttjets_incl":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/mc/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/crab_FullMorV1_TT/170216_130900/0000/',
"stop_schan":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/mc/ST_s-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1/crab_FullMorV1_STs/170216_132119/0000/',
"stop_tchan_top":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/mc/ST_t-channel_top_4f_inclusiveDecays_TuneCUETP8M2T4_13TeV-powhegV2-madspin/crab_FullMorV1_STt/170217_064944/0000/',
"stop_tchan_antitop":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/mc/ST_t-channel_antitop_4f_inclusiveDecays_TuneCUETP8M2T4_13TeV-powhegV2-madspin/crab_FullMorV1_SaTt/170216_132428/0000/',
"stop_tW_antitop":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/mc/ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M2T4/crab_FullMorV1_SaT/170216_133251/0000/',
"stop_tw_top":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/mc/ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M2T4/crab_FullMorV1_ST/170217_065140/0000/',
"ttWjets_SL_ext1":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/mc/TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/crab_FullMorV1_amcTTWJetsToLNuext2/170216_135729/0000/',
"ttWjets_SL_ext2":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/mc/TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/crab_FullMorV1_amcTTWJetsToLNuext1/170216_140120/0000/',
"ZZ":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/mc/ZZ_TuneCUETP8M1_13TeV-pythia8/crab_FullMorV1_ZZ/170216_134718/0000/',
"ZZ_ext":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/mc/ZZ_TuneCUETP8M1_13TeV-pythia8/crab_FullMorV1_ZZext1/170216_135129/0000/',
"WZ":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/mc/WZ_TuneCUETP8M1_13TeV-pythia8/crab_FullMorV1_WZ/170217_065443/0000/',
"WZ_ext":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/mc/WZ_TuneCUETP8M1_13TeV-pythia8/crab_FullMorV1_WZext1/170216_134410/0000/',
"WW":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/mc/WW_TuneCUETP8M1_13TeV-pythia8/crab_FullMorV1_WW/170216_133749/0000/',
"WW_ext":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/mc/WW_TuneCUETP8M1_13TeV-pythia8/crab_FullMorV1_WWext1/170216_134055/0000/',
"data_SEleBlockB1":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/data/SingleElectron/crab_FullMorV1_SEleBlockB1/170216_125915/0000/',
"data_SEleBlockC1":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/data/SingleElectron/crab_FullMorV1_SEleBlockC1/170216_130855/0000/',
"data_SEleBlockD1":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/data/SingleElectron/crab_FullMorV1_SEleBlockD1/170216_131209/0000/',
"data_SEleBlockE1":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/data/SingleElectron/crab_FullMorV1_SEleBlockE1/170216_131445/0000/',
"data_SEleBlockF1":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/data/SingleElectron/crab_FullMorV1_SEleBlockF1/170216_131713/0000/',
"data_SEleBlockF2":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/data/SingleElectron/crab_FullMorV1_SEleBlockF2/170216_132117/0000/',
"data_SEleBlockG1":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/data/SingleElectron/crab_FullMorV1_SEleBlockG1/170216_132450/0000/',
"data_SEleBlockH1":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/data/SingleElectron/crab_FullMorV1_SEleBlockH1/170216_132719/0000/',
"data_SEleBlockH2":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/data/SingleElectron/crab_FullMorV1_SEleBlockH2/170217_142948/0000/',
"data_SMuBlockB1":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/data/SingleMuon/crab_FullMorV1_SMuBlockB1/170216_133716/0000/',
"data_SMuBlockC1":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/data/SingleMuon/crab_FullMorV1_SMuBlockC1/170217_102735/0000/',
"data_SMuBlockD1":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/data/SingleMuon/crab_FullMorV1_SMuBlockD1/170216_134614/0000/',
"data_SMuBlockE1":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/data/SingleMuon/crab_FullMorV1_SMuBlockE1/170217_081050/0000/',
"data_SMuBlockF1":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/data/SingleMuon/crab_FullMorV1_SMuBlockF1/170217_081305/0000/',
"data_SMuBlockF2":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/data/SingleMuon/crab_FullMorV1_SMuBlockF2/170216_135711/0000/',
"data_SMuBlockG1":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/data/SingleMuon/crab_FullMorV1_SMuBlockG1/170216_140118/0000/',
"data_SMuBlockH1":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/data/SingleMuon/crab_FullMorV1_SMuBlockH1/170216_140404/0000/',
"data_SMuBlockH2":'/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/data/SingleMuon/crab_FullMorV1_SMuBlockH2/170216_140830/0000/'
       }
sampleout={
"ttHbb": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_03/ttHACIHEP/output/MC/',
"ttjets_DL": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_03/ttHACIHEP/output/MC/',
"ttjets_SL": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_03/ttHACIHEP/output/MC/',
"ttjets_bfilter": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_03/ttHACIHEP/output/MC/',
"ttWjets_qq": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_03/ttHACIHEP/output/MC/',
"ttZ_qq": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_03/ttHACIHEP/output/MC/',
"ttZ_DL": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_03/ttHACIHEP/output/MC/',
"ttH_nonbb": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_03/ttHACIHEP/output/MC/',
"ttjets_incl": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_03/ttHACIHEP/output/MC/',
"stop_schan": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_03/ttHACIHEP/output/MC/',
"stop_tchan_top": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_03/ttHACIHEP/output/MC/',
"stop_tchan_antitop": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_03/ttHACIHEP/output/MC/',
"stop_tW_antitop": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_03/ttHACIHEP/output/MC/',
"stop_tw_top": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_03/ttHACIHEP/output/MC/',
"ttWjets_SL_ext1": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_03/ttHACIHEP/output/MC/',
"ttWjets_SL_ext2": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_03/ttHACIHEP/output/MC/',
"ZZ": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_03/ttHACIHEP/output/MC/',
"ZZ_ext": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_03/ttHACIHEP/output/MC/',
"WZ": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_03/ttHACIHEP/output/MC/',
"WZ_ext": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_03/ttHACIHEP/output/MC/',
"WW": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_03/ttHACIHEP/output/MC/',
"WW_ext": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_03/ttHACIHEP/output/MC/',
"data_SEleBlockB1": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_03/ttHACIHEP/output/DATA/',
"data_SEleBlockC1": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_03/ttHACIHEP/output/DATA/',
"data_SEleBlockD1": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_03/ttHACIHEP/output/DATA/',
"data_SEleBlockE1": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_03/ttHACIHEP/output/DATA/',
"data_SEleBlockF1": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_03/ttHACIHEP/output/DATA/',
"data_SEleBlockF2": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_03/ttHACIHEP/output/DATA/',
"data_SEleBlockG1": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_03/ttHACIHEP/output/DATA/',
"data_SEleBlockH1": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_03/ttHACIHEP/output/DATA/',
"data_SEleBlockH2": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_03/ttHACIHEP/output/DATA/',
"data_SMuBlockB1": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_03/ttHACIHEP/output/DATA/',
"data_SMuBlockC1": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_03/ttHACIHEP/output/DATA/',
"data_SMuBlockD1": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_03/ttHACIHEP/output/DATA/',
"data_SMuBlockE1": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_03/ttHACIHEP/output/DATA/',
"data_SMuBlockF1": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_03/ttHACIHEP/output/DATA/',
"data_SMuBlockF2": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_03/ttHACIHEP/output/DATA/',
"data_SMuBlockG1": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_03/ttHACIHEP/output/DATA/',
"data_SMuBlockH1": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_03/ttHACIHEP/output/DATA/',
"data_SMuBlockH2": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_03/ttHACIHEP/output/DATA/'
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
	#print >> subFile, "cd /publicfs/cms/data/TopQuark/cms13TeV/software/root/bin/"
	#print >> subFile, "source thisroot.csh"
	#print >> subFile, "cd /publicfs/cms/user/libh/CMSSW_5_3_9/src/ttH_13Tev"
	#print >> subFile, "setenv SCRAM_ARCH slc5_amd64_gcc462"
	#print >> subFile, "source /cvmfs/cms.cern.ch/cmsset_default.csh"
    print >> subFile, "source  /afs/ihep.ac.cn/soft/CMS/64bit/root/profile/rootenv-entry 6.08.02"
	#print >> subFile, "eval \`scramv1 runtime -sh\`"
    print >> subFile, "cd "+analyzerpath
	#print >> subFile, "cp ${jobDir}/getAhist.C ."
    print >> subFile, "./"+rootplizer+" "+input+" "+output


# Loop over all samples you want to run on.
keys=sample.keys()
for key,k in sample.iteritems():
    #print 'Index ' , k
    # sample[k] gives the full path of root input files.
    print 'Dict key = ' , key
    print 'Sample = ', k
    sampleName = key
    # Full input path + filename.
    rootDirectory   = k # +"/"+k    ## you can use your naming convension to set up the inputDirectory
    outputDirectory = sampleout[key]+"/"+key # you can use your naming convension to set up the outputDirectory
    print 'output directory = ' , outputDirectory

    # Full paths to samples created: "/afs/ihep.ac.cn/users/j/joshuha/"+analysis+"Analyzer/"+ sample
    AnalyzerSampleDir = AnalyzerDir + "/" + sampleName
    os.popen('mkdir -p ' +AnalyzerSampleDir)
    os.popen('mkdir -p ' +sampleout[key])
    os.popen('mkdir -p '+outputDirectory)
    os.chdir(rootDirectory)
    # glob.glob() is a wrapped around a os.listdir()
    roots = glob.glob('*.root')
    # Sorted alphanumerically by name:
    roots.sort()
    nroot = len(roots)
    MergeSourceFile = ""
    for iroot in range(nroot):

        # Input filename (full-path):
        input  = rootDirectory+"/"+roots[iroot]

        # Use input files to name output (with edited suffix)
        #output = outputDirectory+"/"+roots[iroot].replace(".root","_rootplas")
        #print 'output directory = ' , output
        #os.popen('mkdir -p '+output)

        # Make temp directory in which the analysis is to be performed:
        analyzerpath = AnalyzerSampleDir+"/"+roots[iroot].replace(".root","")
        #print 'Temp. analysis directory = ', analyzerpath
        os.popen('mkdir -p '+analyzerpath)

        # Copy analysis code to analysis Directory
        analysis_code_path = analysis_dir_path+"/"+rootplizer
        #print "Analysis code path = ", analysis_code_path
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
    print >> MergeFile, "hadd " + key + "_Merged_rootplas.root", " *.root"

print >> allJobFile, "cd -"
