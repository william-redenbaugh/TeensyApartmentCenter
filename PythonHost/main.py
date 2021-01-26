import serial 
import time
import serial_neopixel_control

neopixel_control = serial_neopixel_control.NeopixelSerialControl("/dev/ttyACM0", 484)
neopixel_control.set_pixel(0, 100, 100, 0)
neopixel_control.update()


while True: 
    for n in range(484): 
        neopixel_control.set_pixel(n, 100, 100, 100)
        neopixel_control.update()
        # time.sleep(.001)

    for n in range(484): 
        neopixel_control.set_pixel(n, 0, 0, 0)
        neopixel_control.update()
        time.sleep(.001)
        