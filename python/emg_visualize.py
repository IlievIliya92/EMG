#!/bin/bash/python

import matplotlib.pyplot as plt
import numpy as np
import csv

plt.style.use('ggplot')

size = 100
x_vec = np.linspace(0 , 1, size + 1)[0:-1]
y_vec = [0] * len(x_vec)
line1 = []

def emg_visualizeCH(chdata):
    global y_vec
    global line1
    global size
    global x_vec

    y_vec[-1] = chdata
    line1 = emg_visualizeDataLive(x_vec, y_vec, line1, "Channel 0")
    y_vec = np.append(y_vec[1:], 0.0)

def emg_visualizeDataLive(x_vec, y1_data, line1, identifier='', pause_time=0.1):
    if line1==[]:
        plt.ion()
        fig = plt.figure(figsize=(13,6))
        ax = fig.add_subplot(111)
        line1, = ax.plot(x_vec, y1_data,'-g', alpha=0.4, color='#00DD44', linestyle='solid')
        plt.ylabel('Sample value')
        plt.title('{}'.format(identifier))
        plt.show()

    line1.set_ydata(y1_data)
    if np.min(y1_data) <= line1.axes.get_ylim()[0] or np.max(y1_data) >= line1.axes.get_ylim()[1]:
        plt.ylim([np.min(y1_data) - np.std(y1_data), np.max(y1_data) + np.std(y1_data)])

    plt.pause(pause_time)
    return line1

def emg_visualizeDataPlt(datafile):
    sn = []
    ch0 = []
    ch1 = []

    with open(datafile, 'r') as csvfile:
        plots= csv.reader(csvfile, delimiter=',')
        for row in plots:
            sn.append(int(row[0]))
            ch0.append(int(row[1]))
            ch1.append(int(row[2]))

    plt.plot(sn, ch0, marker=',')
    plt.plot(sn, ch1, marker=',')

    plt.title('Data visualization of emg samples:')
    plt.xlabel('Sample N')
    plt.ylabel('Sample Value')
    plt.show()

