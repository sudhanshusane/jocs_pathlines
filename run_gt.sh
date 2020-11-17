#!/bin/bash

INPUT="/research/Sudhanshu/VIS/Nyx/Pathlines/output/Groundtruth_"
#INPUT="/research/Sudhanshu/VIS/Cloverleaf3D/Pathlines/output/Groundtruth_"
START="25"
END="400"
INTERVAL="25"
NUM_MPI="1"
OUTPUT="/research/Sudhanshu/VIS/Nyx/Pathlines/output/Groundtruth_All.vtk"

./PathlineEvaluation $INPUT $START $END $INTERVAL $NUM_MPI $OUTPUT
