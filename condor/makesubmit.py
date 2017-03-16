import sys
import os
import glob
import string
#####
##   Parameters to be specified by the user
#####
#analysis and task
analysis = "TTHbb"
taskname = "SL"
#for the queue
workpath    = "/afs/ihep.ac.cn/users/j/joshuha/"+analysis
jobDir      = workpath+"/"+"Jobs"# Replaced/created on the fly
AnalyzerDir = workpath+"/"+"Analyzer"# Replaced/created on the fly
task        = analysis+"_"+taskname

rootplizer = "SecondStep"
#rootplizer  = "Rootplizer_"+task+".cc"
#headplizer  = "Rootplizer_"+task+".h"

#Directory of input files
sample={
"ttHbb": '/acfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/mc/ttHTobb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/crab_FullMorV1_ttHbb/170216_130523/0000/'
       }
sampleout={
"ttHbb": '/publicfs/cms/data/TopQuark/'+analysis+'/JTW/2017_03/ttHACIHEP/output/'
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
	#print >> subFile, "source  /afs/ihep.ac.cn/soft/CMS/64bit/root/profile/rootenv-entry 5.34.18"
    print >> subFile, "source  /afs/ihep.ac.cn/soft/CMS/64bit/root/profile/rootenv-entry default-slc6"
	#print >> subFile, "eval \`scramv1 runtime -sh\`"
    print >> subFile, "cd "+analyzerpath
	#print >> subFile, "cp ${jobDir}/getAhist.C ."
    print >> subFile, "root -b -q -l "+rootplizer+"+'(\""+input+"\",\""+output+"\")'"
    print >> subFile, "./"+rootplizer+" "+input+" "+output
    #print >> subFile, "root -b -q -l "+rootplizer+"'(\""+input+"\",\""+output+"\")'"


# Loop over all samples you want to run on.
for k in sample:
    print 'Index ' , k
    # sample[k] gives the full path of root input files.
    print 'Sample = ', sample[k]
    sampleName = k
    # Full input path + filename.
    rootDirectory   = sample[k] # +"/"+k    ## you can use your naming convension to set up the inputDirectory
    outputDirectory = sampleout[k] # you can use your naming convension to set up the outputDirectory
    # Full paths to samples created: "/afs/ihep.ac.cn/users/j/joshuha/"+analysis+"Analyzer/"+ sample
    AnalyzerSampleDir = AnalyzerDir + "/" + sampleName
    os.popen('mkdir -p ' +AnalyzerSampleDir)
    os.popen('mkdir -p '+outputDirectory)
    os.chdir(rootDirectory)
    # glob.glob() is a wrapped around a os.listdir()
    roots = glob.glob('*.root')
    # Sorted alphanumerically by name:
    roots.sort()
    nroot = len(roots)
    for iroot in range(nroot):
        # Input filename (full-path):
        input  = rootDirectory+"/"+roots[iroot]
        # Use input files to name output (with edited suffix)
        output = outputDirectory+"/"+roots[iroot].replace(".root","_rootplas.root")
        # Make temp directory in which the analysis is to be performed:
        analyzerpath = AnalyzerSampleDir+"/"+roots[iroot]
        os.popen('mkdir -p '+analyzerpath)
        # Copy analysis code to analysis Directory
        command_cp_cc = 'cp '+workpath+"/"+rootplizer+" "+analyzerpath
        #command_cp_h = 'cp '+workpath+"/"+headplizer+" "+analyzerpath
        os.system(command_cp_cc)
        #os.system(command_cp_h)

        MergeSourceFile += " "+roots[iroot].replace(".root","_rootplas.root")

        # Make the name of submitFile and InputFile matched
        name = roots[iroot].replace('.root','')
        submitFileName = sampleName+"_"+name+".submit"
        cshFileName = cshFilePath+"/"+sampleName+"_"+name+".sh"
        logFileName = logFilePath+"/"+sampleName+"_"+name+".log"
        errFileName = logFilePath+"/"+sampleName+"_"+name+".err"

        prepareSubmitJob(cshFilePath+"/"+submitFileName, cshFileName, logFileName, errFileName)
        prepareCshJob(input,output,cshFileName,analyzerpath)
        print >> allJobFile, "condor_submit "+ submitFileName + " -group cms -name job@schedd01.ihep.ac.cn"


print >> MergeFile, "cd",outputDirectory
print >> MergeFile, "hadd Merged_rootplas.root",MergeSourceFile

print >> allJobFile, "cd -"
