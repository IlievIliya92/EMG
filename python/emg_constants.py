#!/bin/bash/python

# Constans
APPLICATION = "emg"
TEST_APPLICATION = "emg_test"
TEST_ITERATIONS = 10
ALLOWED_EMPTY_ITTERATIONS = 5

BAUDRATE = 115200

LOG_DIR = "log"
EMG_LOG_FILE = LOG_DIR + "/" + APPLICATION + ".log"
EMG_LOG_FILE_TEST_SERIAL = EMG_LOG_FILE = LOG_DIR + "/" + APPLICATION + "_test_serial.log"

DATA_DIR = "data"
EMG_DATA = DATA_DIR + "/datafile.csv"

FILTER_SAMPLES     = 20
EMG_SAMPLES_TARGET = ""
