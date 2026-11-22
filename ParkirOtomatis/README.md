# Parkir Otomatis

Palang parkir otomatis dengan sensor ultrasonik **HC-SR04**, servo, LCD I2C, LED
indikator, dan buzzer. Cocok untuk belajar gabungan sensor jarak, aktuator, dan
tampilan LCD.

## Fitur

- Mendeteksi kendaraan pada jarak **≤ 20 cm**.
- Palang servo terbuka 90° dan menutup kembali 3 detik setelah kendaraan menjauh.
- LED hijau = palang terbuka, LED merah = palang tertutup.
- LCD 16×2 menampilkan status dan jarak secara real-time.
- Buzzer berbunyi saat palang membuka/menutup.
- Log jarak dikirim ke Serial Monitor setiap detik.

## Komponen

| Komponen | Jumlah | Keterangan |
| --- | --- | --- |
| Arduino Uno | 1 | Mikrokontroler |
| HC-SR04 | 1 | Sensor jarak ultrasonik |
| Servo SG90 | 1 | Penggerak palang |
| LCD I2C 16×2 | 1 | Alamat default `0x27` |
| LED hijau & merah | 2 | Indikator status |
| Resistor 220 Ω | 2 | Pembatas arus LED |
| Buzzer | 1 | Indikator suara |
| Breadboard + jumper | — | Koneksi |

## Skema koneksi

| Komponen | Pin komponen | Pin Arduino |
| --- | --- | --- |
| HC-SR04 | VCC | 5V |
| HC-SR04 | GND | GND |
| HC-SR04 | TRIG | D9 |
| HC-SR04 | ECHO | D8 |
| Servo | V+ (merah) | 5V |
| Servo | GND (coklat) | GND |
| Servo | PWM (oranye) | D10 |
| LCD I2C | VCC | 5V |
| LCD I2C | GND | GND |
| LCD I2C | SDA | A4 |
| LCD I2C | SCL | A5 |
| LED hijau | Anoda (+) | D6 melalui resistor 220 Ω |
| LED merah | Anoda (+) | D7 melalui resistor 220 Ω |
| Semua LED | Katoda (−) | GND |
| Buzzer | Positif (+) | D5 |
| Buzzer | Negatif (−) | GND |

> Servo sebaiknya diberi catu daya terpisah bila memakai servo besar. Pada
> simulasi Wokwi, catu 5V dari Arduino sudah cukup.

## Library

- `Wire.h` — bawaan Arduino.
- `Servo.h` — bawaan Arduino.
- `LiquidCrystal_I2C.h` — pasang lewat Library Manager.

## Cara kerja

1. Sensor ultrasonik mengukur jarak setiap ±50 ms.
2. Jika jarak ≤ 20 cm dan palang masih tertutup, `bukaPalang()` dipanggil.
3. Selama kendaraan dekat, waktu objek terakhir terus diperbarui.
4. Jika kendaraan sudah menjauh lebih dari 3 detik, `tutupPalang()` dipanggil.
5. LCD diperbarui tiap 250 ms agar tidak berkedip berlebihan.

## Simulasi di Wokwi

1. Buat proyek **Arduino Uno** baru di [wokwi.com](https://wokwi.com).
2. Tempel `ParkirOtomatis.ino` ke `sketch.ino`.
3. Tempel `diagram.json` ke tab diagram.
4. Klik **Play**.
5. Saat simulasi berjalan, **klik sensor HC-SR04** dan ubah nilai **distance**
   menjadi `10` cm untuk membuka palang, lalu `100` cm untuk menutupnya.
6. Perhatikan servo, LED, LCD, dan Serial Monitor.

Contoh log:

```text
=== Parkir Otomatis ===
Palang tertutup. Menunggu kendaraan...
Jarak: 400 cm | Palang: TERTUTUP
Jarak: 10 cm | Palang: TERBUKA
Palang DIBUKA - silakan masuk.
Jarak: 100 cm | Palang: TERTUTUP
Palang DITUTUP - area aman.
```

## Status verifikasi

- [x] Kompilasi `arduino:avr:uno` berhasil.
- [x] `wokwi-cli lint` pada `diagram.json`: tidak ada masalah.
- [ ] Simulasi Wokwi interaktif: ubah nilai distance pada sensor.
