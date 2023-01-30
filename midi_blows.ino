#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <MIDIUSB.h>

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10
#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C communication
float baselinePressure = 0;
unsigned long delayTime = 12;

byte cc = 10; // CC (0-127)
float rawState = 0;
float normalizedState = 0;
int midiCurrentState = 0; // Current state of the CC midi value 7bit
int midiPrevState = 0;    // Previous loop state of the CC midi value 7bit

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
    // bme.takeForcedMeasurement(); // has no effect in normal mode, can be removed

    xfader();
    //  Serial.println(bme.readPressure());

    delay(delayTime);
}

void controlChange(byte channel, byte control, byte value)
{
    midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
    MidiUSB.sendMIDI(event);
}

void xfader()
{
    rawState = bme.readPressure(); // raw input

    if (rawState > baselinePressure)
    {
        normalizedState = rawState - baselinePressure;
    }
    else
    {
        normalizedState = 0;
    }

    // midiCurrentState = normalizedState * 0.1;
    midiCurrentState = constrain(applyCurve(normalizedState), 0, 127);

    if (midiPrevState != midiCurrentState)
    { // only send CC change if midistate has changed

        controlChange(10, cc, midiCurrentState); // send control change (channel, CC, value) to Midi Buffer
        MidiUSB.flush();                         // flush Midi Buffer (-> send CC)
        // Serial.println(midiCurrentState); // print midi position

        midiPrevState = midiCurrentState; // Stores the current Midistate to compare with the next on next loop
    }
}

int applyCurve(float input)
{
    float result = input * 0.1;
    return (int)result;
}
