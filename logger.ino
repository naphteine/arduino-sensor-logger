// Ultrasonic
int pinTrigger = 5;
int pinEcho = 6;
int rtime = 0;
int distance = 0;

// DHT11
#include "DHT.h"
DHT dht1(2, DHT11); // dht(pin, dht_type)
DHT dht2(3, DHT11); // dht(pin, dht_type)

float h, t, hic;

// RGB led
int red_pin = 11;
int green_pin = 10;
int blue_pin = 9;

int redV = 0;
int greenV = 0;
int blueV = 0;

void RGB_color(int red, int green, int blue) {
  analogWrite(red_pin, 255 - red);
  analogWrite(green_pin, 255 - green);
  analogWrite(blue_pin, 255 - blue);
}

// Light sensor
int light = 0;

// IR sensor
int ir_pin = 4;
int ir_status = 0;

// Timer
int timer = 0;

// Main functions
void setup() {
  // Serial
  Serial.begin(9600);
  
  // Ultrasonic
  pinMode(pinTrigger, OUTPUT);
  pinMode(pinEcho, INPUT);
  
  // DHT11
  dht1.begin();
  dht2.begin();

  // RGB led
  pinMode(red_pin, OUTPUT);
  pinMode(green_pin, OUTPUT);
  pinMode(blue_pin, OUTPUT);

  // IR sensor
  pinMode(ir_pin, INPUT);
}

void loop() {
  // Light sensor
  light = analogRead(A0);
  
  // Ultrasonic
  digitalWrite(pinTrigger, LOW);
  delayMicroseconds(2);
  digitalWrite(pinTrigger, HIGH);
  delayMicroseconds(2);
  digitalWrite(pinTrigger, LOW);
  rtime = pulseIn(pinEcho, HIGH);
  distance = (0.0344 * rtime) / 2;

  // IR sensor
  ir_status = digitalRead(ir_pin);

  // RGB led
  if (distance <= 30) {
    redV = map(distance, 0, 30, 255, 0);
    greenV = map(distance, 0, 30, 0, 255);
    blueV = greenV;
  } else if (distance <= 60) {
    redV = 0;
    greenV = map(distance, 30, 60, 255, 0);
    blueV = greenV;
  } else {
    redV = 0;
    greenV = 0;
    blueV = 0;
  }

  RGB_color(redV, greenV, blueV);
  
  if(timer >= 20) {
    timer = 0;

    // Light sensor
    Serial.print("LIG1 ");
    Serial.println(light);
    
    // Ultrasonic
    Serial.print("DIS1 ");
    Serial.println(distance);

    // IR sensor
    /*
    Serial.print("IRS1 ");
    Serial.println(ir_status);
    */
  
    // DHT11 - SENSOR 1
    // Reading temperature or humidity takes about 250 milliseconds!
    h = dht1.readHumidity(); // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    t = dht1.readTemperature(); // Read temperature as Celsius (the default)
    
    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t)) {
      Serial.println(F("DHT1 FAIL"));
      return;
    }
    
    // Compute heat index in Celsius (isFahreheit = false)
    hic = dht1.computeHeatIndex(t, h, false);
    
    Serial.print(F("DHT1 H"));
    Serial.print(h);
    Serial.print(F(" C"));
    Serial.print(t);
    Serial.print(F(" I"));
    Serial.println(hic);

    // DHT11 - SENSOR 2
    // Reading temperature or humidity takes about 250 milliseconds!
    h = dht2.readHumidity(); // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    t = dht2.readTemperature(); // Read temperature as Celsius (the default)
    
    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t)) {
      Serial.println(F("DHT2 FAIL"));
      return;
    }
    
    // Compute heat index in Celsius (isFahreheit = false)
    float hic = dht2.computeHeatIndex(t, h, false);
    
    Serial.print(F("DHT2 H"));
    Serial.print(h);
    Serial.print(F(" C"));
    Serial.print(t);
    Serial.print(F(" I"));
    Serial.println(hic);
  }

  // Timer
  delay(100);
  timer++;
}
