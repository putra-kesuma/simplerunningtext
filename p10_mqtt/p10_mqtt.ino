// ====================================================================
// MATERI GURU IOT & HARDWARE (File: p10_mqtt.ino)
// File ini adalah program (Firmware) yang di-upload ke alat/modul ESP8266/NodeMCU.
// Fungsinya: Terhubung ke WiFi, menyambung ke server MQTT, mendengarkan 
// pesan (perintah) dari Web, lalu menampilkannya di Panel LED P10.
// ====================================================================

#include <ESP8266WiFi.h>  // Library wajib agar alat bisa pakai WiFi
#include <PubSubClient.h> // GURU IOT: Wajib install library PubSubClient by Nick O'Leary (Untuk MQTT)
#include <DMDESP.h>       // Library untuk mengontrol nyala/mati lampu di Panel P10
#include <fonts/Mono5x7.h>// Font (Bentuk Huruf) untuk P10
#include <Ticker.h>       // Library Timer bawaan ESP (Agar P10 tidak kedip-kedip saat loading)
#include <ArduinoJson.h>  // Wajib install ArduinoJson by Benoit (Untuk membaca pesan JSON dari web)
#include <LittleFS.h>     // Sistem memori internal untuk menyimpan setting cadangan saat mati lampu

// 1. SETTING WIFI (Ubah sesuai hotspot/wifi di sekolah)
const char* ssid = "olrait";
const char* password = "12345678";

// --- 2. KONFIGURASI MQTT (Materi Inti IoT) ---
// Harus SAMA PERSIS dengan settingan yang ada di file index.html pada bagian Web
const char* mqtt_server = "broker.emqx.io";
const int mqtt_port = 1883; // Port 1883 adalah standar koneksi MQTT (Tanpa SSL/WebSockets) untuk alat hardware
const char* mqtt_topic = "sekolah/iot/p10/data"; // "Saluran" komunikasi (Harus Sama dengan Web)

WiFiClient espClient;
PubSubClient mqttClient(espClient);
// ------------------------

#define DISPLAYS_WIDE 1
#define DISPLAYS_HIGH 1
DMDESP Disp(DISPLAYS_WIDE, DISPLAYS_HIGH);

String teks_berjalan = "MENUNGGU MQTT...";
int posisi_X;
int panjang_layar;
int mode_tampilan = 1; 
int kecepatan_scroll = 40; 
int tingkat_kecerahan = 30; 

volatile boolean teksSedangDiupdate = false;

// --- Ticker untuk Refresh P10 di Background ---
Ticker timerP10;
void refreshP10() {
  Disp.loop(); 
  
  static unsigned long waktuScroll = 0;
  if(millis() - waktuScroll > kecepatan_scroll) { 
    waktuScroll = millis();
    
    if (!teksSedangDiupdate) {
      Disp.clear(); 
      if (mode_tampilan == 1) {
        Disp.drawText(posisi_X, 4, teks_berjalan); 
        int panjang_teks = teks_berjalan.length() * 6;
        posisi_X--;
        if (posisi_X < -panjang_teks) {
          posisi_X = panjang_layar;
        }
      } else if (mode_tampilan == 2) {
        int panjang_teks = teks_berjalan.length() * 6;
        int tengah_X = (panjang_layar - panjang_teks) / 2;
        Disp.drawText(tengah_X, 4, teks_berjalan); 
        posisi_X = panjang_layar; 
      }
    }
  }
}
// ----------------------------------------------

// Fungsi yang dipanggil OTOMATIS saat ada pesan baru dari Web Dashboard
void saatPesanMqttMasuk(char* topic, byte* payload, unsigned int length) {
  // Ubah payload dari byte array menjadi string
  String pesanMasuk = "";
  for (int i = 0; i < length; i++) {
    pesanMasuk += (char)payload[i];
  }
  
  Serial.println("Pesan Masuk (Real-Time): " + pesanMasuk);

  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, pesanMasuk);
  
  if (!error) {
    teksSedangDiupdate = true; // Kunci agar tidak error dengan Ticker
    
    String teks_baru = doc["teks"].as<String>();
    if(teks_baru != "" && teks_baru != teks_berjalan) {
       teks_berjalan = teks_baru;
    }
    
    tingkat_kecerahan = doc["brightness"];
    Disp.setBrightness(tingkat_kecerahan);
    
    kecepatan_scroll = doc["speed"];
    mode_tampilan = doc["mode"];
    
    teksSedangDiupdate = false; // Buka kunci

    // Simpan ke LittleFS sebagai cadangan offline
    File f = LittleFS.open("/config.json", "w");
    if (f) {
      f.print(pesanMasuk);
      f.close();
    }
  }
}

// Fungsi untuk menghubungkan ke Broker MQTT
void konekMQTT() {
  // Loop terus sampai terhubung
  while (!mqttClient.connected()) {
    Serial.print("Menghubungkan ke MQTT...");
    String clientId = "AlatP10_" + String(random(0xffff), HEX);
    
    // Menggunakan fitur LWT (Last Will and Testament)
    // Jika alat mati/putus koneksi tiba-tiba, Broker akan otomatis mengirim pesan "offline" ke Web
    const char* topic_status = "sekolah/iot/p10/status";
    
    // Parameter connect: (clientId, username, password, willTopic, willQoS, willRetain, willMessage)
    if (mqttClient.connect(clientId.c_str(), NULL, NULL, topic_status, 0, true, "offline")) {
      Serial.println(" BERHASIL!");
      
      // Beritahu Web bahwa alat sekarang sedang ONLINE (Kirim dengan Retain=true)
      mqttClient.publish(topic_status, "online", true);
      
      // Daftar (Subscribe) ke topik utama untuk menerima teks dari web
      mqttClient.subscribe(mqtt_topic);
    } else {
      Serial.print(" GAGAL, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" Coba lagi dalam 5 detik");
      delay(5000); // Tunggu 5 detik sebelum mencoba konek lagi
    }
  }
}

void setup() {
  Serial.begin(115200);
  
  // -- BACA DATA CADANGAN SAAT OFFLINE --
  if (LittleFS.begin() && LittleFS.exists("/config.json")) {
    File file = LittleFS.open("/config.json", "r");
    if (file) {
      String dataTersimpan = file.readString();
      file.close();
      StaticJsonDocument<256> doc;
      if (!deserializeJson(doc, dataTersimpan)) {
         teks_berjalan = doc["teks"].as<String>();
         tingkat_kecerahan = doc["brightness"];
         kecepatan_scroll = doc["speed"];
         mode_tampilan = doc["mode"];
      }
    }
  }

  Disp.start();
  Disp.setBrightness(tingkat_kecerahan);
  Disp.setFont(Mono5x7);
  panjang_layar = Disp.width();
  posisi_X = panjang_layar;     
  
  timerP10.attach_ms(2, refreshP10);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi OK!");

  // Atur server MQTT
  mqttClient.setServer(mqtt_server, mqtt_port);
  mqttClient.setCallback(saatPesanMqttMasuk);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    // Jika koneksi MQTT putus, sambungkan ulang
    if (!mqttClient.connected()) {
      konekMQTT();
    }
    // Fungsi INTI: Menjaga koneksi tetap hidup secara non-blocking
    mqttClient.loop();
  }
}
