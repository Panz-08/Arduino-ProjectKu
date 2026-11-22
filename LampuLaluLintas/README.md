# Lampu Lalu Lintas Adaptif

Simulasi lampu lalu lintas dengan tombol permintaan penyeberangan untuk pejalan kaki.
Proyek ini melatih logika *state machine* non-blocking (`millis()`), input tombol dengan
debounce, dan output LED + buzzer.

## Fitur

- Siklus otomatis: **HIJAU 5 detik → KUNING 2 detik → MERAH 5 detik**.
- Tombol penyeberangan memperpanjang fase merah menjadi **8 detik**.
- LED pejalan berkedip dan buzzer berbunyi singkat selama fase merah.
- Semua proses memakai `millis()`, tanpa `delay()` panjang, sehingga tombol tetap responsif.
- Log status dikirim ke Serial Monitor (9600 baud).

## Komponen

| Komponen | Jumlah | Keterangan |
| --- | --- | --- |
| Arduino Uno | 1 | Mikrokontroler |
| LED merah, kuning, hijau | 3 | Lampu kendaraan |
| LED oranye | 1 | Indikator pejalan kaki |
| Resistor 220 Ω | 4 | Pembatas arus LED |
| Push button | 1 | Permintaan penyeberangan |
| Buzzer | 1 | Isyarat suara |
| Breadboard + jumper | — | Koneksi |

## Skema koneksi

| Komponen | Pin komponen | Pin Arduino |
| --- | --- | --- |
| LED merah | Anoda (+) | D8 melalui resistor 220 Ω |
| LED kuning | Anoda (+) | D9 melalui resistor 220 Ω |
| LED hijau | Anoda (+) | D10 melalui resistor 220 Ω |
| LED pejalan | Anoda (+) | D12 melalui resistor 220 Ω |
| Semua LED | Katoda (−) | GND |
| Push button | Kaki 1 | D2 |
| Push button | Kaki 2 | GND |
| Buzzer | Positif (+) | D11 |
| Buzzer | Negatif (−) | GND |

> Tombol memakai `INPUT_PULLUP`, jadi tidak perlu resistor pull-up tambahan.
> Tekan tombol = menghubungkan D2 ke GND.

## Cara kerja

1. `setup()` mengatur pin dan langsung masuk fase **HIJAU**.
2. `loop()` mengecek tombol dan waktu fase tanpa memblokir program.
3. Saat tombol ditekan, `permintaanPejalan` menjadi `true`.
4. Ketika fase merah dimulai, durasi merah menjadi 8 detik dan LED pejalan berkedip.
5. Setelah merah selesai, permintaan direset dan siklus kembali ke hijau.

## Simulasi di Wokwi

1. Buka [wokwi.com](https://wokwi.com) lalu buat proyek **Arduino Uno** baru.
2. Ganti isi `sketch.ino` dengan file `LampuLaluLintas.ino`.
3. Buka tab `diagram.json`, tempel isi `diagram.json` dari folder ini.
4. Klik **Play** (tombol hijau).
5. Klik tombol **PENYEBERANGAN** saat lampu hijau/kuning untuk menguji permintaan.
6. Buka **Serial Monitor** untuk melihat log fase.

Contoh log:

```text
=== Lampu Lalu Lintas Adaptif ===
FASE: HIJAU - kendaraan boleh jalan
Permintaan penyeberangan diterima.
FASE: KUNING - bersiap berhenti
FASE: MERAH - pejalan kaki boleh menyeberang
FASE: HIJAU - kendaraan boleh jalan
```

## Status verifikasi

- [x] Kompilasi `arduino:avr:uno` berhasil (3.570 byte / 11% flash).
- [x] `wokwi-cli lint` pada `diagram.json`: tidak ada masalah.
- [x] Automation Scenario Wokwi lulus (tombol memicu fase merah pejalan kaki).
- [ ] Uji interaktif: jalankan sesuai langkah di atas.
