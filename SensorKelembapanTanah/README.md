# Monitor Kelembapan Tanah Berbasis Arduino

## Deskripsi
Proyek ini merupakan sistem pemantauan kelembapan tanah menggunakan Arduino. Nilai analog dari sensor kelembapan tanah dibaca melalui pin A0, dikonversi menjadi persentase, lalu ditampilkan pada LCD I2C 16x2 dan Serial Monitor. LED pada pin 13 digunakan sebagai indikator kondisi kelembapan berdasarkan ambang batas yang dapat dikonfigurasi.

## Fitur
- Pembacaan nilai analog sensor kelembapan tanah secara periodik.
- Konversi nilai mentah ADC menjadi persentase kelembapan dengan rentang 0% sampai 100%.
- Penampilan nilai kelembapan dan status LED pada LCD I2C 16x2.
- Pengiriman data kelembapan dan status LED ke Serial Monitor dengan baud rate 9600.
- Indikator LED berdasarkan ambang batas kelembapan.

## Kebutuhan Perangkat Keras
| Komponen | Jumlah | Keterangan |
|----------|--------|------------|
| Arduino Uno/Nano | 1 | Mikrokontroler utama |
| Sensor Kelembapan Tanah | 1 | Modul sensor analog |
| LCD I2C 16x2 | 1 | Display output |
| LED | 1 | Indikator status |
| Resistor 220 ohm | 1 | Pembatas arus LED |
| Kabel jumper | Secukupnya | Koneksi antar komponen |

## Skema Koneksi

### Sensor Kelembapan Tanah
| Pin Sensor | Pin Arduino |
|------------|-------------|
| VCC | 5V |
| GND | GND |
| AO / Output Analog | A0 |

### LED
| Pin LED | Pin Arduino |
|---------|-------------|
| Anoda (+) | Pin 13 melalui resistor 220 ohm |
| Katoda (-) | GND |

### LCD I2C
| Pin LCD | Pin Arduino |
|---------|-------------|
| VCC | 5V |
| GND | GND |
| SDA | A4 |
| SCL | A5 |

## Library yang Digunakan
- `Wire.h` bawaan Arduino.
- `LiquidCrystal_I2C.h` yang perlu diinstal melalui Library Manager.

## Cara Kerja
1. Pada saat inisialisasi, sistem menampilkan teks "MONITOR TANAH" dan "BATAS NYALA 30%" pada LCD selama 2 detik.
2. Arduino membaca nilai analog dari sensor melalui pin A0 menggunakan `analogRead()`.
3. Nilai mentah ADC dikonversi menjadi persentase kelembapan menggunakan fungsi `map()` dengan rentang:
   - Nilai mentah 1023 dipetakan menjadi 0%.
   - Nilai mentah 200 dipetakan menjadi 100%.
4. Hasil persentase dibatasi agar berada pada rentang 0% sampai 100%.
5. Nilai kelembapan ditampilkan pada baris pertama LCD.
6. Jika persentase kelembapan lebih besar dari `batasAtas`, LED dinyalakan dan LCD menampilkan "LED: NYALA".
7. Jika persentase kelembapan kurang dari atau sama dengan `batasAtas`, LED dimatikan dan LCD menampilkan "LED: MATI".
8. Data kelembapan dan status LED juga dikirim ke Serial Monitor setiap 1 detik.

## Konfigurasi
Parameter utama dapat diubah pada bagian berikut:

```cpp
const int pinSensor = A0;
const int pinLED = 13;
const int batasAtas = 50;
```

## Simulasi di Wokwi

Wokwi tidak memiliki part sensor kelembapan tanah, jadi `diagram.json` memakai
**potensiometer** pada A0 sebagai pengganti nilai analog sensor.

1. Buat proyek **Arduino Uno** baru di [wokwi.com](https://wokwi.com).
2. Tempel `SensorKelembapanTanah.ino` ke `sketch.ino`.
3. Tempel `diagram.json` ke tab diagram.
4. Klik **Play**.
5. Klik potensiometer dan atur nilainya:
   - `800` → kelembapan ±27% (LED mati).
   - `400` → kelembapan ±76% (LED nyala).
6. Perhatikan perubahan LCD dan Serial Monitor.

## Status verifikasi

- [x] Kompilasi `arduino:avr:uno` berhasil.
- [x] `wokwi-cli lint` pada `diagram.json`: tidak ada masalah.
- [ ] Simulasi Wokwi interaktif: putar potensiometer.
