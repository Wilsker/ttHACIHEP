#!/bin/bash
bsub -q 8nh -J DataBCD_SS_bsub_SL < lxbatchSubmit_SS_synch_RunBCD_SL.sh
bsub -q 8nh -J DataE_SS_bsub_SL < lxbatchSubmit_SS_synch_RunE_SL.sh
bsub -q 8nh -J DataF1_SS_bsub_SL < lxbatchSubmit_SS_synch_RunF1_SL.sh
bsub -q 8nh -J DataF2_SS_bsub_SL < lxbatchSubmit_SS_synch_RunF2_SL.sh
bsub -q 8nh -J DataG_SS_bsub_SL < lxbatchSubmit_SS_synch_RunG_SL.sh
bsub -q 8nh -J DataH_SS_bsub_SL < lxbatchSubmit_SS_synch_RunH_SL.sh
bsub -q 8nh -J DataBCD_SS_bsub_DL < lxbatchSubmit_SS_synch_RunBCD_DL.sh
bsub -q 8nh -J DataE_SS_bsub_DL < lxbatchSubmit_SS_synch_RunE_DL.sh
bsub -q 8nh -J DataF1_SS_bsub_DL < lxbatchSubmit_SS_synch_RunF1_DL.sh
bsub -q 8nh -J DataF2_SS_bsub_DL < lxbatchSubmit_SS_synch_RunF2_DL.sh
bsub -q 8nh -J DataG_SS_bsub_DL < lxbatchSubmit_SS_synch_RunG_DL.sh
bsub -q 8nh -J DataH_SS_bsub_DL < lxbatchSubmit_SS_synch_RunH_DL.sh
