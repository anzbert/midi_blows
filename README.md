<a href='https://ko-fi.com/S6S8SP865' target='_blank'><img height='36' style='border:0px;height:36px;' src='https://storage.ko-fi.com/cdn/kofi4.png?v=3' border='0' alt='Buy Me a Coffee at ko-fi.com' /></a>


# Midi Blows!!

An Arduino USB Breath Controller using cheap as components. Surprisingly intuitive to use :)

Ingredients:

- 1x Arduino Pro Micro
- 1x BME280 Pressure Sensor
- 1x 20ml Syringe as a pressure container for the sensor
- Some thin Tubing + Mouth piece to blow into. I used another smaller syringe for that

## Software Mapping

- Some VST have an expression control to map the controller to. If not, maybe just map it to the instrument's volume or filter cutoff frequency.
- Wind instruments and Strings work really well!

## Build Details and Other Thoughts

- Air escape hole location and sizes can vary. Currently the air hole in my prototype has been drilled into the bottom area of the mouth piece (about 3mm size), not the pressure container. That seems to work quite well.
- Further tweaking the Oversampling and Filter settings for the sensor could reduce latency even more. It is very responsive already though.
- Tubing diameter and pressure container design could be experimented with.
- The pressure container volume can easily be changed with the syringe plunger with changes the responsiveness and required blowing force.
- The pressure to CC conversion curve function can easily be edited to adapt to preference. At the moment its a simple linear scaling function.
- The sensor pressure baseline can be reset by restarting the device.

## Links

- [Video with Sound](https://youtu.be/7HZMLERt6Gc) Sorry, I am neither a woodwind, nor a piano player 😝
- [Datasheet](https://www.bosch-sensortec.com/products/environmental-sensors/humidity-sensors-bme280/#documents) for the BME280. The BMP280 is cheaper and should work just as well.
- Commercially available USB Breath controller for comparison from [Tecontrol](https://www.tecontrol.se/products/usb-midi-breath-controller) (Quite pricey!)

## Pictures

It doesn't look too attractive, but works well 🤷

![current_version](./midi_blows_pic.jpg)
