// Based on Adafruit advanced BME280 example, with "gaming" setup

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <MIDIUSB.h>

#define BME_SCK 13  // only used for SPI communication
#define BME_MISO 12 // only used for SPI communication
#define BME_MOSI 11 // only used for SPI communication
#define BME_CS 10   // only used for SPI communication
#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // use I2C communication !!

unsigned long delayTime = 12;

float baselinePressure = SEALEVELPRESSURE_HPA;
float rawPressure = SEALEVELPRESSURE_HPA;
float normalizedPressure = 0;

byte midiCC = 11;        // control change number (0-127)
byte midiCurrentCC = 0;  // Current state of the CC midi value 7bit
byte midiPreviousCC = 0; // Previous loop state of the CC midi value 7bit

void setup()
{
    Serial.begin(9600);

    if (!bme.begin(0x76, &Wire))
    {
        while (1)
        {
            Serial.println("Could not find a valid BME280 sensor, check wiring!");
            delay(3000);
        }
    }

    // Serial.println("-- Gaming Scenario --");
    // Serial.println("normal mode, 4x pressure / 1x temperature / 0x humidity oversampling,");
    // Serial.println("= humidity off, 0.5ms standby period, filter 16x");
    bme.setSampling(Adafruit_BME280::MODE_NORMAL,
                    Adafruit_BME280::SAMPLING_X1,   // temperature
                    Adafruit_BME280::SAMPLING_X4,   // pressure
                    Adafruit_BME280::SAMPLING_NONE, // humidity
                    Adafruit_BME280::FILTER_X16,
                    Adafruit_BME280::STANDBY_MS_0_5);

    // Suggested rate is 83Hz
    // 1 + (2 * T_ovs) + (2 * P_ovs + 0.5)
    // T_ovs = 1
    // P_ovs = 4
    // = 11.5ms + 0.5ms standby

    baselinePressure = bme.readPressure();
}

void loop()
{
    // bme.takeForcedMeasurement(); // has no effect in normal mode, so could probably be removed
    xfader();
    delay(delayTime);
}

void xfader()
{
    rawPressure = bme.readPressure();
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
        appendControlChangeToBuffer(10, midiCC, midiCurrentCC);
        MidiUSB.flush(); // Sends Midi Buffer content

        midiPreviousCC = midiCurrentCC;
    }
}

void appendControlChangeToBuffer(byte channel, byte control, byte value)
{
    midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
    MidiUSB.sendMIDI(event);
}

int applyCurve(float input)
{
    float result = input * 0.1;
    return (int)result;
}
