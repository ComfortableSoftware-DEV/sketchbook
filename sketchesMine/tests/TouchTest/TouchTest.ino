#include <XPT2046_Touchscreen.h>
#include <SPI.h>

#define CS_PIN 8
int minx = 1000;
int miny = 1000;
int maxx = 1000;
int maxy = 1000;

XPT2046_Touchscreen ts(CS_PIN);
#define TIRQ_PIN  2
//XPT2046_Touchscreen ts(CS_PIN);  // Param 2 - NULL - No interrupts
//XPT2046_Touchscreen ts(CS_PIN, 255);  // Param 2 - 255 - No interrupts
//XPT2046_Touchscreen ts(CS_PIN, TIRQ_PIN);  // Param 2 - Touch IRQ Pin - interrupt enabled polling

void setup() {
  Serial.begin(38400);
  ts.begin();
  while (!Serial && (millis() <= 1000));
}

void loopB() {
  TS_Point p = ts.getPoint();
  Serial.print("Pressure = ");
  Serial.print(p.z);
  if (ts.touched()) {
    Serial.print(", x = ");
    Serial.print(p.x);
    Serial.print(", y = ");
    Serial.print(p.y);
  }
  Serial.println();
  //  delay(100);
  delay(30);
}

void loop() {
  if (ts.touched()) {
    TS_Point p = ts.getPoint();
    if (p.x > maxx)
    {
      maxx = p.x;
    }
    if (p.y > maxy)
    {
      maxy = p.y;
    }
    if (p.x < minx)
    {
      minx = p.x;
    }
    if (p.y < miny)
    {
      miny = p.y;
    }
    Serial.print("Pressure = ");
   
    Serial.print(p.z);
    Serial.print(", x = ");
    Serial.print(p.x);
    Serial.print(", y = ");
    Serial.println(p.y);
    Serial.print("maxx = ");
    Serial.print(maxx);
    Serial.print(" minx = ");
    Serial.print(minx);
    Serial.print("  maxy = ");
    Serial.print(maxy);
    Serial.print(" miny = ");
    Serial.print(miny);
    delay(30);
    Serial.println();
  }
}
