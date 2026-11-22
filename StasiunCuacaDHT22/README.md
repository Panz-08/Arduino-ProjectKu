# Stasiun Cuaca DHT22

Stasiun cuaca mini dengan sensor **DHT22**, LCD I2C, LED indikator, dan buzzer.
Suhu dan kelembapan ditampilkan secara real-time; bila suhu melewati 35 °C,
LED merah menyala dan buzzer berbunyi.

## Fitur

- Membaca suhu (°C/°F) dan kelembapan relatif tiap 2 detik.
- Tombol D9 mengganti satuan Celsius ↔ Fahrenheit.
- LED hijau = kondisi normal, LED merah + buzzer = suhu ≥ 35 °C.
- Deteksi sensor gagal baca (NaN) dan menampilkan pesan error.
- Log CSV sederhana ke Serial Monitor.

## Komponen

| Komponen | Jumlah | Keterangan |
| --- | --- | --- |
| Arduino Uno | 1 | Mikrokontroler |
| DHT22 / AM2302 | 1 | Sensor suhu & kelembapan |
| LCD I2C 16×2 | 1 | Alamat `0x27` |
| LED hijau & merah | 2 | Indikator kondisi |
| Resistor 220 Ω | 2 | Pembatas arus LED |
| Buzzer | 1 | Alarm suhu panas |
| Push button | 1 | Ganti °C/°F |
| Breadboard + jumper | — | Koneksi |

## Skema koneksi

| Komponen | Pin | Arduino |
| --- | --- | --- |
| DHT22 | VCC | 5V |
| DHT22 | SDA / DATA | D2 |
| DHT22 | GND | GND |
| LCD I2C | VCC / GND | 5V / GND |
| LCD I2C | SDA | A4 |
| LCD I2C | SCL | A5 |
| LED hijau | Anoda | D6 via 220 Ω |
| LED merah | Anoda | D7 via 220 Ω |
| Buzzer | Positif | D8 |
| Push button | Kaki 1 | D9 |
| Push button | Kaki 2 | GND |

> DHT22 punya 4 pin: VCC, DATA, NC, GND. Pin NC tidak dipakai.

## Library

- `Wire.h` — bawaan Arduino.
- `LiquidCrystal_I2C.h` — Library Manager.
- `DHT sensor library` (Adafruit) + `Adafruit Unified Sensor` — Library Manager.

## Cara kerja

1. `setup()` menginisialisasi DHT22, LCD, LED, buzzer, dan tombol.
2. Setiap 2 detik `dht.readTemperature()` dan `dht.readHumidity()` dipanggil.
3. Nilai ditampilkan pada LCD dengan format `Suhu: 24.0 °C` dan `Lembap: 40.0 %`.
4. Jika suhu ≥ 35 °C, LED merah menyala dan buzzer berbunyi tiap 1 detik.
5. Tombol D9 membalik satuan tampilan tanpa mengubah pembacaan sensor.

## Simulasi di Wokwi

1. Buat proyek **Arduino Uno** baru di [wokwi.com](https://wokwi.com).
2. Tempel `StasiunCuacaDHT22.ino` ke `sketch.ino`.
3. Tempel `diagram.json` ke tab diagram.
4. Klik **Play**.
5. Saat simulasi berjalan, **klik sensor DHT22** lalu geser slider suhu ke `36`
   untuk memicu alarm. Geser ke `24` untuk kembali normal.
6. Klik tombol **C/F** untuk menguji perubahan satuan.

Contoh log:

```text
=== Stasiun Cuaca DHT22 ===
Suhu=24.0 C, Kelembapan=40.0 %
Satuan: Fahrenheit
Suhu=75.2 F, Kelembapan=40.0 %
PERINGATAN: suhu di atas batas 35 C!
Suhu kembali normal.
```

## Status verifikasi

- [x] Kompilasi `arduino:avr:uno` berhasil.
- [x] `wokwi-cli lint` pada `diagram.json`: tidak ada masalah.
- [x] Automation Scenario Wokwi lulus (suhu 36 °C memicu alarm).
- [ ] Uji interaktif: ubah slider suhu pada DHT22.
