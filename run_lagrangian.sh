#!/bin/bash

DATASET="Nyx"
TEST=3
START="25"
END="400"
INTERVAL="25"
NUM_MPI="1"
INPUT="/research/Sudhanshu/VIS/"$DATASET"/Lagrangian/"$TEST"/output/Pathlines_"
OUTPUT="/research/Sudhanshu/VIS/"$DATASET"/Lagrangian/"$TEST"/output/Lagrangian_Pathlines_All"$TEST".vtk"

./PathlineEvaluation $INPUT $START $END $INTERVAL $NUM_MPI $OUTPUT
