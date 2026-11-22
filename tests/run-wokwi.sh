#!/usr/bin/env bash
#
# Menjalankan semua Automation Scenario Wokwi untuk Arduino-ProjectKu.
#
# Pemakaian:
#   export WOKWI_CLI_TOKEN="token-dari-wokwi.com/dashboard/ci"
#   bash tests/run-wokwi.sh                 # semua proyek
#   bash tests/run-wokwi.sh LampuLaluLintas # hanya proyek tertentu
#
# Hasil ada di build/wokwi/<NamaProyek>/serial.log

set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
OUT_ROOT="$ROOT/build/wokwi"
# Timeout default: 45 detik untuk AVR, 90 detik untuk ESP32 (booting + WiFi).
TIMEOUT_MS="${WOKWI_TIMEOUT_MS:-}"

if [[ -z "${WOKWI_CLI_TOKEN:-}" ]]; then
  echo "ERROR: WOKWI_CLI_TOKEN belum di-set." >&2
  echo "Ambil token di https://wokwi.com/dashboard/ci" >&2
  exit 1
fi

command -v arduino-cli >/dev/null || { echo "ERROR: arduino-cli tidak ditemukan." >&2; exit 1; }
command -v wokwi-cli >/dev/null || { echo "ERROR: wokwi-cli tidak ditemukan." >&2; exit 1; }

# Format: NamaProyek|FQBN
PROJECTS=(
  "SensorUltrasonik|arduino:avr:uno"
  "SensorSuara|arduino:avr:uno"
  "SensorKelembapanTanah|arduino:avr:uno"
  "LampuLaluLintas|arduino:avr:uno"
  "AlarmGerakPIR|arduino:avr:uno"
  "ParkirOtomatis|arduino:avr:uno"
  "KunciPintarKeypad|arduino:avr:uno"
  "StasiunCuacaDHT22|arduino:avr:uno"
  "MonitorSuhuWebESP32|esp32:esp32:esp32"
)

# Filter opsional: nama proyek yang diberikan sebagai argumen
selected=("$@")

lulus=0
gagal=0

for item in "${PROJECTS[@]}"; do
  nama="${item%%|*}"
  fqbn="${item##*|}"

  if [[ ${#selected[@]} -gt 0 ]]; then
    cocok=0
    for s in "${selected[@]}"; do
      [[ "$s" == "$nama" ]] && cocok=1
    done
    [[ "$cocok" -eq 1 ]] || continue
  fi
  proyek="$ROOT/$nama"
  out="$OUT_ROOT/$nama"

  if [[ -n "$TIMEOUT_MS" ]]; then
    timeout_ms="$TIMEOUT_MS"
  elif [[ "$fqbn" == esp32:* ]]; then
    timeout_ms=90000
  else
    timeout_ms=45000
  fi

  echo
  echo "============================================================"
  echo "  $nama ($fqbn)"
  echo "============================================================"

  rm -rf "$out"
  mkdir -p "$out/build"

  cp "$proyek/$nama.ino" "$out/"
  cp "$proyek/diagram.json" "$out/"
  cp "$ROOT/tests/$nama.test.yaml" "$out/scenario.yaml"

  echo "[1/3] Kompilasi..."
  if ! arduino-cli compile --fqbn "$fqbn" --output-dir "$out/build" "$out"; then
    echo "GAGAL: kompilasi $nama" >&2
    gagal=$((gagal + 1))
    continue
  fi

  echo "[2/3] Menyiapkan wokwi.toml..."
  if [[ "$fqbn" == esp32:* ]]; then
    cat > "$out/wokwi.toml" <<EOF
[wokwi]
version = 1
firmware = 'build/$nama.ino.bin'
elf = 'build/$nama.ino.elf'
EOF
  else
    cat > "$out/wokwi.toml" <<EOF
[wokwi]
version = 1
firmware = 'build/$nama.ino.hex'
EOF
  fi

  echo "[3/3] Menjalankan simulasi Wokwi..."
  if wokwi-cli "$out" \
      --scenario "$out/scenario.yaml" \
      --timeout "$timeout_ms" \
      --serial-log-file "$out/serial.log"; then
    echo "LULUS: $nama"
    lulus=$((lulus + 1))
  else
    echo "GAGAL: simulasi $nama (lihat $out/serial.log)" >&2
    gagal=$((gagal + 1))
  fi
done

echo
echo "============================================================"
echo "  Ringkasan: $lulus lulus, $gagal gagal"
echo "  Log: $OUT_ROOT"
echo "============================================================"

[[ "$gagal" -eq 0 ]]
