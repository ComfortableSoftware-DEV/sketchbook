#include <ArdusatSDK.h>

Display display;
Acceleration accel;

void setup() {
  accel.begin();
  
  display.begin();
  display.display();

  delay(1000);
}

int width = 30;
int height = 12;
int x1 = 0;
int y1 = 10;
int x2 = SSD1306_LCDWIDTH - width;
int y2 = 10;

float m1 = 1.0;
float m2 = 1.0;
float look_dir1 = 0;
//float look_dir2 = 3.1415;

void drawEye(int x, int y, int look_x = 0, int look_y = 0) {
  int cx = x+width/2, cy = y+height/2;

  // Draw some circles to make the eye
  for (int i = -3; i <= 3; i++) {
    int rx = cx-i*4;
    int ry = cy;
    int r = 6-abs(i);
    display.fillCircle(rx, ry, r, WHITE);
    display.drawLine(rx, ry - r - 2, rx-(i*2/3), ry - r - 4, WHITE);
  }

  // Draw a circle in the middle and remove the iris
  display.fillCircle(cx, cy, 6, WHITE);
  display.fillCircle(cx, cy, 5, BLACK);
  
  // Add a pupil, with slight offset to show "looking" in a direction
  display.fillCircle(cx + look_x, cy + look_y, 2, WHITE);
}

void loop() {
  accel.read();

  if (accel.x > 0) {
    m1 = 1.0;
    m2 = 0.5;
  } else {
    m1 = 0.5;
    m2 = 1.0;
  }

  x1 -= accel.x * m1;
  // Stop left eye at left edge of screen
  if (x1 < 0) x1 = 0;
  // Stop left eye at right edge of screen
  if (x1 > SSD1306_LCDWIDTH - width) x1 = SSD1306_LCDWIDTH - width;
  // Don't allow left eye to overlap right eye
  if (x1 > x2 - width) x1 = x2 - width;


  x2 -= accel.x * m2;
  // Stop right eye at left edge of screen
  if (x2 < 0) x2 = 0;
  // Stop right eye at right edge of screen
  if (x2 > SSD1306_LCDWIDTH - width) x2 = SSD1306_LCDWIDTH - width;
  // Don't allow right eye to overlap left eye
  if (x2 < x1 + width) x2 = x1 + width;

  look_dir1 = atan2(-accel.y, -accel.x);
  if (look_dir1 < 0) look_dir1 += 2 * M_PI;

  display.clearDisplay();
  drawEye(x1, y1, cos(look_dir1)*3.5, sin(look_dir1)*3.5);
  drawEye(x2, y2, cos(look_dir1)*3.5, sin(look_dir1)*3.5);
  display.display();
}

