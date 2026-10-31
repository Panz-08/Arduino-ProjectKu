# Radar Cerdas Berbasis Arduino

## Deskripsi
Proyek ini merupakan implementasi sistem radar sederhana menggunakan Arduino. Sistem memanfaatkan sensor ultrasonik untuk mengukur jarak objek dan servo motor sebagai penggerak pemindai. Hasil pemindaian ditampilkan pada LCD I2C 16x2 dan indikator LED. Proyek ini cocok untuk pembelajaran dasar tentang sensor, aktuator, dan antarmuka pengguna.

## Fitur
- Pemindaian area secara otomatis dengan pergerakan servo bolak-balik pada rentang sudut 15° hingga 165°.
- Deteksi objek dengan ambang jarak yang dapat dikonfigurasi (default 15 cm).
- Indikasi visual melalui LED dan tampilan status pada LCD.
- Menampilkan sudut pemindaian dan jarak objek secara real-time.

## Kebutuhan Perangkat Keras
| Komponen | Jumlah | Keterangan |
|----------|--------|------------|
| Arduino Uno/Nano | 1 | Mikrokontroler utama |
| Sensor Ultrasonik HC-SR04 | 1 | Pengukur jarak |
| Servo Motor (SG90/MG996R) | 1 | Penggerak pemindai |
| LCD I2C 16x2 | 1 | Display output |
| LED | 1 | Indikator deteksi |
| Resistor 220 Ω | 1 | Pembatas arus LED |
| Kabel jumper | Secukupnya | Koneksi antar komponen |

## Skema Koneksi

### Sensor Ultrasonik HC-SR04
| Pin Sensor | Pin Arduino |
|------------|-------------|
| VCC | 5V |
| GND | GND |
| Trig | Pin 9 |
| Echo | Pin 8 |

### Servo Motor
| Pin Servo | Pin Arduino |
|-----------|-------------|
| VCC (Merah) | 5V |
| GND (Coklat) | GND |
| Signal (Orange) | Pin 10 |

### LED
| Pin LED | Pin Arduino |
|---------|-------------|
| Anoda (+) | Pin 13 melalui resistor 220 Ω |
| Katoda (-) | GND |

### LCD I2C
| Pin LCD | Pin Arduino |
|---------|-------------|
| VCC | 5V |
| GND | GND |
| SDA | A4 |
| SCL | A5 |

## Library yang Digunakan
- `Wire.h` (bawaan Arduino)
- `LiquidCrystal_I2C.h` (instal melalui Library Manager)
- `Servo.h` (bawaan Arduino)

## Cara Kerja
1. Pada saat inisialisasi, sistem menampilkan teks "Radar Cerdas" dan "Sistem Siap..." pada LCD selama 2 detik.
2. Servo bergerak dari sudut 15° hingga 165° dengan penambahan 3° setiap 30 ms.
3. Sensor ultrasonik mengukur jarak setiap 10 ms.
4. Jika jarak objek ≤ 15 cm dan > 0 cm, LED menyala dan LCD menampilkan "TARGET TERKUNCI" beserta nilai jarak.
5. Jika tidak ada objek dalam ambang jarak, LED mati dan LCD menampilkan "Memindai..." beserta sudut servo saat ini.

## Konfigurasi
Parameter sistem dapat diubah pada bagian berikut dalam kode sumber:
```cpp
const int jarakAman = 15;        // Ambang jarak deteksi (cm)
const int jedaServo = 30;        // Jeda pergerakan servo (ms)
int sudutSekarang = 15;          // Sudut awal servo
int arahPutaran = 3;             // Increment sudut per langkah
