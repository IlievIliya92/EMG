import serial
import signal
import logging
import sys
import os
import getopt

from emg_constants import *

def _usage():
    print("python emg_test_serial.py -s [--sport] < serial_port > -i < itterations >")
    sys.exit()

def _emgExitTestSignHandler(signal, frame):
    print("Closing " + TEST_APPLICATION)
    sys.exit(0)
signal.signal(signal.SIGINT, _emgExitTestSignHandler)

def emgTestSerial(argv):
    # Initialize logging
    logging.basicConfig(filename=EMG_LOG_FILE_TEST_SERIAL, filemode='w',
                        level=logging.DEBUG,
                        format='%(asctime)s - %(message)s', datefmt='%d-%b-%y %H:%M:%S')

    if len(argv) != 4:
        _usage()

    try:
        opts, args = getopt.getopt(argv,"hs:i:",["sp="])
    except getopt.GetoptError:
        _usage()
    for opt, arg in opts:
        if opt == '-h':
            _usage()
        elif opt in ("-s", "--sport"):
            serPort = arg
        elif opt in ("-i", "--iterations"):
            iterations = arg
        else:
            sys.exit()

    print("\nRunning {} with {} iterations".format(serPort, iterations))
    print("Received serial data:")

    try:
        ser = serial.Serial(serPort, BAUDRATE)
        logging.info(TEST_APPLICATION + " serial port " + serPort + " opened for debugging.")
    except Exception as e:
        logging.error(TEST_APPLICATION + " has failed to open the serial port: " + serPort)
        logging.warning("Check for available serial connection:")
        logging.info(os.system("ls -l /dev/ttyACM*"))
        logging.info(os.system("ls -l /dev/ttyUSB*"))
        exit()

    try:
        line = ser.readline().strip()
    except Exception as e:
        logging.error(TEST_APPLICATION +
        " failed to read data from serial port: " + str(e))

    iterations_cnt = 0
    while True:
        iterations_cnt += 1
        try:
            line = ser.readline().strip()
            print(str(iterations_cnt) + " : " + line)
            logging.info(TEST_APPLICATION +
                         " Data {} : {}".format(iterations_cnt, line))
        except Exception as e:
            pass

        if iterations_cnt == int(iterations):
            sys.exit()

if __name__ == "__main__":
    emgTestSerial(sys.argv[1:])

