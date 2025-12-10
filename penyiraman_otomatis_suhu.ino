// Program Arduino: Penyiraman Otomatis berdasarkan Suhu
// Sensor suhu (misalnya LM35) pada pin A0
// Relay/pompa air pada pin D8

const int SENSOR_SUHU_PIN = A0;    // Pin analog untuk sensor suhu
const int RELAY_POMPA_PIN = 8;     // Pin digital untuk mengontrol relay/pompa

// Atur batas suhu dalam derajat Celcius
const float BATAS_SUHU = 30.0;     // Jika suhu di atas 30°C, pompa menyala

void setup() {
  Serial.begin(9600);

  pinMode(SENSOR_SUHU_PIN, INPUT);
  pinMode(RELAY_POMPA_PIN, OUTPUT);

  // Pastikan pompa mati di awal
  digitalWrite(RELAY_POMPA_PIN, LOW);
}

void loop() {
  // Membaca nilai analog dari sensor
  int nilaiAnalog = analogRead(SENSOR_SUHU_PIN);

  // Konversi ke tegangan (asumsi referensi 5V dan 10-bit ADC)
  float tegangan = nilaiAnalog * (5.0 / 1023.0);

  // Konversi tegangan ke suhu (untuk LM35: 10 mV per °C, atau 0.01 V per °C)
  float suhu = tegangan / 0.01;   // Contoh: 0.30 V ≈ 30°C

  Serial.print("Nilai analog: ");
  Serial.print(nilaiAnalog);
  Serial.print(" | Tegangan: ");
  Serial.print(tegangan);
  Serial.print(" V | Suhu: ");
  Serial.print(suhu);
  Serial.println(" °C");

  // Logika penyiraman otomatis
  if (suhu > BATAS_SUHU) {
    // Suhu di atas batas -> nyalakan pompa
    digitalWrite(RELAY_POMPA_PIN, HIGH);
    Serial.println("Pompa: MENYALA (suhu di atas batas)");
  } else {
    // Suhu di bawah/di sama dengan batas -> matikan pompa
    digitalWrite(RELAY_POMPA_PIN, LOW);
    Serial.println("Pompa: MATI (suhu aman)");
  }

  // Tunggu sebentar sebelum pembacaan berikutnya
  delay(1000); // 1 detik
}
