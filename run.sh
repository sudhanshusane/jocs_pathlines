#!/bin/bash

INPUT="/research/Sudhanshu/VIS/Cloverleaf3D/Pathlines/output/Groundtruth_"
START="20"
END="20"
INTERVAL="20"
NUM_MPI="4"
OUTPUT="/research/Sudhanshu/VIS/Cloverleaf3D/Pathlines/output/Groundtruth_All.vtk"

./PathlineEvaluation $INPUT $START $END $INTERVAL $NUM_MPI $OUTPUT
