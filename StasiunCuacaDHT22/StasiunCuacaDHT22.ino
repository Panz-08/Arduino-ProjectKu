/*
 * Stasiun Cuaca DHT22
 * -------------------
 * Memantau suhu dan kelembapan dengan sensor DHT22, menampilkannya di LCD,
 * dan memberi peringatan saat suhu terlalu panas.
 *
 * Fitur:
 *   - Pembacaan suhu & kelembapan tiap 2 detik.
 *   - Tampilan Celsius / Fahrenheit (tombol D9 untuk ganti satuan).
 *   - LED hijau = normal, LED merah + buzzer = suhu >= 35 derajat Celsius.
 *
 * Board  : Arduino Uno
 * Wokwi  : klik sensor DHT22 untuk mengubah nilai suhu/kelembapan
 * Serial : 9600 baud
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define PIN_DHT   2
#define TIPE_DHT  DHT22

DHT dht(PIN_DHT, TIPE_DHT);
LiquidCrystal_I2C lcd(0x27, 16, 2);

const uint8_t PIN_LED_NORMAL = 6;
const uint8_t PIN_LED_PANAS  = 7;
const uint8_t PIN_BUZZER     = 8;
const uint8_t PIN_TOMBOL     = 9;

const float BATAS_PANAS = 35.0;
const unsigned long JEDA_BACA  = 2000;
const unsigned long JEDA_BUNYI = 1000;

bool fahrenheit = false;
bool panasSebelumnya = false;
float suhu = NAN;
float kelembapan = NAN;

unsigned long waktuBaca = 0;
unsigned long waktuBunyi = 0;

bool tombolSebelumnya = HIGH;
unsigned long waktuTombol = 0;

void tampil() {
  char baris0[17];
  char baris1[17];

  if (isnan(suhu) || isnan(kelembapan)) {
    snprintf(baris0, sizeof(baris0), "%-16s", "Sensor error");
    snprintf(baris1, sizeof(baris1), "%-16s", "Cek DHT22");
  } else {
    float nilai = fahrenheit ? (suhu * 9.0 / 5.0 + 32.0) : suhu;
    char satuan = fahrenheit ? 'F' : 'C';

    if (suhu >= BATAS_PANAS) {
      snprintf(baris0, sizeof(baris0), "PANAS! %4.1f %c%c", nilai, (char)223, satuan);
    } else {
      snprintf(baris0, sizeof(baris0), "Suhu: %5.1f %c%c", nilai, (char)223, satuan);
    }
    snprintf(baris1, sizeof(baris1), "Lembap: %4.1f %%", kelembapan);
  }

  lcd.setCursor(0, 0);
  lcd.print(baris0);
  lcd.setCursor(0, 1);
  lcd.print(baris1);
}

bool tombolDitekan() {
  bool sekarang = digitalRead(PIN_TOMBOL);
  if (sekarang != tombolSebelumnya && (millis() - waktuTombol) > 50) {
    waktuTombol = millis();
    tombolSebelumnya = sekarang;
    if (sekarang == LOW) {
      return true;
    }
  }
  return false;
}

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println("=== Stasiun Cuaca DHT22 ===");

  pinMode(PIN_LED_NORMAL, OUTPUT);
  pinMode(PIN_LED_PANAS, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_TOMBOL, INPUT_PULLUP);

  dht.begin();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Stasiun Cuaca");
  lcd.setCursor(0, 1);
  lcd.print("Membaca sensor");
  delay(1500);
}

void loop() {
  if (tombolDitekan()) {
    fahrenheit = !fahrenheit;
    Serial.print("Satuan: ");
    Serial.println(fahrenheit ? "Fahrenheit" : "Celsius");
    tampil();
  }

  if (millis() - waktuBaca >= JEDA_BACA) {
    waktuBaca = millis();

    float t = dht.readTemperature();
    float h = dht.readHumidity();

    if (isnan(t) || isnan(h)) {
      suhu = NAN;
      kelembapan = NAN;
      Serial.println("Gagal membaca DHT22.");
    } else {
      suhu = t;
      kelembapan = h;
      Serial.print("Suhu=");
      Serial.print(suhu, 1);
      Serial.print(" C, Kelembapan=");
      Serial.print(kelembapan, 1);
      Serial.println(" %");
    }

    tampil();
  }

  bool panas = !isnan(suhu) && (suhu >= BATAS_PANAS);

  if (panas) {
    digitalWrite(PIN_LED_PANAS, HIGH);
    digitalWrite(PIN_LED_NORMAL, LOW);
    if (millis() - waktuBunyi >= JEDA_BUNYI) {
      waktuBunyi = millis();
      tone(PIN_BUZZER, 2200, 100);
    }
    if (!panasSebelumnya) {
      Serial.println("PERINGATAN: suhu di atas batas 35 C!");
    }
  } else {
    digitalWrite(PIN_LED_PANAS, LOW);
    digitalWrite(PIN_LED_NORMAL, HIGH);
    noTone(PIN_BUZZER);
    if (panasSebelumnya) {
      Serial.println("Suhu kembali normal.");
    }
  }

  panasSebelumnya = panas;
}
