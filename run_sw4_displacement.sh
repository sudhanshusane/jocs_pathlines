#!/bin/bash

OUTPUT="sw4_displacement.csv"

rm $OUTPUT

SEED="/research/Sudhanshu/VIS/SW4/Seeds.txt"
NUM_SEEDS="90000"
NUM_MPI="6"
INTERVAL="2000"

GT="/research/Sudhanshu/VIS/SW4/sw4_accuracy_tests/gt/output/Groundtruth_"
EUL250="/research/Sudhanshu/VIS/SW4/sw4_accuracy_tests/eul/output_250/EulerianPathlines_"
EUL500="/research/Sudhanshu/VIS/SW4/sw4_accuracy_tests/eul/output_500/EulerianPathlines_"
LAG1="/research/Sudhanshu/VIS/SW4/sw4_accuracy_tests/1/output/Pathlines_"
LAG8="/research/Sudhanshu/VIS/SW4/sw4_accuracy_tests/2/output/Pathlines_"
LAG27="/research/Sudhanshu/VIS/SW4/sw4_accuracy_tests/3/output/Pathlines_"
LAG64="/research/Sudhanshu/VIS/SW4/sw4_accuracy_tests/4/output/Pathlines_"
OP="/home/users/ssane/Dev/VIS20_research/pathline_evaluation/SW4_Fields.vtk"

./SW4_Evaluation $SEED $NUM_SEEDS $NUM_MPI $INTERVAL $GT $EUL250 $EUL500 $LAG1 $LAG8 $LAG27 $LAG64 $OP >> $OUTPUT 
