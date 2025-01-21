# ELEGOO-Smart-Robot-Car-Kit-V4.0-2024.01.30

That version is hard to find, but can be downloaded from Elegoo as Zip on this page:
[https://www.elegoo.com/pages/download](https://www.elegoo.com/pages/download)

When you are there, select "STEM Kits" on the left, "Robot Kits" as Series and "Smart Robot Car Kit V4.0 (With Camera)" as Model. Then download "Tutorial" under the "Product Files" section.

## About this repository here
Consider this repository as a fork of the above mentioned download. It is based on the most recent offical software version I could find in January 2025.

The intention of that repo here is to extend the Smart Robot Car with an AZ-Delivery Mega2560 to allow for further sensory and actuator additions. As the original UNO is almost pinned out and has only one unused pin left I've successfully mounted a Mega2560 instead of the UNO. The original software works on it without changes.

## Extensions to the original code
The robot car needs sound. Hence, the first addition is an [AZ-Delivery DFPlayer Mini Master Module](https://www.az-delivery.de/products/mp3-player-modul). The intention of it is to play different sounds at certain situations, e.g.
- Greet on switched on
- Honk on obstacle encountered
- Specific sound on line tracking activated
- Specific sound on self driving activated

### Adding Sound using a DFPlayer Mini Master Module
Since we're Futurama fans I've downloaded some sounds of Fry, Leela and Bender from [https://www.101soundboards.com](https://www.101soundboards.com) to be used for the events. The files need to be put on an SD card into the folder "mp3".
- Greet on switched on: 0008leel.mp3 (Yes, the system works)
- Honk on obstacle encountered: 0001honk.mp3
- Specific sound on obstacle avoidance activated: 0004bend.mp3 (I'm unstoppable)
- Specific sound on line tracking activated: 0003fry.mp3 (Oh my god)
- Specific sound on self driving activated: 0002bend.mp3 (Ha ha ha ha ha ha ha)

