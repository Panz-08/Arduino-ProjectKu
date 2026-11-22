# Arduino ProjectKu

[![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Arduino](https://img.shields.io/badge/board-Arduino%20Uno-00979D?logo=arduino&logoColor=white)](https://www.arduino.cc/)
[![ESP32](https://img.shields.io/badge/board-ESP32-E7352C?logo=espressif&logoColor=white)](https://www.espressif.com/)
[![Wokwi](https://img.shields.io/badge/simulator-Wokwi-7C3AED)](https://wokwi.com/)

Kumpulan proyek Arduino dan ESP32 untuk pembelajaran di SMK Putra Pakuan.
Setiap proyek punya sketch `.ino`, panduan komponen, tabel koneksi, penjelasan
cara kerja, dan **diagram Wokwi** (`diagram.json`) agar bisa langsung disimulasikan
di [wokwi.com](https://wokwi.com).

## Daftar proyek

| # | Proyek | Board | Sensor / aktuator | Output | Folder |
| --- | --- | --- | --- | --- | --- |
| 1 | Radar Cerdas | Uno | HC-SR04 + servo | LCD I2C, LED | [`SensorUltrasonik/`](SensorUltrasonik/) |
| 2 | Detektor Tepukan Ganda | Uno | Sensor suara digital | LED | [`SensorSuara/`](SensorSuara/) |
| 3 | Monitor Kelembapan Tanah | Uno | Sensor tanah analog | LCD I2C, LED | [`SensorKelembapanTanah/`](SensorKelembapanTanah/) |
| 4 | Lampu Lalu Lintas Adaptif | Uno | Push button | 4 LED, buzzer | [`LampuLaluLintas/`](LampuLaluLintas/) |
| 5 | Alarm Gerak PIR | Uno | PIR HC-SR501 | LED, buzzer | [`AlarmGerakPIR/`](AlarmGerakPIR/) |
| 6 | Parkir Otomatis | Uno | HC-SR04 + servo | LCD I2C, LED, buzzer | [`ParkirOtomatis/`](ParkirOtomatis/) |
| 7 | Kunci Pintar Keypad | Uno | Keypad 4×4 + servo | LCD I2C, LED, buzzer | [`KunciPintarKeypad/`](KunciPintarKeypad/) |
| 8 | Stasiun Cuaca DHT22 | Uno | DHT22 + tombol | LCD I2C, LED, buzzer | [`StasiunCuacaDHT22/`](StasiunCuacaDHT22/) |
| 9 | Monitor Suhu Web | ESP32 | DHT22 | Web dashboard, LED, buzzer | [`MonitorSuhuWebESP32/`](MonitorSuhuWebESP32/) |

## Komponen umum

- Arduino Uno / Nano atau ESP32 DevKit C V4
- LCD I2C 16×2 (alamat default `0x27`)
- LED + resistor 220 Ω
- Buzzer
- Push button
- Breadboard dan kabel jumper

## Library

| Library | Dipakai oleh |
| --- | --- |
| `Wire.h`, `Servo.h` | Bawaan Arduino |
| `LiquidCrystal_I2C.h` | Proyek LCD |
| `DHT sensor library` + `Adafruit Unified Sensor` | StasiunCuacaDHT22 |
| `Keypad.h` | KunciPintarKeypad |
| `DHT sensor library for ESPx` (`DHTesp.h`) | MonitorSuhuWebESP32 |
| `WiFi.h`, `WebServer.h` | Bawaan core ESP32 |

Pasang library lewat **Library Manager** di Arduino IDE, atau:

```bash
arduino-cli lib install "LiquidCrystal I2C" "DHT sensor library" \
  "Adafruit Unified Sensor" "Keypad" "DHT sensor library for ESPx" Servo
```

## Cara memakai

### Arduino IDE

1. Buka folder proyek yang diinginkan. Nama folder sudah sama dengan nama sketch,
   jadi Arduino IDE bisa langsung membukanya.
2. Baca `README.md` di folder tersebut untuk skema koneksi dan konfigurasi pin.
3. Pilih board dan port yang benar, lalu **Upload**.
4. Buka Serial Monitor pada baud rate yang tertulis di README.

### Wokwi

Setiap folder berisi `diagram.json`. Cara cepat:

1. Buka [wokwi.com](https://wokwi.com) dan buat proyek baru sesuai board.
2. Tempel isi file `.ino` ke `sketch.ino`.
3. Buka tab `diagram.json`, tempel isi `diagram.json` dari folder proyek.
4. Klik **Play**.

Beberapa proyek memakai part pengganti karena Wokwi belum menyediakan sensornya:

| Proyek | Part Wokwi | Cara memicu |
| --- | --- | --- |
| Detektor Tepukan | Push button + pull-up 10 kΩ | Klik dua kali cepat |
| Monitor Kelembapan Tanah | Potensiometer | Putar nilai 800 → 400 |
| Radar / Parkir | HC-SR04 | Klik sensor, ubah `distance` |
| Stasiun Cuaca / ESP32 | DHT22 | Klik sensor, geser suhu |
| Alarm Gerak | PIR | Klik sensor → **Simulate Motion** |

## Verifikasi otomatis dengan Wokwi

Folder [`tests/`](tests/) berisi **Automation Scenario** Wokwi untuk menguji setiap
proyek: menekan tombol, mengubah nilai sensor, lalu menunggu teks tertentu di
Serial Monitor.

```bash
export WOKWI_CLI_TOKEN="token-dari-wokwi.com/dashboard/ci"
bash tests/run-wokwi.sh
```

Script akan mengompilasi firmware, membuat `wokwi.toml`, menjalankan simulasi, dan
menyimpan log ke `build/wokwi/<NamaProyek>/serial.log`. Detailnya ada di
[`tests/README.md`](tests/README.md).

## Struktur repo

```text
Arduino-ProjectKu/
├── SensorUltrasonik/          # radar ultrasonik + servo
├── SensorSuara/               # detektor tepukan ganda
├── SensorKelembapanTanah/     # monitor kelembapan tanah
├── LampuLaluLintas/           # lampu lalu lintas adaptif
├── AlarmGerakPIR/             # alarm gerak PIR
├── ParkirOtomatis/            # palang parkir otomatis
├── KunciPintarKeypad/         # kunci pintu keypad 4x4
├── StasiunCuacaDHT22/         # stasiun cuaca DHT22
├── MonitorSuhuWebESP32/       # dashboard web ESP32
└── tests/                     # Automation Scenario Wokwi
```

Setiap folder berisi:

```text
NamaProyek/
├── NamaProyek.ino   # sketch Arduino
├── diagram.json     # rangkaian Wokwi
└── README.md        # panduan lengkap
```

## Status verifikasi

| Proyek | Kompilasi | Lint diagram | Simulasi Wokwi |
| --- | --- | --- | --- |
| SensorUltrasonik | ✅ `arduino:avr:uno` | ✅ | ✅ |
| SensorSuara | ✅ `arduino:avr:uno` | ✅ | ✅ |
| SensorKelembapanTanah | ✅ `arduino:avr:uno` | ✅ | ✅ |
| LampuLaluLintas | ✅ `arduino:avr:uno` | ✅ | ✅ |
| AlarmGerakPIR | ✅ `arduino:avr:uno` | ✅ | ✅ |
| ParkirOtomatis | ✅ `arduino:avr:uno` | ✅ | ✅ |
| KunciPintarKeypad | ✅ `arduino:avr:uno` | ✅ | ✅ |
| StasiunCuacaDHT22 | ✅ `arduino:avr:uno` | ✅ | ✅ |
| MonitorSuhuWebESP32 | ✅ `esp32:esp32:esp32` | ✅ | ✅ |

Verifikasi kompilasi memakai `arduino-cli`, validasi rangkaian memakai
`wokwi-cli lint`, dan pengujian perilaku memakai **Automation Scenario** Wokwi
(`tests/`). Kesembilan proyek sudah lulus simulasi otomatis di Wokwi CI.

> Tombol pada diagram memakai atribut `"bounce": "0"` agar hasil pengujian
> otomatis konsisten. Pada rangkaian nyata, debounce tetap ditangani oleh sketch.

## Catatan perubahan

- Folder lama `Sensor-UltraSonik`, `Sensor-Suara`, dan `Sensor-Kelembapan-Tanah`
  diganti nama menjadi `SensorUltrasonik`, `SensorSuara`, dan
  `SensorKelembapanTanah` agar sesuai aturan Arduino IDE (nama folder harus sama
  dengan nama sketch).
- Enam proyek baru ditambahkan: LampuLaluLintas, AlarmGerakPIR, ParkirOtomatis,
  KunciPintarKeypad, StasiunCuacaDHT22, dan MonitorSuhuWebESP32.
- Semua proyek kini punya `diagram.json` untuk Wokwi.

## Lisensi

[MIT](LICENSE) © 2026 Panz-08
