import serial 
import time
import serial_neopixel_control
import pyaudio
import struct
import matplotlib.pyplot as plt
import numpy as np
from scipy import signal
from scipy.fft import fft

neopixel_control = serial_neopixel_control.NeopixelSerialControl("/dev/ttyACM1", 484)

neomatrix = serial_neopixel_control.NeopixelMatrix(neopixel_control, 44, 11)

def pretty_colors(): 
    global neomatrix
    while True:
            for h in range(255):
                for y in range(11):
                    for x in range(44):
                        hue_val = ((y * x)*6 + h * 3) % 255
                        neomatrix.set_pixel_hsv(x, y, hue_val, 255, 60)
                neomatrix.update()
                time.sleep(0.01)

def fft_test(): 
    global neomatrix
    mic = pyaudio.PyAudio()
    FORMAT = pyaudio.paInt16
    CHANNELS = 1
    RATE = 20000
    CHUNK = int(RATE/20)
    stream = mic.open(format=FORMAT, channels=CHANNELS, rate=RATE, input=True, output=True, frames_per_buffer=CHUNK)

    while True: 
        data = abs(np.fromstring(stream.read(88), dtype=np.int16))
        fft_data = (abs(fft(data)))

        for i in range(44): 
            fft_data[i] = fft_data[i]/400
            if(fft_data[i] > 10): 
                fft_data[i] = 10

        for x in range(44):
            for y in range(11):
                neomatrix.set_pixel_hsv(x, y, 0, 0, 0)

        for x in range(44): 
            for y in range(int(fft_data[x])):
                neomatrix.set_pixel_hsv(x, y, 100, 100, 100)   

        neomatrix.update()         

        time.sleep(.06)

fft_test()