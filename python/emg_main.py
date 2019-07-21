#!/bin/bash/python

import os
import random
import sys
import signal

import csv
import getopt
import logging
import serial
import threading
import subprocess

import numpy as np

from emg_constants import *
from emg_visualize import *
from emg_filters import *

# Initialize logging
logging.basicConfig(filename=EMG_LOG_FILE, filemode='w',
                    level=logging.INFO,
                    format='%(asctime)s - %(message)s', datefmt='%d-%b-%y %H:%M:%S')

class checkSamplesAmount(threading.Thread):
    def __init__(self, threadID, name):
        threading.Thread.__init__(self)
        self._finished = threading.Event()
        self._interval = 4.0
        self.threadID = threadID
        self.name = name
        self.iterations = 0

    def setInterval(self, interval):
        self._interval = interval

    def shutdown(self):
        """Stop this thread"""
        self._finished.set()

    def run(self):
        while 1:
            if self._finished.isSet(): return
            self.task()

            # sleep for interval or until shutdown
            self._finished.wait(self._interval)

    def task(self):
        self.iterations += 1

        if emgSampleCnt != 0:
            self.iterations = 0
            print("Gathered Samples: " + str(emgSampleCnt))
        if emgSampleCnt == EMG_SAMPLES_TARGET:
            print("Sample target of {} reached stopping {}.".format(emgSampleCnt, APPLICATION))
            self.shutdown()
        if self.iterations >= ALLOWED_EMPTY_ITTERATIONS:
            print("No input samples detected!")
            print("Running serial connection check ...")
            subprocess.Popen("python emg_test_serial.py -s {} -i {}".format(serPort, TEST_ITERATIONS),
                              shell=True)
            os._exit(1)
        pass

# Global Context
emgSampleCnt = 0
cntSampleThread = checkSamplesAmount(1, "cntSampleThread")
serPort = ''

def _emgExitSignHandler(signal, frame):
    print("Closing " + APPLICATION)
    cntSampleThread.shutdown()
    sys.exit(0)
signal.signal(signal.SIGINT, _emgExitSignHandler)

def _emgInit():
    if not os.path.isdir(DATA_DIR):
        os.mkdir(DATA_DIR)

    if not os.path.isdir(LOG_DIR):
        os.mkdir(LOG_DIR)

    logging.info(APPLICATION + " has been succesfuly initialized.")

    cntSampleThread.start()

def _usage():
    print("python emg_main.py -s [--sport] < serial_port >")
    sys.exit()


def emgMain(argv):
    global emgSampleCnt
    global serPort
    # Filter configuration

    fs = 1000.0 # in Hz
    lowcut = 200.0
    highcut = 500.0
    ch_x = []

    if len(argv) != 2:
        _usage()

    try:
        opts, args = getopt.getopt(argv,"hs:",["sp="])
    except getopt.GetoptError:
        _usage()
    for opt, arg in opts:
        if opt == '-h':
            _usage()
        elif opt in ("-s", "--sport"):
            serPort = arg
        else:
            sys.exit()

    # Open com port
    try:
        ser = serial.Serial(serPort, BAUDRATE)
        logging.info(APPLICATION + " serial port " + serPort + " opened.")
    except Exception as e:
        logging.error(APPLICATION + " has failed to open the serial port: " + serPort)
        logging.warning("Check for available serial connection:")
        logging.info(os.system("ls -l /dev/ttyACM*"))
        logging.info(os.system("ls -l /dev/ttyUSB*"))
        exit()

    _emgInit()

    with open(EMG_DATA, "w") as data:
    	csv_writer= csv.writer(data)

        print(APPLICATION + " running...\n"
              "Press Ctrl + C to close\n" );

    	while emgSampleCnt != EMG_SAMPLES_TARGET:
            writeData = True
            try:
                line = ser.readline().strip()
            except Exception as e:
                logging.error(APPLICATION +
                              " failed to read data from serial port: " + str(e))


            # Split the string "180,3600,1234" into a list ["180", "3600", "1234"]
            xy_string = line.split(",")
            if len(xy_string) != 2:
                logging.error(APPLICATION +
                              " has received invalid input: " + line)
                writeData = False

            if writeData:
                try:
                    x = int(xy_string[0])
                    y = int(xy_string[1])
                    # Write XYZ to the CSV file
                    emgSampleCnt += 1
                    csv_writer.writerow([emgSampleCnt, x, y])
                    ch_x.append(x)

                    if len(ch_x) == FILTER_SAMPLES:
                        ch_x_filtered = smooth(np.array(ch_x), 1)
                        ch_x_filtered = savitzky_golay(np.array(ch_x_filtered), 7, 2)

                        #ch_x_filtered = butterBandpassFilterZi(np.array(ch_x_filtered), lowcut, highcut, fs, 3)
                        #ch_x_filtered = butterBandpassFilter(np.array(ch_x_filtered), lowcut, highcut, fs, order=3)
                        x = sum(ch_x_filtered) / FILTER_SAMPLES
                        emg_visualizeCH(x)
                        del ch_x[0:FILTER_SAMPLES]

                except Exception as e:
                    logging.error(APPLICATION +
                                 " discarting invalid data - " + str(e))
                    continue

if __name__ == "__main__":
    emgMain(sys.argv[1:])
