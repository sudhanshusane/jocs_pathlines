#!/bin/bash

INPUT="/research/Sudhanshu/VIS/Cloverleaf3D/Pathlines/output/Groundtruth_"
START="20"
END="600"
INTERVAL="20"
NUM_MPI="96"
OUTPUT="/research/Sudhanshu/VIS/Cloverleaf3D/Pathlines/output/Groundtruth_All.vtk"

./PathlineEvaluation $INPUT $START $END $INTERVAL $NUM_MPI $OUTPUT
