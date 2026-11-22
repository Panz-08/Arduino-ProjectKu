# Monitor Suhu Web ESP32

Proyek IoT: **ESP32 DevKit C V4** membaca sensor DHT22 dan menyajikan dashboard
web sederhana. Suhu, kelembapan, status alarm, dan kontrol LED bisa diakses
lewat browser.

## Fitur

- Koneksi ke WiFi virtual **Wokwi-GUEST** (khusus simulasi Wokwi).
- Dashboard HTML responsif dengan pembaruan otomatis tiap 2 detik.
- API JSON: `GET /api/data`.
- Kontrol LED: `GET /api/led?state=on`, `?state=off`, atau `?state=toggle`.
- Buzzer berbunyi bila suhu ≥ 35 °C.
- Log Serial 115200 baud untuk debugging.

## Komponen

| Komponen | Jumlah | Keterangan |
| --- | --- | --- |
| ESP32 DevKit C V4 | 1 | Mikrokontroler + WiFi |
| DHT22 | 1 | Sensor suhu & kelembapan |
| LED | 1 | Indikator yang dikontrol dari web |
| Resistor 220 Ω | 1 | Pembatas arus LED |
| Buzzer | 1 | Alarm suhu panas |
| Breadboard + jumper | — | Koneksi |

## Skema koneksi

| Komponen | Pin | ESP32 |
| --- | --- | --- |
| DHT22 | VCC | 3V3 |
| DHT22 | SDA / DATA | GPIO15 |
| DHT22 | GND | GND |
| LED | Anoda | GPIO2 via resistor 220 Ω |
| LED | Katoda | GND |
| Buzzer | Positif | GPIO5 |
| Buzzer | Negatif | GND |

> DHT22 diberi catu 3.3 V agar aman untuk pin ESP32. Jangan memberi 5 V ke pin data.

## Library

- `WiFi.h`, `WebServer.h` — bawaan core ESP32.
- `DHTesp.h` — **DHT sensor library for ESPx** (disarankan Wokwi untuk ESP32).

## Cara kerja

1. `setup()` menyalakan Serial, menginisialisasi DHT22, lalu menghubungkan WiFi
   `Wokwi-GUEST` pada channel 6.
2. Setelah terhubung, ESP32 menampilkan alamat IP di Serial Monitor.
3. `WebServer` melayani halaman dashboard dan endpoint API.
4. `loop()` memanggil `server.handleClient()` agar web tetap responsif, membaca
   DHT22 tiap 2 detik, dan menyalakan buzzer saat suhu terlalu panas.
5. Tombol **Toggle LED** di dashboard memanggil `/api/led?state=toggle`.

## Simulasi di Wokwi

1. Buat proyek **ESP32 DevKit C V4** baru di [wokwi.com](https://wokwi.com).
2. Tempel `MonitorSuhuWebESP32.ino` ke `sketch.ino`.
3. Tempel `diagram.json` ke tab diagram.
4. Klik **Play** dan buka **Serial Monitor** (115200 baud).
5. Tunggu hingga muncul `WiFi terhubung. Buka http://...`.
6. Untuk membuka dashboard dari browser, pasang **Wokwi IoT Gateway** lalu akses
   `http://localhost:9080/`. Gateway ini diperlukan karena browser tidak bisa
   menjangkau jaringan virtual Wokwi secara langsung.
7. Klik sensor DHT22 dan ubah suhu ke `36` untuk menguji alarm.

Contoh log:

```text
=== Monitor Suhu Web ESP32 ===
Menghubungkan ke WiFi....
WiFi terhubung. Buka http://10.13.37.2
Web server aktif pada port 80.
Suhu=24.0 C, Kelembapan=40.0 %
LED -> ON
PERINGATAN: suhu di atas batas 35 C!
```

Contoh respons `GET /api/data`:

```json
{"suhu":24.0,"kelembapan":40.0,"led":false,"panas":false}
```

## Status verifikasi

- [x] Kompilasi `esp32:esp32:esp32` berhasil (1.008.447 byte / 76% flash).
- [x] `wokwi-cli lint` pada `diagram.json`: tidak ada masalah.
- [x] Automation Scenario Wokwi lulus (WiFi, web server, dan alarm suhu).
- [ ] Uji interaktif: cek Serial Monitor dan dashboard.
