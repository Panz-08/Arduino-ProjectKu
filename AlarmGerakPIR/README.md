# Alarm Gerak PIR

Alarm keamanan sederhana berbasis sensor gerak **PIR HC-SR501**. Saat ada gerakan,
LED berkedip dan buzzer berbunyi putus-putus. Sistem bisa diaktifkan atau
dinonaktifkan lewat tombol.

## Fitur

- Deteksi gerakan memakai PIR (passive infrared).
- Indikator LED merah berkedip dan buzzer berbunyi saat alarm aktif.
- Alarm berhenti otomatis 3 detik setelah gerakan terakhir.
- Tombol ARM/DISARM dengan debounce.
- Log ke Serial Monitor (9600 baud).

## Komponen

| Komponen | Jumlah | Keterangan |
| --- | --- | --- |
| Arduino Uno | 1 | Mikrokontroler |
| Sensor PIR HC-SR501 | 1 | Deteksi gerakan |
| LED merah | 1 | Indikator alarm |
| Resistor 220 Ω | 1 | Pembatas arus LED |
| Buzzer | 1 | Alarm suara |
| Push button | 1 | ARM/DISARM |
| Breadboard + jumper | — | Koneksi |

## Skema koneksi

| Komponen | Pin komponen | Pin Arduino |
| --- | --- | --- |
| PIR | VCC | 5V |
| PIR | GND | GND |
| PIR | OUT | D2 |
| LED merah | Anoda (+) | D13 melalui resistor 220 Ω |
| LED merah | Katoda (−) | GND |
| Buzzer | Positif (+) | D9 |
| Buzzer | Negatif (−) | GND |
| Push button | Kaki 1 | D3 |
| Push button | Kaki 2 | GND |

## Cara kerja

1. Sistem mulai dalam keadaan **AKTIF**.
2. `loop()` membaca pin OUT PIR. Nilai HIGH berarti ada gerakan.
3. Saat gerakan pertama terdeteksi, alarm dinyalakan dan waktu gerakan terakhir dicatat.
4. Selama alarm aktif, LED berkedip tiap 120 ms dan buzzer berbunyi tiap 250 ms.
5. Jika tidak ada gerakan selama 3 detik, alarm dimatikan otomatis.
6. Tombol D3 membalik status ARM/DISARM.

## Simulasi di Wokwi

1. Buat proyek **Arduino Uno** baru di [wokwi.com](https://wokwi.com).
2. Tempel `AlarmGerakPIR.ino` ke `sketch.ino`.
3. Tempel `diagram.json` ke tab diagram.
4. Klik **Play**.
5. Saat simulasi berjalan, **klik sensor PIR** lalu pilih **Simulate Motion**.
6. LED dan buzzer akan aktif. Klik tombol **ARM/DISARM** untuk menguji mode nonaktif.

Contoh log:

```text
=== Alarm Gerak PIR ===
Sistem AKTIF. Tekan tombol untuk ARM/DISARM.
ALARM: gerakan terdeteksi!
Alarm berhenti: area kembali aman.
Sistem NONAKTIF (DISARM)
```

## Status verifikasi

- [x] Kompilasi `arduino:avr:uno` berhasil.
- [x] `wokwi-cli lint` pada `diagram.json`: tidak ada masalah.
- [ ] Simulasi Wokwi interaktif: klik **Simulate Motion** pada sensor PIR.
