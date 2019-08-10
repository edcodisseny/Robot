#include <RobotController.h>
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup()
{
  RobotBegin();
  Serial.begin(9600);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
    Serial.println(F("SSD1306 allocation failed"));
  }
  calibrateAccelGyros();

}

#define _GYROS_ERROR 5

void printStateGyros(int16_t x, int16_t y, int16_t z){
  // Clear the buffer
  display.clearDisplay();  
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(0, 16);
  display.print(F("GX:"));
  if(x>_GYROS_ERROR){
    display.println("ATRAS");
  }else if(x<-_GYROS_ERROR){
    display.println("AVANZ.");
  }else{
    display.println("PARADO");
  }
  
  display.print(F("GY: "));
  if(y>_GYROS_ERROR){
    display.println("IZQ");
  }else if(y<-_GYROS_ERROR){
    display.println("DER");
  }else{
    display.println("RECT");
  }
  
  display.print(F("GZ: "));
  if(z>_GYROS_ERROR){
    display.println("BAJA");
  }else if(z<-_GYROS_ERROR){
    display.println("SUBE");
  }else{
    display.println("=");
  }
  
  display.display();
  //doDelay(10);
}

void loop()
{
  int16_t gx,gy,gz;
  getGyros(&gx,&gy,&gz);
  printStateGyros(gx,gy,gz);
}
