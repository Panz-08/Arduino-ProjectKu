/*
 * Parkir Otomatis
 * ----------------
 * Palang parkir otomatis memakai sensor ultrasonik HC-SR04 dan servo.
 *
 * Cara kerja:
 *   - Jika jarak kendaraan <= 20 cm, palang terbuka (servo 90 derajat),
 *     LED hijau menyala, dan buzzer berbunyi sekali.
 *   - Palang tetap terbuka selama kendaraan berada dekat sensor.
 *   - Tiga detik setelah kendaraan menjauh, palang menutup kembali.
 *
 * Board  : Arduino Uno
 * Wokwi  : ubah nilai jarak dengan mengklik sensor HC-SR04 saat simulasi
 * Serial : 9600 baud
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo palang;

const uint8_t PIN_TRIG       = 9;
const uint8_t PIN_ECHO       = 8;
const uint8_t PIN_SERVO      = 10;
const uint8_t PIN_LED_HIJAU  = 6;
const uint8_t PIN_LED_MERAH  = 7;
const uint8_t PIN_BUZZER     = 5;

const int JARAK_BUKA_CM      = 20;
const unsigned long TAHAN_BUKA_MS = 3000;
const unsigned long JEDA_LCD_MS   = 250;
const unsigned long JEDA_SERIAL_MS = 1000;

bool terbuka = false;
int jarakTerakhir = 400;
unsigned long waktuObjekTerakhir = 0;
unsigned long waktuLcd = 0;
unsigned long waktuSerial = 0;

int ukurJarak() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  unsigned long durasi = pulseIn(PIN_ECHO, HIGH, 30000UL);
  if (durasi == 0) {
    return 400;                       // tidak ada pantulan
  }
  return (int)(durasi * 0.034 / 2);   // konversi ke centimeter
}

void tampilLcd() {
  char baris0[17];
  char baris1[17];

  snprintf(baris0, sizeof(baris0), "%-16s",
           terbuka ? "PINTU TERBUKA" : "PARKIR OTOMATIS");
  snprintf(baris1, sizeof(baris1), "Jarak: %3d cm   ", jarakTerakhir);

  lcd.setCursor(0, 0);
  lcd.print(baris0);
  lcd.setCursor(0, 1);
  lcd.print(baris1);
}

void bukaPalang() {
  terbuka = true;
  palang.write(90);
  digitalWrite(PIN_LED_HIJAU, HIGH);
  digitalWrite(PIN_LED_MERAH, LOW);
  tone(PIN_BUZZER, 1200, 120);
  Serial.println("Palang DIBUKA - silakan masuk.");
  tampilLcd();
}

void tutupPalang() {
  terbuka = false;
  palang.write(0);
  digitalWrite(PIN_LED_HIJAU, LOW);
  digitalWrite(PIN_LED_MERAH, HIGH);
  tone(PIN_BUZZER, 600, 120);
  Serial.println("Palang DITUTUP - area aman.");
  tampilLcd();
}

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println("=== Parkir Otomatis ===");
  Serial.println("Palang tertutup. Menunggu kendaraan...");

  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  pinMode(PIN_LED_HIJAU, OUTPUT);
  pinMode(PIN_LED_MERAH, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  palang.attach(PIN_SERVO);
  palang.write(0);                    // posisi awal: tertutup

  digitalWrite(PIN_LED_HIJAU, LOW);
  digitalWrite(PIN_LED_MERAH, HIGH);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Parkir Otomatis");
  lcd.setCursor(0, 1);
  lcd.print("Sistem Siap...");
  delay(1500);
}

void loop() {
  jarakTerakhir = ukurJarak();

  if (jarakTerakhir > 0 && jarakTerakhir <= JARAK_BUKA_CM) {
    waktuObjekTerakhir = millis();
    if (!terbuka) {
      bukaPalang();
    }
  } else if (terbuka && (millis() - waktuObjekTerakhir >= TAHAN_BUKA_MS)) {
    tutupPalang();
  }

  if (millis() - waktuLcd >= JEDA_LCD_MS) {
    waktuLcd = millis();
    tampilLcd();
  }

  if (millis() - waktuSerial >= JEDA_SERIAL_MS) {
    waktuSerial = millis();
    Serial.print("Jarak: ");
    Serial.print(jarakTerakhir);
    Serial.print(" cm | Palang: ");
    Serial.println(terbuka ? "TERBUKA" : "TERTUTUP");
  }

  delay(50);
}
