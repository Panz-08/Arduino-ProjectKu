/*
 * Alarm Gerak PIR
 * ---------------
 * Sistem alarm sederhana memakai sensor gerak PIR (HC-SR501).
 *
 * Cara kerja:
 *   - Saat sistem AKTIF dan PIR mendeteksi gerakan, LED berkedip dan buzzer
 *     berbunyi putus-putus.
 *   - Alarm berhenti otomatis 3 detik setelah gerakan terakhir hilang.
 *   - Tombol pada D3 memfungsikan sistem (ARM / DISARM).
 *
 * Board  : Arduino Uno
 * Wokwi  : klik sensor PIR lalu "Simulate Motion" saat simulasi berjalan
 * Serial : 9600 baud
 */

#include <Arduino.h>

const uint8_t PIN_PIR    = 2;
const uint8_t PIN_TOMBOL = 3;
const uint8_t PIN_BUZZER = 9;
const uint8_t PIN_LED    = 13;

const unsigned long JEDA_KEDIP   = 120;
const unsigned long JEDA_BUNYI   = 250;
const unsigned long JEDA_SELESAI = 3000;

bool aktif = true;
bool alarm = false;
bool ledNyala = false;

unsigned long waktuKedip = 0;
unsigned long waktuBunyi = 0;
unsigned long terakhirGerak = 0;

bool tombolSebelumnya = HIGH;
unsigned long waktuTombol = 0;

void matikanAlarm() {
  alarm = false;
  ledNyala = false;
  digitalWrite(PIN_LED, LOW);
  noTone(PIN_BUZZER);
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
  pinMode(PIN_PIR, INPUT);
  pinMode(PIN_TOMBOL, INPUT_PULLUP);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_LED, OUTPUT);

  Serial.begin(9600);
  Serial.println();
  Serial.println("=== Alarm Gerak PIR ===");
  Serial.println("Sistem AKTIF. Tekan tombol untuk ARM/DISARM.");
}

void loop() {
  // ARM / DISARM
  if (tombolDitekan()) {
    aktif = !aktif;
    matikanAlarm();
    Serial.print("Sistem ");
    Serial.println(aktif ? "AKTIF (ARM)" : "NONAKTIF (DISARM)");
  }

  bool gerak = digitalRead(PIN_PIR) == HIGH;

  if (aktif && gerak) {
    terakhirGerak = millis();
    if (!alarm) {
      alarm = true;
      Serial.println("ALARM: gerakan terdeteksi!");
    }
  }

  if (alarm && (millis() - terakhirGerak > JEDA_SELESAI)) {
    matikanAlarm();
    Serial.println("Alarm berhenti: area kembali aman.");
  }

  if (alarm) {
    if (millis() - waktuKedip >= JEDA_KEDIP) {
      waktuKedip = millis();
      ledNyala = !ledNyala;
      digitalWrite(PIN_LED, ledNyala ? HIGH : LOW);
    }
    if (millis() - waktuBunyi >= JEDA_BUNYI) {
      waktuBunyi = millis();
      tone(PIN_BUZZER, 1800, 90);
    }
  }
}
