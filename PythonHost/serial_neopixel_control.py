import serial 
import colorsys

class NeopixelSerialControl: 
    def __init__(self, port, num_leds): 
        # Saving relevant variables
        self.num_leds = num_leds 
        self.port = port
        
        # Header array.
        self.led_arr = [0] * (num_leds * 3 + 4)
        self.set_header() 

        # Connect to the serial device. 
        self.serial_device = serial.Serial(port)
    
    # Set header of message so device knows that the message is accurate. 
    def set_header(self):     
        self.led_arr[0] = 24
        self.led_arr[1] = 48
        self.led_arr[2] = 69
        self.led_arr[3] = 52

    # Update the array 
    def update(self):
        self.serial_device.write(bytearray(self.led_arr))

    # Sets the current pixel to a color. 
    def set_pixel(self, n, r, g, b):
        self.led_arr[4 + n*3] = r
        self.led_arr[5 + n*3] = g
        self.led_arr[6 + n*3] = b

    
class NeopixelMatrix:
    def __init__(self, neopixel_control_obj, width, len): 
        self.neopixel_control_obj = neopixel_control_obj
        self.width = width
        self.len = len
    
    # Sets the pixel of the matrix. 
    def set_pixel(self, x, y, r, g, b): 
        if(y % 2 == 0): 
            pos = (self.width * y + x)
            self.neopixel_control_obj.set_pixel(pos, r, g, b)
        else: 
            pos = self.width * y + (self.width - 1 - x)
            self.neopixel_control_obj.set_pixel(pos, r, g, b)

    def set_pixel_hsv(self, x, y, h, s, v):
        hue = h
        if hue > 255:
            hue = 255
        if hue < 0:
            hue = 0
        saturation = s
        if saturation > 255:
            saturation = 255
        if saturation < 0:
            saturation = 0
        value = v
        if value > 255:
            value = 255
        if value < 0:
            value = 0

        r, g, b = colorsys.hsv_to_rgb(hue/256, saturation/256, value/256)
        r = int(r * 256)
        g = int(g * 256)
        b = int(b * 256)
        self.set_pixel(x, y, r, g, b)

    #  Forward the update command.
    def update(self): 
        self.neopixel_control_obj.update()