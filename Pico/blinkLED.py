from machine import Pin
from time import sleep
led = Pin("LED", Pin.OUT)
while(1):
    sleep(1)
    led.toggle()