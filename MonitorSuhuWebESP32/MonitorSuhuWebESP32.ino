/*
 * Monitor Suhu Web ESP32
 * ----------------------
 * ESP32 membaca DHT22 lalu menyajikan dashboard web sederhana.
 *
 * Fitur:
 *   - Koneksi WiFi simulasi Wokwi (SSID "Wokwi-GUEST", tanpa password).
 *   - Endpoint "/" menampilkan dashboard HTML.
 *   - Endpoint "/api/data" mengembalikan JSON suhu, kelembapan, status LED.
 *   - Endpoint "/api/led?state=on|off|toggle" mengontrol LED.
 *   - Buzzer berbunyi bila suhu >= 35 derajat Celsius.
 *
 * Board  : ESP32 DevKit C V4
 * Wokwi  : WiFi virtual Wokwi-GUEST (hanya bekerja di dalam Wokwi)
 * Serial : 115200 baud
 */

#include <WiFi.h>
#include <WebServer.h>
#include <DHTesp.h>

const char *SSID = "Wokwi-GUEST";
const char *PASSWORD = "";

const uint8_t PIN_DHT    = 15;
const uint8_t PIN_LED    = 2;
const uint8_t PIN_BUZZER = 5;

const float BATAS_PANAS = 35.0;
const unsigned long JEDA_BACA  = 2000;
const unsigned long JEDA_BUNYI = 1000;

DHTesp dht;
WebServer server(80);

float suhu = 0.0;
float kelembapan = 0.0;
bool ledNyala = false;
bool sensorSiap = false;
bool panasSebelumnya = false;

unsigned long waktuBaca = 0;
unsigned long waktuBunyi = 0;

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="id">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Monitor Suhu ESP32</title>
  <style>
    body { font-family: system-ui, sans-serif; background: #0f172a; color: #e2e8f0; margin: 0; min-height: 100vh; display: grid; place-items: center; }
    .card { background: #1e293b; padding: 2rem; border-radius: 1rem; box-shadow: 0 20px 40px rgba(0,0,0,.4); width: min(90vw, 420px); }
    h1 { font-size: 1.25rem; margin: 0 0 1rem; color: #38bdf8; }
    .grid { display: grid; grid-template-columns: 1fr 1fr; gap: 1rem; }
    .box { background: #0f172a; border-radius: .75rem; padding: 1rem; text-align: center; }
    .value { font-size: 2rem; font-weight: 700; }
    .label { font-size: .8rem; color: #94a3b8; text-transform: uppercase; letter-spacing: .08em; }
    .status { margin-top: 1rem; padding: .75rem; border-radius: .75rem; background: #14532d; color: #bbf7d0; text-align: center; font-weight: 600; }
    .status.hot { background: #7f1d1d; color: #fecaca; }
    button { margin-top: 1rem; width: 100%; padding: .9rem; border: 0; border-radius: .75rem; background: #2563eb; color: white; font-size: 1rem; font-weight: 600; cursor: pointer; }
    button:hover { background: #1d4ed8; }
  </style>
</head>
<body>
  <div class="card">
    <h1>ESP32 Monitor Suhu</h1>
    <div class="grid">
      <div class="box"><div class="value" id="suhu">--</div><div class="label">Suhu (&deg;C)</div></div>
      <div class="box"><div class="value" id="kelembapan">--</div><div class="label">Kelembapan (%)</div></div>
    </div>
    <div class="status" id="status">Menghubungkan...</div>
    <button onclick="toggleLed()">Toggle LED</button>
  </div>
  <script>
    async function muat() {
      try {
        const r = await fetch('/api/data');
        const d = await r.json();
        document.getElementById('suhu').textContent = d.suhu.toFixed(1);
        document.getElementById('kelembapan').textContent = d.kelembapan.toFixed(1);
        const s = document.getElementById('status');
        s.textContent = d.panas ? 'PANAS! Suhu di atas 35 °C' : (d.led ? 'LED menyala' : 'Kondisi normal');
        s.className = d.panas ? 'status hot' : 'status';
      } catch (e) {
        document.getElementById('status').textContent = 'Gagal membaca data';
      }
    }
    async function toggleLed() {
      await fetch('/api/led?state=toggle');
      muat();
    }
    muat();
    setInterval(muat, 2000);
  </script>
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send_P(200, "text/html", INDEX_HTML);
}

void handleData() {
  String json = "{";
  json += "\"suhu\":" + String(suhu, 1) + ",";
  json += "\"kelembapan\":" + String(kelembapan, 1) + ",";
  json += "\"led\":" + String(ledNyala ? "true" : "false") + ",";
  json += "\"panas\":" + String((suhu >= BATAS_PANAS) ? "true" : "false");
  json += "}";
  server.send(200, "application/json", json);
}

void handleLed() {
  String state = server.arg("state");

  if (state == "on") {
    ledNyala = true;
  } else if (state == "off") {
    ledNyala = false;
  } else {
    ledNyala = !ledNyala;
  }

  digitalWrite(PIN_LED, ledNyala ? HIGH : LOW);
  Serial.print("LED -> ");
  Serial.println(ledNyala ? "ON" : "OFF");

  String json = String("{\"led\":") + (ledNyala ? "true" : "false") + "}";
  server.send(200, "application/json", json);
}

void handleNotFound() {
  server.send(404, "text/plain", "Endpoint tidak ditemukan");
}

void bacaSensor() {
  TempAndHumidity data = dht.getTempAndHumidity();

  if (dht.getStatus() != DHTesp::ERROR_NONE) {
    sensorSiap = false;
    Serial.print("Gagal membaca DHT22, status: ");
    Serial.println(dht.getStatusString());
    return;
  }

  suhu = data.temperature;
  kelembapan = data.humidity;
  sensorSiap = true;

  Serial.print("Suhu=");
  Serial.print(suhu, 1);
  Serial.print(" C, Kelembapan=");
  Serial.print(kelembapan, 1);
  Serial.println(" %");
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("=== Monitor Suhu Web ESP32 ===");

  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  dht.setup(PIN_DHT, DHTesp::DHT22);

  WiFi.begin(SSID, PASSWORD, 6);
  Serial.print("Menghubungkan ke WiFi");
  unsigned long mulai = millis();
  while (WiFi.status() != WL_CONNECTED && (millis() - mulai < 15000)) {
    delay(300);
    Serial.print(".");
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("WiFi terhubung. Buka http://");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("WiFi gagal terhubung. Pastikan memakai Wokwi.");
  }

  server.on("/", handleRoot);
  server.on("/api/data", handleData);
  server.on("/api/led", handleLed);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("Web server aktif pada port 80.");
}

void loop() {
  server.handleClient();

  if (millis() - waktuBaca >= JEDA_BACA) {
    waktuBaca = millis();
    bacaSensor();
  }

  bool panas = sensorSiap && (suhu >= BATAS_PANAS);

  if (panas) {
    if (millis() - waktuBunyi >= JEDA_BUNYI) {
      waktuBunyi = millis();
      tone(PIN_BUZZER, 2200, 100);
    }
    if (!panasSebelumnya) {
      Serial.println("PERINGATAN: suhu di atas batas 35 C!");
    }
  } else {
    noTone(PIN_BUZZER);
    if (panasSebelumnya) {
      Serial.println("Suhu kembali normal.");
    }
  }

  panasSebelumnya = panas;
}
