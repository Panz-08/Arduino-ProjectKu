/*
 * Kunci Pintar Keypad
 * -------------------
 * Kunci pintu digital memakai keypad 4x4, servo, LCD I2C, dan buzzer.
 *
 * Aturan:
 *   - PIN yang benar adalah "1234" lalu tekan '#'.
 *   - Tekan '*' untuk menghapus input.
 *   - Tiga kali salah berturut-turut -> sistem terkunci 10 detik.
 *   - Servo membuka kunci selama 5 detik lalu menutup kembali.
 *
 * Board  : Arduino Uno
 * Wokwi  : keypad 4x4 (wokwi-membrane-keypad)
 * Serial : 9600 baud
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo kunci;

const uint8_t PIN_SERVO      = 10;
const uint8_t PIN_BUZZER     = 11;
const uint8_t PIN_LED_HIJAU  = 12;
const uint8_t PIN_LED_MERAH  = 13;

const char PIN_BENAR[] = "1234";
const uint8_t PANJANG_PIN = 4;

char keys[4][4] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

byte rowPins[4] = { 9, 8, 7, 6 };
byte colPins[4] = { 5, 4, 3, 2 };

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, 4, 4);

char input[PANJANG_PIN + 1];
uint8_t panjangInput = 0;
uint8_t salah = 0;

bool terbuka = false;
bool terkunci = false;
unsigned long waktuBuka = 0;
unsigned long waktuKunci = 0;

const unsigned long DURASI_BUKA    = 5000;
const unsigned long DURASI_TERKUNCI = 10000;

void tampil(const char *baris0, const char *baris1) {
  char b0[17];
  char b1[17];
  snprintf(b0, sizeof(b0), "%-16s", baris0);
  snprintf(b1, sizeof(b1), "%-16s", baris1);
  lcd.setCursor(0, 0);
  lcd.print(b0);
  lcd.setCursor(0, 1);
  lcd.print(b1);
}

void tampilInput() {
  char mask[PANJANG_PIN + 1];
  for (uint8_t i = 0; i < panjangInput; i++) {
    mask[i] = '*';
  }
  mask[panjangInput] = '\0';
  tampil("Masukkan PIN:", mask);
}

void resetInput() {
  panjangInput = 0;
  input[0] = '\0';
}

void bukaKunci() {
  terbuka = true;
  waktuBuka = millis();
  kunci.write(90);
  digitalWrite(PIN_LED_HIJAU, HIGH);
  digitalWrite(PIN_LED_MERAH, LOW);
  tone(PIN_BUZZER, 1800, 150);
  tampil("AKSES DIBERIKAN", "Pintu terbuka");
  Serial.println("AKSES DIBERIKAN - pintu terbuka.");
  resetInput();
}

void tutupKunci() {
  terbuka = false;
  kunci.write(0);
  digitalWrite(PIN_LED_HIJAU, LOW);
  digitalWrite(PIN_LED_MERAH, HIGH);
  tampil("Pintu terkunci", "Masukkan PIN:");
  Serial.println("Pintu dikunci kembali.");
}

void pinSalah() {
  salah++;
  digitalWrite(PIN_LED_MERAH, HIGH);
  digitalWrite(PIN_LED_HIJAU, LOW);
  tone(PIN_BUZZER, 400, 300);
  resetInput();

  Serial.print("PIN salah (");
  Serial.print(salah);
  Serial.println("/3).");

  if (salah >= 3) {
    terkunci = true;
    waktuKunci = millis();
    tampil("TERKUNCI 10 DETIK", "Tunggu sebentar");
    Serial.println("Sistem terkunci 10 detik.");
  } else {
    tampil("PIN SALAH", "Coba lagi");
  }
}

void prosesTombol(char tombol) {
  if (terkunci || terbuka) {
    return;
  }

  if (tombol == '*') {
    resetInput();
    tampilInput();
    tone(PIN_BUZZER, 800, 60);
    Serial.println("Input dihapus.");
    return;
  }

  if (tombol == '#') {
    if (panjangInput == 0) {
      tone(PIN_BUZZER, 300, 100);
      return;
    }
    if (strcmp(input, PIN_BENAR) == 0) {
      bukaKunci();
    } else {
      pinSalah();
    }
    return;
  }

  // Hanya terima angka 0-9
  if (tombol >= '0' && tombol <= '9') {
    if (panjangInput < PANJANG_PIN) {
      input[panjangInput++] = tombol;
      input[panjangInput] = '\0';
      tone(PIN_BUZZER, 1000, 50);
      tampilInput();
      Serial.print("Input: ");
      Serial.println(input);
    }
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println("=== Kunci Pintar Keypad ===");
  Serial.println("PIN default: 1234 lalu tekan #.");
  Serial.println("Bisa juga mengetik PIN di Serial Monitor untuk pengujian.");

  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_LED_HIJAU, OUTPUT);
  pinMode(PIN_LED_MERAH, OUTPUT);

  kunci.attach(PIN_SERVO);
  kunci.write(0);
  digitalWrite(PIN_LED_HIJAU, LOW);
  digitalWrite(PIN_LED_MERAH, HIGH);

  lcd.init();
  lcd.backlight();
  tampil("KUNCI PINTAR", "Sistem siap...");
  delay(1200);
  tampilInput();
}

void loop() {
  // Buka kunci sementara
  if (terbuka && (millis() - waktuBuka >= DURASI_BUKA)) {
    tutupKunci();
  }

  // Akhir masa lockout
  if (terkunci && (millis() - waktuKunci >= DURASI_TERKUNCI)) {
    terkunci = false;
    salah = 0;
    tampil("Masukkan PIN:", "");
    Serial.println("Lockout selesai. Silakan coba lagi.");
  }

  if (terkunci || terbuka) {
    return;
  }

  char tombol = keypad.getKey();
  if (tombol) {
    prosesTombol(tombol);
  }

  while (Serial.available() > 0) {
    prosesTombol((char)Serial.read());
  }
}
