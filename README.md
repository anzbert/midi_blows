# Midi Blows!!

An Arduino USB Breath Controller using cheap as components.

Ingredients:

- 1x Arduino Pro Micro
- 1x BME280 Pressure Sensor
- 1x 10ml Syringe as a pressure container for the sensor
- (Optional) Some thin Tubing + Mouth piece to blow into

Recipe:

- Wire it all up via I2C like it was a weather station.
- Get a syringe. Something like 10ml (ok size) or 20ml (Bit big. Feels a bit like blowing something else 😆). Drill a tight whole into it for the cable and put the sensor into it. If you have tubing you can use that to blow into. May require some kind of mouth piece as well.
- Map the CC to your VST parameter, and blow into it while playing your midi keyboard.

## Tips

- Some VST have an expression control to map the controller to. If not, maybe just map it to the instrument's volume or filter cutoff frequency.

## Possible Improvements

- Play with the oversampling settings to reduce latency further down from the current 12ms.

## Links

- [Datasheet](https://www.bosch-sensortec.com/products/environmental-sensors/humidity-sensors-bme280/#documents) for the BME280
