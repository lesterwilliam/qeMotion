#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include <Keyboard.h>

#define LED_Q 9
#define LED_E 10
#define LED_STATUS_1 7
#define LED_STATUS_2 6
#define LED_STATUS_3 5
#define LED_STATUS_4 4
#define BUTTON_L 14
#define BUTTON_R 15

#define TRACKER_INTERVAL 10
#define KEYBOARD_INTERVAL 10
#define LED_INTERVAL 100
#define BUTTON_INTERVAL 10
#define TEST_INTERVAL 1000
#define TRACKING_DURATION 500
#define TRACKING_TICKS (TRACKING_DURATION / TRACKER_INTERVAL)

unsigned long currentMillis = 0;
unsigned long tracker_previousMillis = 0;
unsigned long keyboard_previousMillis = 0;
unsigned long led_previousMillis = 0;
unsigned long button_previousMillis = 0;
unsigned long test_previousMillis = 0;

unsigned char button_l_flag = 0;
unsigned char button_r_flag = 0;

unsigned char keyboard_hold_q = 0;
unsigned char keyboard_hold_e = 0;

unsigned char mode = 0; // 0 = disabled; 1 = qeMotion

MPU6050 mpu;
int16_t ax, ay, az, gx, gy, gz;
int vx[TRACKING_TICKS] = {0};
int vy[TRACKING_TICKS] = {0};
int vz[TRACKING_TICKS] = {0};
int trackerCounter = 0;

void hardwareTest(){
  digitalWrite(LED_E, HIGH);
  digitalWrite(LED_Q, LOW);
  delay(1000);
  digitalWrite(LED_Q, HIGH);
  digitalWrite(LED_E, LOW);
  delay(1000);
  digitalWrite(LED_E, HIGH);

  for(int i = 0; i < 10; i++){
    digitalWrite(LED_STATUS_1, HIGH);
    delay(50);
    digitalWrite(LED_STATUS_1, LOW);
    digitalWrite(LED_STATUS_2, HIGH);
    delay(50);
    digitalWrite(LED_STATUS_2, LOW);
    digitalWrite(LED_STATUS_3, HIGH);
    delay(50);
    digitalWrite(LED_STATUS_3, LOW);
    digitalWrite(LED_STATUS_4, HIGH);
    delay(50);
    digitalWrite(LED_STATUS_4, LOW);
    digitalWrite(LED_STATUS_3, HIGH);
    delay(50);
    digitalWrite(LED_STATUS_3, LOW);
    digitalWrite(LED_STATUS_2, HIGH);
    delay(50);
    digitalWrite(LED_STATUS_2, LOW);
  }
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  pinMode(LED_STATUS_4, OUTPUT);
  pinMode(LED_STATUS_3, OUTPUT);
  pinMode(LED_STATUS_2, OUTPUT);
  pinMode(LED_STATUS_1, OUTPUT);
  pinMode(LED_Q, OUTPUT);
  pinMode(LED_E, OUTPUT);
  pinMode(14, INPUT_PULLUP);
  pinMode(15, INPUT_PULLUP);
  
  digitalWrite(LED_Q, HIGH);
  digitalWrite(LED_E, HIGH);
  //hardwareTest();
  
  mpu.initialize();
  if (!mpu.testConnection()) { while (1); }
  
  Keyboard.begin();
  delay(3000);
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
  if(keyboard_hold_q){
    Keyboard.press('q');
  }else{
    Keyboard.release('q');
  }

  if(keyboard_hold_e){
    Keyboard.press('e');
  }else{
    Keyboard.release('e');
  }
}

void led(){
  switch(mode){
    case 0:
      digitalWrite(LED_STATUS_1, LOW);
      digitalWrite(LED_STATUS_2, LOW);
      digitalWrite(LED_STATUS_3, LOW);
      digitalWrite(LED_STATUS_4, LOW);
      break;
      
    case 1:
      digitalWrite(LED_STATUS_1, HIGH);
      digitalWrite(LED_STATUS_2, HIGH);
      digitalWrite(LED_STATUS_3, LOW);
      digitalWrite(LED_STATUS_4, LOW);
      break;
      
    case 2:
      digitalWrite(LED_STATUS_1, LOW);
      digitalWrite(LED_STATUS_2, LOW);
      digitalWrite(LED_STATUS_3, HIGH);
      digitalWrite(LED_STATUS_4, HIGH);
      break;

    default:
      break;
  }

  if(keyboard_hold_q){
    digitalWrite(LED_Q, LOW);
  }else{
    digitalWrite(LED_Q, HIGH);
  }
  
  if(keyboard_hold_e){
    digitalWrite(LED_E, LOW);
  }else{
    digitalWrite(LED_E, HIGH);
  }
}

void button(){
  if(!digitalRead(BUTTON_L) && !button_l_flag){
    button_l_flag = 1;
    if(mode <= 1){
      mode++;
    }else{
      mode = 0;
    }
  }
  if(digitalRead(BUTTON_L)){
    button_l_flag = 0;
  }
  
  if(!digitalRead(BUTTON_R) && !button_r_flag){
    button_r_flag = 1;
    // insert what button does
  }
  if(digitalRead(BUTTON_R)){
    button_r_flag = 0;
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
  
  if(currentMillis - button_previousMillis >= BUTTON_INTERVAL){
    button_previousMillis = currentMillis;
    button();
  }
  
  if(currentMillis - test_previousMillis >= TEST_INTERVAL){
    test_previousMillis = currentMillis;
  }
}
