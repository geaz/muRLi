# Filter Generation: http://www.schwietering.com/jayduino/filtuino/index.php

import numpy as np
import matplotlib.pyplot as plotter
import math

Fs = 10000          # Sampling frequency                    
T = 1/Fs            # Sampling period
t = np.arange(0, 1, T)        # Time vector

# Signal with 50hz, 130hz, 1046hz, 3140hz and 3500hz and 4000hz
signal = np.sin(2*math.pi*50*t) + np.sin(2*math.pi*130*t) + np.sin(2*math.pi*1046*t) + np.sin(2*math.pi*3140*t) + np.sin(2*math.pi*3500*t) + np.sin(2*math.pi*4000*t)

#Corrupt the signal with zero-mean white noise with a variance of 4.
signalNoised = signal + 2*np.random.randn(len(t))

signalLowPassed = []
signalHighPassed = []
signalBandPassed = []

# Low pass butterworth filter order=2 alpha1=0.01 
v = [0.0, 0.0, 0.0]
def butterLowPass(x):
    v[0] = v[1]
    v[1] = v[2]
    v[2] = (9.446918438401618072e-4 * x) + (-0.91497583480143362955 * v[0]) + (1.91119706742607298189 * v[1])
    return (v[0] + v[2]) + 2 * v[1]

# High pass butterworth filter order=2 alpha1=0.32
vHigh = [0.0, 0.0, 0.0]
def butterHighPass(x):
    vHigh[0] = vHigh[1]
    vHigh[1] = vHigh[2]
    vHigh[2] = (1.750876436721008067e-1 * x) + (-0.21965398391369472808 * vHigh[0]) + (-0.51930340922529150127 * vHigh[1])
    return (vHigh[0] + vHigh[2]) - 2 * vHigh[1]

# Band pass butterworth filter order=2 alpha1=0.01 alpha2=0.32 
vBand = [0.0, 0.0, 0.0, 0.0, 0.0]
def butterBandPass(x):
    vBand[0] = vBand[1]
    vBand[1] = vBand[2]
    vBand[2] = vBand[3]
    vBand[3] = vBand[4]
    vBand[4] = (4.146985469705365968e-1 * x) + (-0.20667198516656454244 * vBand[0]) + (-0.02794930525906139107 * vBand[1]) + (-0.18114773810038770074 * vBand[2]) + (1.40897920655145059143 * vBand[3])
    return (vBand[0] + vBand[4]) - 2 * vBand[2]

for i in range(len(signalNoised)):    
    signalLowPassed.append(butterLowPass(signalNoised[i]))
    signalHighPassed.append(butterHighPass(signalNoised[i]))
    signalBandPassed.append(butterBandPass(signalNoised[i]))

figure, axis = plotter.subplots(3, 1)
plotter.subplots_adjust(hspace=1)

axis[0].set_title('Signal with noise')
axis[0].plot(t, signalNoised)
axis[0].set_xlabel('Time')
axis[0].set_ylabel('Amplitude')

fourierTransform = np.fft.fft(signalNoised)/len(signalNoised)
fourierTransform = fourierTransform[range(int(len(signalNoised)/2))]
tpCount     = len(signalNoised)
values      = np.arange(int(tpCount/2))
timePeriod  = tpCount/Fs
frequencies = values/timePeriod

fourierTransformHigh = np.fft.fft(signalHighPassed)/len(signalHighPassed)
fourierTransformHigh = fourierTransformHigh[range(int(len(signalHighPassed)/2))]
tpCountHigh     = len(signalHighPassed)
valuesHigh      = np.arange(int(tpCountHigh/2))
timePeriodHigh  = tpCountHigh/Fs
frequenciesHigh = valuesHigh/timePeriodHigh

fourierTransformLow = np.fft.fft(signalLowPassed)/len(signalLowPassed)
fourierTransformLow = fourierTransformLow[range(int(len(signalLowPassed)/2))]
tpCountLow     = len(signalLowPassed)
valuesLow      = np.arange(int(tpCountLow/2))
timePeriodLow  = tpCountLow/Fs
frequenciesLow = valuesLow/timePeriodLow

fourierTransformBand = np.fft.fft(signalBandPassed)/len(signalBandPassed)
fourierTransformBand = fourierTransformBand[range(int(len(signalBandPassed)/2))]
tpCountBand     = len(signalBandPassed)
valuesBand      = np.arange(int(tpCountBand/2))
timePeriodBand  = tpCountBand/Fs
frequenciesBand = valuesBand/timePeriodBand

axis[1].set_title('Fourier transform (Unpassed, Low-Pass, High-Pass)')
axis[1].plot(frequencies, abs(fourierTransform), 'r', frequenciesLow, abs(fourierTransformLow), 'g', frequenciesHigh, abs(fourierTransformHigh), 'b')
axis[1].set_xlabel('Frequency')
axis[1].set_ylabel('Amplitude')

axis[2].set_title('Fourier transform Band Passed')
axis[2].plot(frequenciesBand, abs(fourierTransformBand), 'g')
axis[2].set_xlabel('Frequency')
axis[2].set_ylabel('Amplitude')

plotter.show()