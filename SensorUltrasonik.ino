#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myServo;

const int trigPin = 9;
const int echoPin = 8;
const int ledPin = 13;
const int servoPin = 10;

const int jarakAman = 15; 
long duration;
int distance;

int sudutSekarang = 15;
int arahPutaran = 3; 
unsigned long waktuSebelumnya = 0;
const int jedaServo = 30; 

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  
  myServo.attach(servoPin);
  myServo.write(sudutSekarang); // Set posisi awal
  
  lcd.init();
  lcd.backlight();
  
  lcd.setCursor(0, 0);
  lcd.print("Radar Cerdas");
  lcd.setCursor(0, 1);
  lcd.print("Sistem Siap...");
  delay(2000);
  lcd.clear();
}

void loop() {
  ambilJarak();

  if (distance <= jarakAman && distance > 0) {
    digitalWrite(ledPin, HIGH);
    
    lcd.setCursor(0, 0);
    lcd.print("TARGET TERKUNCI ");
    
    lcd.setCursor(0, 1);
    lcd.print("Jarak: ");
    lcd.print(distance);
    lcd.print(" cm   ");
    
  } else {
    digitalWrite(ledPin, LOW);
    
    lcd.setCursor(0, 0);
    lcd.print("Memindai...     ");
    
    lcd.setCursor(0, 1);
    lcd.print("Sudut: ");
    lcd.print(sudutSekarang);
    lcd.print((char)223);
    lcd.print("    ");
    
    unsigned long waktuSekarang = millis();
    if (waktuSekarang - waktuSebelumnya >= jedaServo) {
      waktuSebelumnya = waktuSekarang;
      
      sudutSekarang += arahPutaran;
      
      if (sudutSekarang >= 165) {
        sudutSekarang = 165;
        arahPutaran = -3;
      }
      else if (sudutSekarang <= 15) {
        sudutSekarang = 15;
        arahPutaran = 3;
      }
      
      myServo.write(sudutSekarang);
    }
  }
  
  delay(10);
}

void ambilJarak() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH, 30000);
  distance = duration * 0.034 / 2;
}
