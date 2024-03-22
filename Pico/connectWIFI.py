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

# Configure your WiFi SSID and password
ssid = 'CenturyLink0306' #wifi name *********************************************
password = 'eagleview'# wifi password

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
<body><center><h1>led controller</h1></center><br><br>
<form><center>
<center> <button class="button" name="led" value="on" type="submit">led on</button>
<br><br>
<center> <button class="buttonRed" name="led" value="off" type="submit">led off</button>
<br><br>
<center> <button class="button" name="led" value="toggle" type="submit">led toggle</button></center>
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
        blink_led(0.1, 10)
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
    cmd_on = request.find('led=on')
    cmd_off = request.find('led=off')
    cmd_toggle = request.find('led=toggle')
    print ('led=on => ' + str(cmd_on)) # show where the commands were found (-1 means not found)
    print ('led=off => ' + str(cmd_off))
    print ('led=toggle => ' + str(cmd_toggle))

    stateis = "" # Keeps track of the last command issued
    
    # Carry out a command if it is found (found at index: 8)
    if cmd_on == 8:
        stateis = "led: on"
        print(stateis)
        led.on()
        
    elif cmd_off == 8:
        stateis = "led: off"
        print(stateis)
        led.off()
        
    elif cmd_toggle == 8:
        stateis = "led: toggle"
        print(stateis)
        led.toggle()
    
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