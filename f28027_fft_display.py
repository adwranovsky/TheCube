#!/usr/bin/env python3

#################################################################
# Author: Alex Wranovsky                                        #
# Gets a waveform and fft from an F28027 and creates a printout #
#################################################################

import matplotlib.pyplot as plt
import numpy
import serial
import sys
import math
import re


if __name__ == "__main__":
    # get args and open port
    port_name = sys.argv[1]
    sample_period = 1.0/float(sys.argv[2])
    port = serial.Serial(port_name, 9600, timeout=2)

    # start the collection sequence
    port.write(b's')

    # collect the samples
    line = port.read_until(b'\n\r').decode('utf-8')
    while not re.search(r'START TIME DOMAIN', line):
        print(line)
        line = port.read_until(b'\n\r').decode('utf-8')

    sample_indices = []
    samples = []
    line = port.read_until(b'\n\r').decode('utf-8')
    while not re.search(r'END TIME DOMAIN', line):
        rows = line.split(', ')
        sample_num = int(rows[0])
        sample_val = int(rows[1])
        sample_indices.append(sample_num*sample_period)
        samples.append(sample_val)
        line = port.read_until(b'\n\r').decode('utf-8')

    # collect fft
    line = port.read_until(b'\n\r').decode('utf-8')
    while not re.search('START FREQ DOMAIN', line):
        line = port.read_until(b'\n\r').decode('utf-8')

    frequencies = []
    magnitudes = []
    line = port.read_until(b'\n\r').decode('utf-8')
    while not re.search('END FREQ DOMAIN', line):
        rows = line.split(', ')
        frequency = float(rows[0])
        magnitude = float(rows[1])
        frequencies.append(frequency)
        magnitudes.append(math.sqrt(magnitude))
        line = port.read_until(b'\n\r').decode('utf-8')

    # create graph
    plt.figure(1, figsize=(9,3))

    plt.subplot(131)
    plt.plot(sample_indices, samples)
    plt.xlabel('time')
    plt.ylabel('value')

    plt.subplot(132)
    plt.plot(frequencies, magnitudes, 'ro')
    plt.xlabel('Freq (Hz)')
    plt.ylabel('Magnitude^2')

    plt.show()


    # clean up
    port.close()

