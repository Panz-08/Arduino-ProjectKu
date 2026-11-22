/*
 * Lampu Lalu Lintas Adaptif
 * -------------------------
 * Simulasi lampu lalu lintas dengan tombol penyeberangan untuk pejalan kaki.
 *
 * Alur:
 *   HIJAU (5 detik) -> KUNING (2 detik) -> MERAH (5 detik, atau 8 detik
 *   bila ada permintaan pejalan kaki) -> kembali ke HIJAU.
 *
 * Saat fase MERAH dengan permintaan pejalan kaki, LED pejalan berkedip dan
 * buzzer berbunyi singkat sebagai isyarat aman menyeberang.
 *
 * Board  : Arduino Uno
 * Wokwi  : lihat diagram.json di folder ini
 * Serial : 9600 baud
 */

#include <Arduino.h>

// ---------------- Konfigurasi pin ----------------
const uint8_t PIN_LAMPU_MERAH   = 8;
const uint8_t PIN_LAMPU_KUNING  = 9;
const uint8_t PIN_LAMPU_HIJAU   = 10;
const uint8_t PIN_LAMPU_PEJALAN = 12;
const uint8_t PIN_TOMBOL        = 2;   // tombol ke GND, memakai INPUT_PULLUP
const uint8_t PIN_BUZZER        = 11;

// ---------------- Durasi tiap fase (ms) ----------------
const unsigned long DURASI_HIJAU          = 5000;
const unsigned long DURASI_KUNING         = 2000;
const unsigned long DURASI_MERAH          = 5000;
const unsigned long DURASI_MERAH_PEJALAN  = 8000;
const unsigned long JEDA_KEDIP            = 400;

enum Fase { FASE_HIJAU, FASE_KUNING, FASE_MERAH };

Fase fase = FASE_HIJAU;
unsigned long waktuFase = 0;
bool permintaanPejalan = false;

// Status kedip LED pejalan
bool ledPejalanNyala = false;
unsigned long waktuKedip = 0;

// Debounce tombol
bool tombolSebelumnya = HIGH;
unsigned long waktuTombol = 0;

// ---------------- Helper ----------------
void aturLampu(bool merah, bool kuning, bool hijau, bool pejalan) {
  digitalWrite(PIN_LAMPU_MERAH,   merah  ? HIGH : LOW);
  digitalWrite(PIN_LAMPU_KUNING,  kuning ? HIGH : LOW);
  digitalWrite(PIN_LAMPU_HIJAU,   hijau  ? HIGH : LOW);
  digitalWrite(PIN_LAMPU_PEJALAN, pejalan ? HIGH : LOW);
}

void masukFase(Fase baru) {
  fase = baru;
  waktuFase = millis();
  ledPejalanNyala = false;
  waktuKedip = millis();
  noTone(PIN_BUZZER);

  switch (fase) {
    case FASE_HIJAU:
      aturLampu(false, false, true, false);
      Serial.println("FASE: HIJAU - kendaraan boleh jalan");
      break;

    case FASE_KUNING:
      aturLampu(false, true, false, false);
      Serial.println("FASE: KUNING - bersiap berhenti");
      break;

    case FASE_MERAH:
      aturLampu(true, false, false, permintaanPejalan);
      if (permintaanPejalan) {
        Serial.println("FASE: MERAH - pejalan kaki boleh menyeberang");
      } else {
        Serial.println("FASE: MERAH - kendaraan berhenti");
      }
      break;
  }
}

// Mengembalikan true tepat satu kali saat tombol ditekan (active-low).
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

unsigned long durasiFaseSekarang() {
  switch (fase) {
    case FASE_HIJAU:  return DURASI_HIJAU;
    case FASE_KUNING: return DURASI_KUNING;
    case FASE_MERAH:  return permintaanPejalan ? DURASI_MERAH_PEJALAN : DURASI_MERAH;
  }
  return DURASI_MERAH;
}

// ---------------- Setup & Loop ----------------
void setup() {
  pinMode(PIN_LAMPU_MERAH, OUTPUT);
  pinMode(PIN_LAMPU_KUNING, OUTPUT);
  pinMode(PIN_LAMPU_HIJAU, OUTPUT);
  pinMode(PIN_LAMPU_PEJALAN, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_TOMBOL, INPUT_PULLUP);

  Serial.begin(9600);
  Serial.println();
  Serial.println("=== Lampu Lalu Lintas Adaptif ===");
  Serial.println("Tekan tombol untuk meminta penyeberangan.");

  masukFase(FASE_HIJAU);
}

void loop() {
  // Permintaan pejalan kaki
  if (tombolDitekan()) {
    permintaanPejalan = true;
    Serial.println("Permintaan penyeberangan diterima.");
  }

  // Perpindahan fase
  if (millis() - waktuFase >= durasiFaseSekarang()) {
    if (fase == FASE_HIJAU) {
      masukFase(FASE_KUNING);
    } else if (fase == FASE_KUNING) {
      masukFase(FASE_MERAH);
    } else {
      permintaanPejalan = false;   // permintaan dipakai sekali
      masukFase(FASE_HIJAU);
    }
  }

  // Kedip LED pejalan + bunyi isyarat selama fase MERAH
  if (fase == FASE_MERAH && permintaanPejalan) {
    if (millis() - waktuKedip >= JEDA_KEDIP) {
      waktuKedip = millis();
      ledPejalanNyala = !ledPejalanNyala;
      digitalWrite(PIN_LAMPU_PEJALAN, ledPejalanNyala ? HIGH : LOW);
      if (ledPejalanNyala) {
        tone(PIN_BUZZER, 1200, 80);
      }
    }
  }
}
