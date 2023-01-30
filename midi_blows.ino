#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10
#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C communication
unsigned long delayTime;

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

    // gaming-style responsiveness setup:
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
    delayTime = 12;

    // delayTime = 5000; // for testing
}

void loop()
{
    // bme.takeForcedMeasurement(); // has no effect in normal mode, can be removed

    printValues();
    delay(delayTime);
}

void printValues()
{
    // Serial.print("Temperature = ");
    // Serial.print(bme.readTemperature());
    // Serial.println(" *C");

    // Serial.print("Pressure in hPa = ");
    // Serial.println(bme.readPressure() / 100.0F);

    // Serial.print("Approx. Altitude in m = ");
    // Serial.println(bme.readAltitude(SEALEVELPRESSURE_HPA));

    // Serial.println("Humidity in % = ");
    // Serial.print(bme.readHumidity());

    // Serial.println();

    Serial.println(bme.readPressure());
}
