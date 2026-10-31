# Detektor Tepukan Ganda Berbasis Arduino

## Deskripsi
Proyek ini merupakan implementasi detektor tepukan ganda (double clap) menggunakan Arduino. Sistem membaca sinyal digital dari sensor suara dan mengidentifikasi dua tepukan yang terjadi dalam rentang waktu tertentu. Ketika dua tepukan terdeteksi, status LED akan diubah (toggle). Proyek ini dapat digunakan sebagai dasar untuk kontrol perangkat sederhana berbasis suara.

## Fitur
- Deteksi tepukan melalui sensor suara digital.
- Pengenalan pola dua tepukan dalam jendela waktu 500 ms.
- Mekanisme debounce 100 ms untuk mencegah deteksi palsu akibat pantulan sinyal.
- Toggle status LED setiap kali dua tepukan valid terdeteksi.
- Tidak memerlukan library tambahan selain Arduino core.

## Kebutuhan Perangkat Keras
| Komponen | Jumlah | Keterangan |
|----------|--------|------------|
| Arduino Uno/Nano | 1 | Mikrokontroler utama |
| Sensor Suara Digital (mis. KY-038, LM393) | 1 | Output digital |
| LED | 1 | Indikator status |
| Resistor 220 ohm | 1 | Pembatas arus LED |
| Kabel jumper | Secukupnya | Koneksi antar komponen |

## Skema Koneksi

### Sensor Suara
| Pin Sensor | Pin Arduino |
|------------|-------------|
| VCC | 5V |
| GND | GND |
| DO (Digital Output) | Pin 8 |

### LED
| Pin LED | Pin Arduino |
|---------|-------------|
| Anoda (+) | Pin 13 melalui resistor 220 ohm |
| Katoda (-) | GND |

## Library yang Digunakan
Tidak ada library eksternal. Seluruh fungsi menggunakan API bawaan Arduino.

## Cara Kerja
1. Pada `setup()`, pin sensor diatur sebagai `INPUT` dan pin LED sebagai `OUTPUT`. Status LED awal diatur ke `LOW` (mati).
2. Pada `loop()`, program membaca status digital sensor (`digitalRead`) dan membandingkannya dengan status sebelumnya.
3. Deteksi tepukan terjadi pada transisi dari `HIGH` ke `LOW` (falling edge), yang menandakan adanya suara keras.
4. Untuk setiap tepukan yang terdeteksi:
   - Dilakukan pengecekan debounce: `millis() - waktuTepukanPertama > jedaDebounce` (100 ms). Namun, karena `waktuTepukanPertama` awalnya bernilai 0, tepukan pertama selalu lolos.
   - `hitungTepukan` dinaikkan.
   - Jika ini adalah tepukan pertama (`hitungTepukan == 1`), waktu tepukan pertama dicatat.
   - Jika ini adalah tepukan kedua (`hitungTepukan == 2`) dan selisih waktu sejak tepukan pertama ≤ 500 ms, maka status LED di-toggle dan `hitungTepukan` direset ke 0.
5. Jika `hitungTepukan` lebih dari 0 dan waktu sejak tepukan pertama melebihi 500 ms tanpa tepukan kedua, maka `hitungTepukan` direset ke 0.
6. Status sensor sebelumnya diperbarui setiap iterasi untuk mendeteksi transisi berikutnya.

## Konfigurasi
Parameter waktu dapat diubah pada deklarasi berikut:

```cpp
const unsigned long jedaMaksimalAntarTepukan = 500; // ms
const unsigned long jedaDebounce = 100;             // ms
