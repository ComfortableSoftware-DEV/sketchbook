#include <SPI.h>

#define CS_PIN 10 // 15 or Use 10 when compiling for Arduino

SPISettings MCP3008(2000000, MSBFIRST, SPI_MODE0);

int adcRead(int channel) 
{
  SPI.beginTransaction(MCP3008);
  digitalWrite(CS_PIN, LOW);
  SPI.transfer(1);
  uint8_t r1 = SPI.transfer((channel + 8) << 4);
  uint8_t r2 = SPI.transfer(0);
  digitalWrite(CS_PIN, HIGH);
  SPI.endTransaction();
  return ((r1 & 3) << 8) | r2;
}

void setup() {
  pinMode(CS_PIN, OUTPUT);
  pinMode(A0, INPUT);
  SPI.begin();
  Serial.begin(115200);
  digitalWrite(CS_PIN, LOW);
  digitalWrite(CS_PIN, HIGH);
}

void loop() {
  Serial.print(adcRead(0));
  Serial.print("  ");
  //Serial.print(adcRead(1));
  //Serial.print("  ");
  //Serial.print(adcRead(2));
  //Serial.print("  ");
  Serial.println(analogRead(A0));
  //delay(50);
}
