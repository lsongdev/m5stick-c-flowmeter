#include <M5StickC.h>

float flowRate = 0.0;
float totalLitres = 0.0;
unsigned long oldTime = 0;
const float calibrationFactor = 7.5;
const int flowSensorPin = 26; // G26

volatile int pulseCount = 0;

void IRAM_ATTR pulseCounter() {
  pulseCount++;
}

void setup() {
  M5.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(1);
  
  pinMode(flowSensorPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(flowSensorPin), pulseCounter, RISING);
  
  oldTime = millis();
}

void loop() {
  if((millis() - oldTime) >= 1000) {
    detachInterrupt(digitalPinToInterrupt(flowSensorPin));
    
    float timeInterval = millis() - oldTime;
    flowRate = ((1000.0 / timeInterval) * pulseCount) / calibrationFactor;
    totalLitres += (pulseCount / calibrationFactor);
    
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0, 10);
    M5.Lcd.printf("Flow Rate: %.2f L/min", flowRate);
    M5.Lcd.setCursor(0, 30);
    M5.Lcd.printf("Total: %.2f L", totalLitres);
    
    pulseCount = 0;
    oldTime = millis();
    
    attachInterrupt(digitalPinToInterrupt(flowSensorPin), pulseCounter, RISING);
  }
  
  M5.update();
  if(M5.BtnA.wasPressed()) {
    totalLitres = 0;
  }
}