# Kunci Pintar Keypad

Kunci pintu digital berbasis **keypad 4×4**, servo, LCD I2C, dan buzzer. PIN default
adalah `1234`. Proyek ini melatih pembacaan keypad, validasi input, penguncian
sementara (lockout), dan kontrol servo.

## Fitur

- Input PIN 4 digit dengan masking `****` pada LCD.
- Tekan `*` untuk menghapus input, `#` untuk memproses PIN.
- PIN benar → servo membuka kunci selama 5 detik, LED hijau menyala.
- PIN salah → buzzer berbunyi, LED merah menyala.
- Tiga kali salah berturut-turut → sistem terkunci 10 detik.
- Semua aktivitas dicatat di Serial Monitor.

## Komponen

| Komponen | Jumlah | Keterangan |
| --- | --- | --- |
| Arduino Uno | 1 | Mikrokontroler |
| Keypad membran 4×4 | 1 | Input PIN |
| Servo SG90 | 1 | Penggerak kunci |
| LCD I2C 16×2 | 1 | Alamat `0x27` |
| LED hijau & merah | 2 | Indikator status |
| Resistor 220 Ω | 2 | Pembatas arus LED |
| Buzzer | 1 | Indikator suara |
| Breadboard + jumper | — | Koneksi |

## Skema koneksi

| Keypad | Arduino |
| --- | --- |
| R1 | D9 |
| R2 | D8 |
| R3 | D7 |
| R4 | D6 |
| C1 | D5 |
| C2 | D4 |
| C3 | D3 |
| C4 | D2 |

| Komponen | Pin | Arduino |
| --- | --- | --- |
| Servo | PWM | D10 |
| Servo | V+ | 5V |
| Servo | GND | GND |
| LCD | SDA | A4 |
| LCD | SCL | A5 |
| LCD | VCC / GND | 5V / GND |
| Buzzer | Positif | D11 |
| Buzzer | Negatif | GND |
| LED hijau | Anoda | D12 via 220 Ω |
| LED merah | Anoda | D13 via 220 Ω |

## Library

- `Wire.h`, `Servo.h` — bawaan Arduino.
- `LiquidCrystal_I2C.h` — Library Manager.
- `Keypad.h` — Library Manager (Keypad by Mark Stanley & Alexander Brevig).

## Cara kerja

1. `setup()` menampilkan pesan pembuka, mengunci servo pada 0°, dan menyalakan LED merah.
2. `loop()` membaca keypad hanya saat pintu terkunci dan tidak dalam masa lockout.
3. Setiap angka ditambahkan ke buffer dan ditampilkan sebagai `*`.
4. `#` membandingkan buffer dengan PIN `1234` memakai `strcmp()`.
5. PIN benar → `bukaKunci()`, servo 90°, LED hijau, lalu menutup otomatis setelah 5 detik.
6. PIN salah → counter bertambah; pada kesalahan ke-3 sistem lockout 10 detik.

## Simulasi di Wokwi

1. Buat proyek **Arduino Uno** baru di [wokwi.com](https://wokwi.com).
2. Tempel `KunciPintarKeypad.ino` ke `sketch.ino`.
3. Tempel `diagram.json` ke tab diagram.
4. Klik **Play**.
5. Ketik `1234` lalu `#` pada keypad → servo terbuka dan LED hijau menyala.
6. Coba PIN salah 3 kali untuk menguji lockout.
7. Alternatif untuk pengujian cepat: ketik `1234#` di **Serial Monitor** (9600 baud).
   Sketch menerima input dari keypad maupun Serial.

Contoh log:

```text
=== Kunci Pintar Keypad ===
PIN default: 1234 lalu tekan #.
Input: 1
Input: 12
Input: 123
Input: 1234
AKSES DIBERIKAN - pintu terbuka.
Pintu dikunci kembali.
PIN salah (1/3).
PIN salah (2/3).
PIN salah (3/3).
Sistem terkunci 10 detik.
Lockout selesai. Silakan coba lagi.
```

## Status verifikasi

- [x] Kompilasi `arduino:avr:uno` berhasil.
- [x] `wokwi-cli lint` pada `diagram.json`: tidak ada masalah.
- [x] Automation Scenario Wokwi lulus (PIN benar membuka, PIN salah ditolak).
- [ ] Uji interaktif: ketik PIN pada keypad.
