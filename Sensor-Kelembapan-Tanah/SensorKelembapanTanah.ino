#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int pinSensor = A0;
const int pinLED = 13;
const int batasAtas = 50; 

int nilaiMentah = 0;
int persentaseKelembaban = 0;

void setup() {
  Serial.begin(9600);
  pinMode(pinLED, OUTPUT);
  
  lcd.init();         
  lcd.clear();        
  lcd.backlight();    
  
  lcd.setCursor(0, 0);
  lcd.print("  MONITOR TANAH  ");
  lcd.setCursor(0, 1);
  lcd.print("  BATAS NYALA 30% ");
  delay(2000); 
  lcd.clear();
}

void loop() {
  nilaiMentah = analogRead(pinSensor);
  
  persentaseKelembaban = map(nilaiMentah, 1023, 200, 0, 100);
  
  if(persentaseKelembaban > 100) persentaseKelembaban = 100;
  if(persentaseKelembaban < 0)   persentaseKelembaban = 0;

  lcd.setCursor(0, 0);
  lcd.print("Kelembaban: ");
  lcd.print(persentaseKelembaban);
  lcd.print("%   ");
  lcd.setCursor(0, 1);
  if (persentaseKelembaban > batasAtas) {
    lcd.print("LED: NYALA      ");
    digitalWrite(pinLED, HIGH);
  } 
  else {
    lcd.print("LED: MATI       ");
    digitalWrite(pinLED, LOW);
  }

  Serial.print("Kelembaban: ");
  Serial.print(persentaseKelembaban);
  Serial.println(persentaseKelembaban > batasAtas ? "% -> LED NYALA" : "% -> LED MATI");

  delay(1000);
}
