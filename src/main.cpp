#include <Arduino.h>
#include "DS18B20.h"
#include "EBYTE.h"

constexpr byte M0_PIN       = 35;
constexpr byte M1_PIN       = 14;
constexpr byte TX_PIN       = 12;
constexpr byte RX_PIN       = 13;
constexpr byte DS18B20_PIN  = 23;

EBYTE Transceiver(&Serial2, M0_PIN, M1_PIN, -1);
DS18B20 ds(DS18B20_PIN);

struct DATA {
  float temperature;
} message;

void setup_e32(void)
{
	Transceiver.init();
	Serial.println(Transceiver.GetAirDataRate());
  Serial.println(Transceiver.GetChannel());

  Transceiver.SetAddressH(0);
  Transceiver.SetAddressL(0);
  Transceiver.SetSpeed(0b00011100);  
  Transceiver.SetChannel(1);
  Transceiver.SetOptions(0b01000100);
  Transceiver.SaveParameters(PERMANENT);
  
  Transceiver.PrintParameters();
}

void setup() 
{
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
  setup_e32();
}

void loop() 
{
  message.temperature = ds.getTempC();
  Transceiver.SendStruct(&message, sizeof(message));
  Serial.printf("Sending: %f\n\r", message.temperature);
  delay(300);
}
