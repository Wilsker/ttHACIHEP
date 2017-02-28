#!/bin/bash
bsub -q 8nh -J ttHbb_SS_bsub < lxbatchSubmit_SSttH.sh
bsub -q 8nh -J ttjets_SS_bsub < lxbatchSubmit_SSttjets.sh
