#!/bin/bash
bsub -q 8nh -J ttHbb_SS_bsub < lxbatchSubmit_SSttH.sh
bsub -q 8nh -J ttjets_SS_bsub < lxbatchSubmit_SSttjets.sh
bsub -q 8nh -J DataBG-DL_SS_bsub < lxbatchSubmit_SSDataB-G_DL.sh
bsub -q 8nh -J DataBG-SL_SS_bsub < lxbatchSubmit_SSDataB-G_SL.sh
bsub -q 8nh -J DataH-DL_SS_bsub < lxbatchSubmit_SSDataH_DL.sh
bsub -q 8nh -J DataH-SL_SS_bsub < lxbatchSubmit_SSDataH_SL.sh
