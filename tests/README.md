# Verifikasi Wokwi

Folder ini berisi **Automation Scenario** Wokwi untuk menguji setiap proyek secara
otomatis: menekan tombol, mengubah nilai sensor, lalu menunggu teks tertentu muncul
di Serial Monitor.

## Prasyarat

- [Arduino CLI](https://arduino.github.io/arduino-cli/) untuk kompilasi.
- Core board:
  ```bash
  arduino-cli core install arduino:avr
  arduino-cli core install esp32:esp32 \
    --additional-urls https://espressif.github.io/arduino-esp32/package_esp32_index.json
  ```
- Library:
  ```bash
  arduino-cli lib install "LiquidCrystal I2C" "DHT sensor library" \
    "Adafruit Unified Sensor" "Keypad" "DHT sensor library for ESPx" Servo
  ```
- [Wokwi CLI](https://github.com/wokwi/wokwi-cli) dan token dari
  [wokwi.com/dashboard/ci](https://wokwi.com/dashboard/ci):
  ```bash
  export WOKWI_CLI_TOKEN="token-kamu"
  ```

## Menjalankan semua skenario

Dari root repo:

```bash
bash tests/run-wokwi.sh
```

Untuk menjalankan sebagian proyek saja, sebutkan namanya:

```bash
bash tests/run-wokwi.sh SensorSuara LampuLaluLintas
```

Script akan:

1. Menyalin sketch + `diagram.json` ke `build/wokwi/<NamaProyek>/`.
2. Mengompilasi firmware dengan `arduino-cli`.
3. Membuat `wokwi.toml` yang menunjuk ke firmware hasil kompilasi.
4. Menjalankan `wokwi-cli` dengan scenario terkait.
5. Menyimpan log serial ke `build/wokwi/<NamaProyek>/serial.log`.

Batas waktu default 45 detik untuk board AVR dan 90 detik untuk ESP32. Untuk
mengubahnya, set `WOKWI_TIMEOUT_MS`:

```bash
WOKWI_TIMEOUT_MS=120000 bash tests/run-wokwi.sh MonitorSuhuWebESP32
```

## Menjalankan satu proyek manual

```bash
# Contoh untuk LampuLaluLintas
mkdir -p build/wokwi/LampuLaluLintas
cp LampuLaluLintas/LampuLaluLintas.ino \
   LampuLaluLintas/diagram.json \
   build/wokwi/LampuLaluLintas/
cp tests/LampuLaluLintas.test.yaml build/wokwi/LampuLaluLintas/scenario.yaml

arduino-cli compile --fqbn arduino:avr:uno \
  --output-dir build/wokwi/LampuLaluLintas/build \
  build/wokwi/LampuLaluLintas

cat > build/wokwi/LampuLaluLintas/wokwi.toml <<'EOF'
[wokwi]
version = 1
firmware = 'build/LampuLaluLintas.ino.hex'
EOF

cd build/wokwi/LampuLaluLintas
wokwi-cli . \
  --scenario scenario.yaml \
  --timeout 30000 \
  --serial-log-file serial.log
```

## Daftar skenario

| File | Yang diuji |
| --- | --- |
| `SensorUltrasonik.test.yaml` | Target terdeteksi pada jarak 10 cm |
| `SensorSuara.test.yaml` | Dua tepukan mengubah status LED |
| `SensorKelembapanTanah.test.yaml` | Nilai basah/kering mengubah LED |
| `LampuLaluLintas.test.yaml` | Tombol memicu fase merah pejalan kaki |
| `AlarmGerakPIR.test.yaml` | Gerakan memicu alarm dan berhenti otomatis |
| `ParkirOtomatis.test.yaml` | Palang buka saat dekat, tutup saat jauh |
| `KunciPintarKeypad.test.yaml` | PIN benar membuka, PIN salah ditolak |
| `StasiunCuacaDHT22.test.yaml` | Suhu 36 °C memicu alarm |
| `MonitorSuhuWebESP32.test.yaml` | WiFi, web server, dan alarm suhu |

> Automation Scenario Wokwi masih berstatus **alpha**. Nama kontrol part dapat
> berubah pada versi Wokwi CLI berikutnya.

## Catatan pengujian

- Tombol pada `diagram.json` memakai `"bounce": "0"` supaya satu tekan hanya
  menghasilkan satu transisi. Sketch tetap memiliki debounce untuk rangkaian
  nyata.
- Sensor PIR di Wokwi belum punya kontrol Automation Scenario. Skenario
  `AlarmGerakPIR` memakai perintah uji `M` lewat Serial Monitor, yang memang
  disediakan oleh sketch.
- Skenario `SensorSuara` memeriksa pin 13 dengan `expect-pin`, bukan menunggu
  teks, karena teks `Tepukan 2` muncul tepat saat tombol ditekan.
