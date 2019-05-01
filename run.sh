#!/bin/bash

export PYTHONPATH=/home/hscc/jimmy/the-third-eye-v3/src

make -j8

./darknet detector demo hscc.cfg/hscc.data hscc.cfg/yolov3.cfg hscc.cfg/weights/yolov3_10000.weights test.mp4 -thresh 0.3
#./darknet detector demo hscc.cfg/hscc.data hscc.cfg/yolov3.cfg hscc.cfg/weights/yolov3_10000.weights time_counter.flv -thresh 0.3
