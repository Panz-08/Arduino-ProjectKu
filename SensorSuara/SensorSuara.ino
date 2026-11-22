const int pinSensor = 8;
const int pinLED = 13;

int statusSensorSebelumnya = HIGH;
bool statusLED = LOW;

unsigned long waktuTepukanPertama = 0;
int hitungTepukan = 0;

const unsigned long jedaMaksimalAntarTepukan = 500;
const unsigned long jedaDebounce = 100; 

void setup() {
  Serial.begin(9600);
  Serial.println("=== Detektor Tepukan Ganda ===");
  Serial.println("Tepuk dua kali dalam 500 ms untuk mengubah LED.");

  pinMode(pinSensor, INPUT);
  pinMode(pinLED, OUTPUT);
  digitalWrite(pinLED, statusLED);
}

void loop() {
  int statusSensorSekarang = digitalRead(pinSensor);

  if (statusSensorSekarang == LOW && statusSensorSebelumnya == HIGH) {
    
    if (millis() - waktuTepukanPertama > jedaDebounce) {
      hitungTepukan++;
      
      if (hitungTepukan == 1) {
        waktuTepukanPertama = millis();
        Serial.println("Tepukan 1 terdeteksi...");
      }
      
      else if (hitungTepukan == 2 && (millis() - waktuTepukanPertama <= jedaMaksimalAntarTepukan)) {
        statusLED = !statusLED;
        digitalWrite(pinLED, statusLED);
        hitungTepukan = 0;
        Serial.print("Tepukan 2 terdeteksi - LED ");
        Serial.println(statusLED ? "NYALA" : "MATI");
      }
    }
  }

  statusSensorSebelumnya = statusSensorSekarang;

  if (hitungTepukan > 0 && (millis() - waktuTepukanPertama > jedaMaksimalAntarTepukan)) {
    hitungTepukan = 0; 
  }
}
