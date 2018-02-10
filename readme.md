# Sensibodi ("Ro Ro")
I am a rover exploring space.

## Explore Mode
1. Make sure fun mode cord (yellow) is in a GND header or unplugged.
2. Plug in power to Arduino (blue cord), and make sure its plugged into the 2.4A header.
3. You now have 10 seconds to plug in the Lipo Battery (white balance cord and red/yellow XT-60 plug), before it will go into low battery mode.
4. IMPORTANT Make sure you plug in the white balancing cord with the correct orientation, otherwise RoRo will die a painful death.
5. Explore mode will then start after PIR sensor is triggered.

## Fun Mode
1. Plug in fun mode cord (yellow) into 5V header.
2. Plug in Xbox 360 controller USB into USB header on Arduino Mega 2560.
3. You now have 10 seconds to plug in the Lipo Battery (white balance cord and red/yellow XT-60 plug), before it will go into low battery mode.
4. IMPORTANT Make sure you plug in the white balancing cord with the correct, otherwise RoRo will die a painful death.
5. Turn on Xbox 360 controller.

## Camera Vision
1. Make sure black usb cord is plugged into the 2.1A header.
2. Plug it into an HDMI screen, and using a keyboard and mouse log onto your Wifi network.
3. Make sure you are connected to the internet by browsing with the web browser.
4. Next on your PC or Mac log into roro using SSH to determine its ip address.
5. Type this command on your pc or mac using git bash/terminal: 
ssh pi@roro
ifconfig
6. Write down inet/ip address for wireless lan device.
7. Go into web browser on viewer and type in inet/ip address, then navigate to camera. Click picture to go full screen.

## Author
Joshua and Jessica Wright

## License
This project is licensed under the MIT license. See the [License](LICENSE) file for more info.
