#!/bin/bash

DATASET="Nyx"
TEST=1
START="25"
END="400"
INTERVAL="25"
NUM_MPI="1"
INPUT="/research/Sudhanshu/VIS/"$DATASET"/Eulerian/"$TEST"/output/EulerianPathlines_"
OUTPUT="/research/Sudhanshu/VIS/"$DATASET"/Eulerian/"$TEST"/output/Eulerian_Pathlines_All"$TEST".vtk"

./PathlineEvaluation $INPUT $START $END $INTERVAL $NUM_MPI $OUTPUT
