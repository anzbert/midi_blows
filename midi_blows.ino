// Based on Adafruit advanced BME280 example, with "gaming" setup

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <MIDIUSB.h>

#define SEALEVELPRESSURE_HPA (1013.25)

// //// SPI
// #define BME_SCK 13  // for SPI communication
// #define BME_MISO 12 // for SPI communication
// #define BME_MOSI 11 // for SPI communication
// #define BME_CS 10   // for SPI communication
// Adafruit_BME280 bme(BME_CS); // hardware SPI
// Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

// //// I2C
#define I2C_ADDRESS 0x76 // bme280 sensor address for I2C communication
Adafruit_BME280 bme;     // use I2C

// //// MIDI
#define MIDI_CHAN 1 // (0 to 15) +1 = (Channel 1 to 16)
#define MIDI_CC 1   // control change number (0 - 127)

float baselinePressure = SEALEVELPRESSURE_HPA;
float rawPressure = SEALEVELPRESSURE_HPA;
float normalizedPressure = 0;

unsigned long delayBetweenMeasurements; // in ms

byte midiCurrentCC = 0;  // Current state of the CC midi value in 7bit
byte midiPreviousCC = 0; // Previous loop state of the CC midi value

void setup()
{
    Serial.begin(9600);

    while (!bme.begin(I2C_ADDRESS, &Wire))
    {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        delay(3000);
    }

    baselinePressure = bme.readPressure();

    // Sensor Settings (Datasheet: https://www.bosch-sensortec.com/products/environmental-sensors/humidity-sensors-bme280/#documents)

    //////////////////////////////////////////////////////////////////////////
    // Suggested Gaming Settings from Datasheet:
    // normal mode, 4x pressure / 1x temperature / 0x humidity oversampling,
    // = humidity off, 0.5ms standby period, filter 16x
    //
    // Suggested rate for standard gaming settings is 83Hz:
    // 1 + (2 * T_ovs) + (2 * P_ovs + 0.5)
    // T_ovs = 1
    // P_ovs = 4
    // = 11.5ms + 0.5ms standby = 12ms -> 1 / 0.012s = 83 Hz

    // bme.setSampling(Adafruit_BME280::MODE_NORMAL,   // normal mode = automatic continuous measurements
    //                 Adafruit_BME280::SAMPLING_X1,   // temperature
    //                 Adafruit_BME280::SAMPLING_X4,   // pressure
    //                 Adafruit_BME280::SAMPLING_NONE, // humidity
    //                 Adafruit_BME280::FILTER_X16,    // Lower = More Noise, but more responsive
    //                 Adafruit_BME280::STANDBY_MS_0_5); // 0.5ms is lowest available value
    // delayBetweenMeasurements = 12;

    /////////////////////////////////////////////////////////////////////////
    // Let's experiment with the standard settings to reduce latency:
    bme.setSampling(Adafruit_BME280::MODE_NORMAL,
                    Adafruit_BME280::SAMPLING_NONE, // temperature
                    Adafruit_BME280::SAMPLING_X4,   // pressure
                    Adafruit_BME280::SAMPLING_NONE, // humidity
                    Adafruit_BME280::FILTER_X4,
                    Adafruit_BME280::STANDBY_MS_0_5);
    delayBetweenMeasurements = 10;
}

void loop()
{
    // bme.takeForcedMeasurement(); // has no effect in normal mode, so can be removed
    sendPressureAsCC(bme.readPressure());
    delay(delayBetweenMeasurements);
}

void sendPressureAsCC(float rawPressure)
{
    // Serial.println(rawPressure);
    if (rawPressure > baselinePressure)

    {
        normalizedPressure = rawPressure - baselinePressure;
    }
    else
    {
        normalizedPressure = 0;
    }
    // Serial.println(applyCurve(normalizedPressure));

    midiCurrentCC = constrain(applyCurve(normalizedPressure), 0, 127);
    // Serial.println(midiCurrentCC);

    if (midiPreviousCC != midiCurrentCC)
    {
        appendCCToBuffer(MIDI_CHAN, MIDI_CC, midiCurrentCC);
        MidiUSB.flush(); // Sends Midi Buffer content

        midiPreviousCC = midiCurrentCC;
    }
}

void appendCCToBuffer(byte channel, byte control, byte value)
{
    midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
    MidiUSB.sendMIDI(event);
}

int applyCurve(float input)
{
    float result = input * 0.1; // just linear at the moment
    return (int)result;
}
