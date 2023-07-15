#include <WiFi.h>
#include <PubSubClient.h>

// Deklarasi jaringan
const char* ssid = "Galaxy A12 A7B5";
const char* password = "ayub1234";
// Deklarasi koneksi MQTT
const char* mqtt_server = "broker.mqtt-dashboard.com";
int mqtt_port = 1883;
const char* mqtt_topic = "TL";

WiFiClient espClient;
PubSubClient client(espClient);
// Variabel pin jalanSatu
int LedMerah1 = 14;
int LedKuning1 = 27;
int LedHijau1 = 26;
// Variabel pin jalanDua
int LedMerah2 = 25;
int LedKuning2 = 33;
int LedHijau2 = 32;
// Variabel pin jalanTiga
int LedMerah3 = 5;
int LedKuning3 = 18;
int LedHijau3 = 19;
// Variabel pin jalanEmpat
int LedMerah4 = 21;
int LedKuning4 = 22;
int LedHijau4 = 23;

// Durasi waktu untuk setiap fase (dalam milidetik)
int waktuMerah = 5000;
int waktuKuning = 2000;
int waktuHijau = 5000;

// Variable status dan waktu
// Fase saat ini
int currentPhase = 1;
// Waktu sebelumnya
unsigned long previousMillis = 0;

// Variabel lokasi
float longitude = 0;
float latitude = 0;

//rubah sesuai kondisi
float lokasi1awal = 1;
float lokasi2awal = 6;
float lokasi3awal = 1;
float lokasi4awal = 6;

float lokasi1akhir = 5;
float lokasi2akhir = 10;
float lokasi3akhir = 5;
float lokasi4akhir = 10;

bool penanda1 = false;
bool penanda2 = false;
bool penanda3 = false;

void setup() {
  Serial.begin(9600);

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  // Deklarasi pin jalanSatu
  pinMode(LedHijau1, OUTPUT);
  pinMode(LedKuning1, OUTPUT);
  pinMode(LedMerah1, OUTPUT);
  // Deklarasi pin jalanDua
  pinMode(LedHijau2, OUTPUT);
  pinMode(LedKuning2, OUTPUT);
  pinMode(LedMerah2, OUTPUT);
  // Deklarasi pin jalanTiga
  pinMode(LedHijau3, OUTPUT);
  pinMode(LedKuning3, OUTPUT);
  pinMode(LedMerah3, OUTPUT);
  // Deklarasi pin jalanEmpat
  pinMode(LedHijau4, OUTPUT);
  pinMode(LedKuning4, OUTPUT);
  pinMode(LedMerah4, OUTPUT);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  client.loop(); // Proses pesan MQTT
  if (!penanda1) { // Jika lampu menyala dan belum ditandai
    //kondisi awal
    if (!penanda2) { // Jika lampu menyala dan belum ditandai
      if (longitude > lokasi1awal && longitude <= lokasi1akhir) {
        lewatJalanSatu();
        penanda3 = true;
      } else if (latitude > lokasi2awal && latitude <= lokasi2akhir ) {
        lewatJalanDua();
        penanda3 = true;
      } else if (longitude > lokasi3awal && longitude <= lokasi3akhir) {
        lewatJalanTiga();
        penanda3 = true;
      } else if (latitude > lokasi4awal && latitude <= lokasi4akhir) {
        lewatJalanEmpat();
        penanda3 = true;
      } else {
        normal();
        if (penanda3) {
          penanda2 = true;
          lewat();
        }
      }
    } else if (penanda2) {
      penanda1 = true;
    }
  } else if (penanda1) {
    normal();
  }

}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  char message[length + 1];
  memcpy(message, payload, length);
  message[length] = '\0';

  // Tampilkan hasil pengukuran suhu ke LCD
  if (strcmp(topic, "longitude") == 0) {
    longitude = atof(message);
  } else if (strcmp(topic, "latitude") == 0) {
    latitude = atof(message);
  }
  Serial.println(longitude);
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (WiFi.status() == WL_CONNECTED && client.connect(mqtt_topic)) {
      Serial.println("connected");
      client.subscribe("longitude");
      client.subscribe("latitude");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}


//kondisi traffic normal
void normal() {
  // Mendapatkan waktu saat ini
  unsigned long currentMillis = millis();
  // Mengatur fase berdasarkan waktu yang telah berlalu
  if (currentPhase == 1) {
    digitalWrite(LedHijau1, HIGH);
    digitalWrite(LedKuning1, LOW);
    digitalWrite(LedMerah1, LOW);
    digitalWrite(LedMerah2, HIGH);
    digitalWrite(LedHijau2, LOW);
    digitalWrite(LedMerah3, HIGH);
    digitalWrite(LedHijau3, LOW);
    digitalWrite(LedMerah4, HIGH);
    digitalWrite(LedHijau4, LOW);

    if (currentMillis - previousMillis >= waktuHijau) {
      currentPhase = 2;
      previousMillis = currentMillis;
    }
  } else if (currentPhase == 2) {
    digitalWrite(LedKuning2, HIGH);
    digitalWrite(LedMerah2, LOW);

    if (currentMillis - previousMillis >= waktuKuning) {
      currentPhase = 3;
      previousMillis = currentMillis;
    }
  } else if (currentPhase == 3) {
    digitalWrite(LedMerah1, HIGH);
    digitalWrite(LedHijau2, HIGH);
    digitalWrite(LedKuning2, LOW);
    digitalWrite(LedHijau1, LOW);

    if (currentMillis - previousMillis >= waktuMerah) {
      currentPhase = 4;
      previousMillis = currentMillis;
    }
  } else if (currentPhase == 4) {
    digitalWrite(LedMerah2, HIGH);
    digitalWrite(LedHijau2, LOW);
    digitalWrite(LedMerah3, LOW);
    digitalWrite(LedKuning3, HIGH);

    if (currentMillis - previousMillis >= waktuKuning) {
      currentPhase = 5;
      previousMillis = currentMillis;
    }
  } else if (currentPhase == 5) {
    digitalWrite(LedKuning3, LOW);
    digitalWrite(LedHijau3, HIGH);

    if (currentMillis - previousMillis >= waktuHijau) {
      currentPhase = 6;
      previousMillis = currentMillis;
    }
  } else if (currentPhase == 6) {
    digitalWrite(LedMerah3, HIGH);
    digitalWrite(LedHijau3, LOW);
    digitalWrite(LedMerah4, LOW);
    digitalWrite(LedKuning4, HIGH);

    if (currentMillis - previousMillis >= waktuKuning) {
      currentPhase = 7;
      previousMillis = currentMillis;
    }
  } else if (currentPhase == 7) {
    digitalWrite(LedKuning4, LOW);
    digitalWrite(LedHijau4, HIGH);

    if (currentMillis - previousMillis >= waktuHijau) {
      currentPhase = 8;
      previousMillis = currentMillis;
    }
  } else if (currentPhase == 8) {
    digitalWrite(LedMerah4, HIGH);
    digitalWrite(LedHijau4, LOW);
    digitalWrite(LedKuning1, HIGH);
    digitalWrite(LedMerah1, LOW);

    if (currentMillis - previousMillis >= waktuKuning) {
      currentPhase = 1;
      previousMillis = currentMillis;
    }
  }
}
// Kondisi traffic jika melewati jalanSatu
void lewatJalanSatu() {
  digitalWrite(LedMerah1, LOW);
  digitalWrite(LedMerah2, HIGH);
  digitalWrite(LedMerah3, HIGH);
  digitalWrite(LedMerah4, HIGH);

  digitalWrite(LedKuning1, LOW);
  digitalWrite(LedKuning2, LOW);
  digitalWrite(LedKuning3, LOW);
  digitalWrite(LedKuning4, LOW);

  digitalWrite(LedHijau1, HIGH);
  digitalWrite(LedHijau2, LOW);
  digitalWrite(LedHijau3, LOW);
  digitalWrite(LedHijau4, LOW);
}

// Kondisi traffic jika melewati jalanDua
void lewatJalanDua() {
  digitalWrite(LedMerah1, HIGH);
  digitalWrite(LedMerah2, LOW);
  digitalWrite(LedMerah3, HIGH);
  digitalWrite(LedMerah4, HIGH);

  digitalWrite(LedKuning1, LOW);
  digitalWrite(LedKuning2, LOW);
  digitalWrite(LedKuning3, LOW);
  digitalWrite(LedKuning4, LOW);

  digitalWrite(LedHijau1, LOW);
  digitalWrite(LedHijau2, HIGH);
  digitalWrite(LedHijau3, LOW);
  digitalWrite(LedHijau4, LOW);
}

// Kondisi traffic jika melewati jalanTiga
void lewatJalanTiga() {
  digitalWrite(LedMerah1, HIGH);
  digitalWrite(LedMerah2, HIGH);
  digitalWrite(LedMerah3, LOW);
  digitalWrite(LedMerah4, HIGH);

  digitalWrite(LedKuning1, LOW);
  digitalWrite(LedKuning2, LOW);
  digitalWrite(LedKuning3, LOW);
  digitalWrite(LedKuning4, LOW);

  digitalWrite(LedHijau1, LOW);
  digitalWrite(LedHijau2, LOW);
  digitalWrite(LedHijau3, HIGH);
  digitalWrite(LedHijau4, LOW);
}

// Kondisi traffic jika melewati jalanEmpat
void lewatJalanEmpat() {
  digitalWrite(LedMerah1, HIGH);
  digitalWrite(LedMerah2, HIGH);
  digitalWrite(LedMerah3, HIGH);
  digitalWrite(LedMerah4, LOW);

  digitalWrite(LedKuning1, LOW);
  digitalWrite(LedKuning2, LOW);
  digitalWrite(LedKuning3, LOW);
  digitalWrite(LedKuning4, LOW);

  digitalWrite(LedHijau1, LOW);
  digitalWrite(LedHijau2, LOW);
  digitalWrite(LedHijau3, LOW);
  digitalWrite(LedHijau4, HIGH);
}

//kondisi jika sudah melewati
void lewat() {
  digitalWrite(LedHijau1, LOW);
  digitalWrite(LedHijau2, LOW);
  digitalWrite(LedHijau3, LOW);
  digitalWrite(LedHijau4, LOW);
}
