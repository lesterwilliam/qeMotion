#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include <Keyboard.h>

#define TRACKER_INTERVAL 10
#define KEYBOARD_INTERVAL 10
#define LED_INTERVAL 10
#define TRACKING_DURATION 500
#define TRACKING_TICKS (TRACKING_DURATION / TRACKER_INTERVAL)

unsigned long currentMillis = 0;

unsigned long tracker_previousMillis = 0;
unsigned long keyboard_previousMillis = 0;
unsigned long led_previousMillis = 0;

MPU6050 mpu;
int16_t ax, ay, az, gx, gy, gz;
int vx[TRACKING_TICKS] = {0};
int vy[TRACKING_TICKS] = {0};
int vz[TRACKING_TICKS] = {0};
int trackerCounter = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();
  if (!mpu.testConnection()) { while (1); }
  Keyboard.begin();
  delay(5000);
}

void tracker(){
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  vx[trackerCounter] =  (gx + 1800) / 150;  // "+300" because the x axis of gyroscope give values about -350 while it's not moving. Change this value if you get something different using the TEST code, chacking if there are values far from zero.
  vy[trackerCounter] = -(gy -   30) / 150; // same here about "-100"
  vz[trackerCounter] = -(gz -   30) / 150; // same here about "-100"

  /*Serial.print("gx = ");
  Serial.print(gx);
  Serial.print("gy = ");
  Serial.print(gy);
  Serial.print(" | gz = ");
  Serial.print(gz);
  */
  Serial.print("        | X = ");
  Serial.print(vx[trackerCounter]);
  Serial.print(" | Y = ");
  Serial.print(vy[trackerCounter]);
  Serial.print(" | Z = ");
  Serial.println(vz[trackerCounter]);

  if(trackerCounter >= (TRACKING_TICKS -1)){
    trackerCounter = 0;
  }else{
    trackerCounter++;
  }
}

void keyboard(){
}

void led(){
}

}
void loop() {
  currentMillis = millis();
  if (currentMillis - tracker_previousMillis >= TRACKER_INTERVAL) {
    tracker_previousMillis = currentMillis;
    tracker();
  }
  if (currentMillis - keyboard_previousMillis >= KEYBOARD_INTERVAL) {
    keyboard_previousMillis = currentMillis;
    keyboard();
  }
  if (currentMillis - led_previousMillis >= LED_INTERVAL) {
    led_previousMillis = currentMillis;
    led();
  }
  
  //Keyboard.print("test");
  //delay(1000);
  //Keyboard.press('e');
  //delay(2000);
  //Keyboard.releaseAll();
  //delay(1000000);
}
