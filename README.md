# Arduino ProjectKu

[![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)

Kumpulan proyek Arduino untuk pembelajaran di SMK Putra Pakuan. Setiap proyek punya
sketch `.ino`, panduan komponen, tabel koneksi, dan penjelasan cara kerja di folder
masing-masing.

## Daftar proyek

| Proyek | Sensor / aktuator | Output | Folder |
| --- | --- | --- | --- |
| Radar Cerdas | HC-SR04 + servo SG90 | LCD I2C 16x2, LED | [`Sensor-UltraSonik/`](Sensor-UltraSonik/) |
| Detektor Tepukan Ganda | Sensor suara digital (KY-038/LM393) | LED | [`Sensor-Suara/`](Sensor-Suara/) |
| Monitor Kelembapan Tanah | Sensor kelembapan tanah analog | LCD I2C 16x2, LED | [`Sensor-Kelembapan-Tanah/`](Sensor-Kelembapan-Tanah/) |

## Komponen umum

- Arduino Uno atau Nano
- LCD I2C 16x2 (alamat default `0x27`)
- LED + resistor 220 Ω
- Kabel jumper dan breadboard

## Library

- `Wire.h` dan `Servo.h` - bawaan Arduino IDE.
- `LiquidCrystal_I2C.h` - pasang lewat **Library Manager** di Arduino IDE.

## Cara memakai

1. Buka folder proyek yang diinginkan.
2. Baca `README.md` di folder tersebut untuk skema koneksi dan konfigurasi pin.
3. Buka file `.ino` dengan Arduino IDE.
4. Pilih board dan port yang benar, lalu **Upload**.
5. Buka Serial Monitor pada baud rate `9600` bila proyek mengirim data serial.

## Struktur repo

```text
Arduino-ProjectKu/
├── Sensor-UltraSonik/          # radar ultrasonik + servo
├── Sensor-Suara/               # detektor tepukan ganda
└── Sensor-Kelembapan-Tanah/    # monitor kelembapan tanah
```

## Lisensi

[MIT](LICENSE) © 2026 Panz-08
