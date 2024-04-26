'''
Connects to wifi and hosts an html server. 
To access type ip printed in terminal in a web browser. 
there will be three buttons to control the led on the pico
'''
import time
import network
import uasyncio as asyncio
from machine import Pin

# Hardware definitions
led = Pin("LED", Pin.OUT, value=1)

pin0 = Pin(0, Pin.OUT, value=0)
pin1 = Pin(1, Pin.OUT, value=0)

# Configure your WiFi SSID and password
ssid = 'iPhone (6)' #wifi name *********************************************
password = 'jack1928'# wifi password

check_interval_sec = 0.25

wlan = network.WLAN(network.STA_IF)


# The following HTML defines the webpage that is served
html = """<!DOCTYPE html><html>
<head><meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="icon" href="data:,">
<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}
.button { background-color: #4CAF50; border: 2px solid #000000;; color: white; padding: 15px 32px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer; }
.buttonRed { background-color: #d11d53; border: 2px solid #000000;; color: white; padding: 15px 32px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer; }
text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}
</style></head>
<body><center><h1>Boat Lift Controller</h1></center><br><br>
<form><center>
<center> <button class="button" name="button" value="top" type="submit">Top</button>
<br><br>
<center> <button class="button" name="button" value="mid" type="submit">Mid</button>
<br><br>
<center> <button class="button" name="button" value="bottom" type="submit">Bottom</button>
<br><br>
<center> <button class="buttonRed" name="button" value="stop" type="submit">Emergency Stop</button></center>
</center></form>
<br><br>
<br><br>
<p>Last command issued was %s<p></body></html>
"""

        
async def connect_to_wifi():
    wlan.active(True)
    wlan.config(pm = 0xa11140)  # Diable powersave mode
    wlan.connect(ssid, password)

    # Wait for connect or fail
    max_wait = 10
    while max_wait > 0:
        if wlan.status() < 0 or wlan.status() >= 3:
            break
        max_wait -= 1
        print('waiting for connection...')
        time.sleep(1)

    # Handle connection error
    if wlan.status() != 3:
        led .toggle()
        raise RuntimeError('WiFi connection failed')
    else:
        
        print('connected')
        status = wlan.ifconfig()
        print('ip = ' + status[0])


async def serve_client(reader, writer):
    print("Client connected")
    request_line = await reader.readline()
    print("Request:", request_line)
    # We are not interested in HTTP request headers, skip them
    while await reader.readline() != b"\r\n":
        pass
    
    # find() valid led commands within the request
    request = str(request_line)
    cmd_top = request.find('button=top')
    cmd_mid = request.find('button=mid')
    cmd_bottom = request.find('button=bottom')
    cmd_stop = request.find('button=stop')
    print ('BoatLift=top => ' + str(cmd_top)) # show where the commands were found (-1 means not found)
    print ('BoatLift=mid => ' + str(cmd_mid))
    print ('BoatLift=bottom => ' + str(cmd_bottom))
    print ('BoatLift=stop => ' + str(cmd_stop))

    stateis = "" # Keeps track of the last command issued
    
    # Carry out a command if it is found (found at index: 8)
    if cmd_top == 8:
        stateis = "BoatLift=top"
        print(stateis)
        led.on()
        pin0.value(1)  #01
        pin1.value(0)
        
    elif cmd_mid == 8:
        stateis = "BoatLift=mid"
        print(stateis)
        led.off()
        pin0.value(1)  #11
        pin1.value(1)
        
    elif cmd_bottom == 8:
        stateis = "BoatLift=bottom"
        print(stateis)
        led.toggle()
        pin0.value(0)  #10
        pin1.value(1)

    elif cmd_stop == 8:
        stateis = "BoatLift=stop"
        print(stateis)
        led.toggle()
        pin0.value(0)  #00
        pin1.value(0)
    
    response = html % stateis
    writer.write('HTTP/1.0 200 OK\r\nContent-type: text/html\r\n\r\n')
    writer.write(response)

    await writer.drain()
    await writer.wait_closed()


async def main():
    print('Connecting to WiFi...')
    asyncio.create_task(connect_to_wifi())

    print('Setting up webserver...')
    asyncio.create_task(asyncio.start_server(serve_client, "0.0.0.0", 80))

    while True:
        await asyncio.sleep(check_interval_sec)


try:
    asyncio.run(main())
finally:
    asyncio.new_event_loop()
