#!/bin/bash
bsub -q 8nh -J DataBCD_SS_bsub_SL_SingleElectron < lxbatchSubmit_SS_synch_RunBCD_SL_SingleElectron.sh
bsub -q 8nh -J DataE_SS_bsub_SL_SingleElectron < lxbatchSubmit_SS_synch_RunE_SL_SingleElectron.sh
bsub -q 8nh -J DataF1_SS_bsub_SL_SingleElectron < lxbatchSubmit_SS_synch_RunF1_SL_SingleElectron.sh
bsub -q 8nh -J DataF2_SS_bsub_SL_SingleElectron < lxbatchSubmit_SS_synch_RunF2_SL_SingleElectron.sh
bsub -q 8nh -J DataG_SS_bsub_SL_SingleElectron < lxbatchSubmit_SS_synch_RunG_SL_SingleElectron.sh
bsub -q 8nh -J DataH_SS_bsub_SL_SingleElectron < lxbatchSubmit_SS_synch_RunH_SL_SingleElectron.sh

bsub -q 8nh -J DataBCD_SS_bsub_SL_SingleMuon < lxbatchSubmit_SS_synch_RunBCD_SL_SingleMuon.sh
bsub -q 8nh -J DataE_SS_bsub_SL_SingleMuon < lxbatchSubmit_SS_synch_RunE_SL_SingleMuon.sh
bsub -q 8nh -J DataF1_SS_bsub_SL_SingleMuon < lxbatchSubmit_SS_synch_RunF1_SL_SingleMuon.sh
bsub -q 8nh -J DataF2_SS_bsub_SL_SingleMuon < lxbatchSubmit_SS_synch_RunF2_SL_SingleMuon.sh
bsub -q 8nh -J DataG_SS_bsub_SL_SingleMuon < lxbatchSubmit_SS_synch_RunG_SL_SingleMuon.sh
bsub -q 8nh -J DataH_SS_bsub_SL_SingleMuon < lxbatchSubmit_SS_synch_RunH_SL_SingleMuon.sh

bsub -q 8nh -J DataBCD_SS_bsub_DL_DoubleEG < lxbatchSubmit_SS_synch_RunBCD_DL_DoubleEG.sh
bsub -q 8nh -J DataE_SS_bsub_DL_DoubleEG < lxbatchSubmit_SS_synch_RunE_DL_DoubleEG.sh
bsub -q 8nh -J DataF1_SS_bsub_DL_DoubleEG < lxbatchSubmit_SS_synch_RunF1_DL_DoubleEG.sh
bsub -q 8nh -J DataF2_SS_bsub_DL_DoubleEG < lxbatchSubmit_SS_synch_RunF2_DL_DoubleEG.sh
bsub -q 8nh -J DataG_SS_bsub_DL_DoubleEG < lxbatchSubmit_SS_synch_RunG_DL_DoubleEG.sh
bsub -q 8nh -J DataH_SS_bsub_DL_DoubleEG < lxbatchSubmit_SS_synch_RunH_DL_DoubleEG.sh

bsub -q 8nh -J DataBCD_SS_bsub_DL_DoubleMuon < lxbatchSubmit_SS_synch_RunBCD_DL_DoubleMuon.sh
bsub -q 8nh -J DataE_SS_bsub_DL_DoubleMuon < lxbatchSubmit_SS_synch_RunE_DL_DoubleMuon.sh
bsub -q 8nh -J DataF1_SS_bsub_DL_DoubleMuon < lxbatchSubmit_SS_synch_RunF1_DL_DoubleMuon.sh
bsub -q 8nh -J DataF2_SS_bsub_DL_DoubleMuon < lxbatchSubmit_SS_synch_RunF2_DL_DoubleMuon.sh
bsub -q 8nh -J DataG_SS_bsub_DL_DoubleMuon < lxbatchSubmit_SS_synch_RunG_DL_DoubleMuon.sh
bsub -q 8nh -J DataH_SS_bsub_DL_DoubleMuon < lxbatchSubmit_SS_synch_RunH_DL_DoubleMuon.sh

bsub -q 8nh -J DataBCD_SS_bsub_DL_MuonEG < lxbatchSubmit_SS_synch_RunBCD_DL_MuonEG.sh
bsub -q 8nh -J DataE_SS_bsub_DL_MuonEG < lxbatchSubmit_SS_synch_RunE_DL_MuonEG.sh
bsub -q 8nh -J DataF1_SS_bsub_DL_MuonEG < lxbatchSubmit_SS_synch_RunF1_DL_MuonEG.sh
bsub -q 8nh -J DataF2_SS_bsub_DL_MuonEG < lxbatchSubmit_SS_synch_RunF2_DL_MuonEG.sh
bsub -q 8nh -J DataG_SS_bsub_DL_MuonEG < lxbatchSubmit_SS_synch_RunG_DL_MuonEG.sh
bsub -q 8nh -J DataH_SS_bsub_DL_MuonEG < lxbatchSubmit_SS_synch_RunH_DL_MuonEG.sh
