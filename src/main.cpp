#define FASTADC 1

// defines for setting and clearing register bits
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#include <Arduino.h>
#include <Wire.h>

#define PIN A7
#define I2C_ADDR 1
#define ATTN_MARGIN 10
#define CALIBRATION_TIME 5000 // 5 sec
#define NUM_READINGS 60

uint16_t readings[NUM_READINGS];
uint8_t readings_count = 0;
bool calibrated = false;
long timer = 0;
unsigned int baseline = 512;

uint16_t spm = 0;
uint16_t sIntensity = 0;
uint16_t maxForPrc = 512;
byte sIntensityPrc = 0;

void calibrate()
{
#ifdef DEBUG
    Serial.println("CALIBRATING");
#endif //DEBUG
    timer = millis();
    baseline = analogRead(PIN);
    do
    {
        baseline = (baseline + analogRead(PIN)) / 2;
    } while (millis() - timer <= CALIBRATION_TIME);
    uint16_t highMax = 1023 - baseline;
    maxForPrc = max(baseline, highMax);
#ifdef DEBUG
    Serial.print("Baseline: ");
    Serial.println(baseline);
    Serial.print("Max INtensity: ");
    Serial.println(maxForPrc);
#endif
    calibrated = true;
}

void handleI2CRequest()
{
    if (calibrated)
    {
        Wire.write(spm);
        Wire.write(sIntensityPrc);
    }
    else
    {
        Wire.write((byte)0);
        Wire.write((byte)0);
    }
}
void setup()
{
#if FASTADC
    // set prescale to 16
    sbi(ADCSRA, ADPS2);
    cbi(ADCSRA, ADPS1);
    cbi(ADCSRA, ADPS0);
#endif
#ifdef DEBUG
    Serial.begin(115200);
#endif //DEBUG
    Wire.begin(I2C_ADDR);
    Wire.onRequest(handleI2CRequest);
    pinMode(PIN, INPUT);

    for (uint8_t i = 0; i < NUM_READINGS; i++)
    {
        readings[i] = 0;
    }
    // delay(500);
    calibrate();
#ifdef DEBUG
    Serial.println("OK");
#endif //DEBUG
    timer = millis();
}

void updateStats()
{
    for (uint8_t i = 0; i < readings_count; i++)
    {
        if (readings[i] > ATTN_MARGIN)
        {
            spm++;
            sIntensity += readings[i];
        }
    }
    if (spm > 0)
    {
        sIntensity = sIntensity / spm;
    }
    else
    {
        sIntensity = 0;
    }
    sIntensityPrc = map(sIntensity, 0, maxForPrc, 0, 100);
}
void loop()
{
    int reading = baseline - (analogRead(PIN));
    unsigned int absReading = abs(reading);
    readings[readings_count] = max(readings[readings_count], absReading);
    if (millis() - timer >= 1000)
    {
        if (readings_count + 1 == NUM_READINGS)
        {
            for (uint8_t i = 1; i < NUM_READINGS; i++)
            {
                readings[(i - 1)] = readings[i];
            }
            readings[readings_count] = 0;
        }
        else
        {
            readings_count++;
        }
        timer = millis();
#ifdef DEBUG2
        for (uint8_t i = 0; i < readings_count; i++)
        {
            Serial.print(readings[i]);
            Serial.print(" ");
        }
        Serial.println();
        Serial.println();
#endif //DEBUG2
#ifdef DEBUG
        Serial.print("Strikes/Min: ");
        Serial.println(spm);
        Serial.print("Avg Strikes Intensitu: ");
        Serial.print(sIntensityPrc);
        Serial.println("%");
#endif //DEBUG
    }
}