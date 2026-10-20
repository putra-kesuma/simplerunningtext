# Arsitektur Jaringan: Klien, Cloud, dan Server Hosting

Diagram ini berfokus pada topologi jaringan (Network Architecture) yang memperlihatkan bagaimana perangkat di sisi klien berkomunikasi hingga ke server hosting dan server MQTT.

```mermaid
flowchart TD
    %% Definisi Sisi Klien
    subgraph Sisi_Klien ["1. Sisi Klien / Client Side"]
        UserClient["Client Browser (HP/PC/Laptop)"]
        IoTClient["Perangkat Klien IoT (ESP8266 & P10)"]
    end

    %% Definisi Sisi Cloud / Jembatan
    subgraph Sisi_Cloud ["2. Sisi Cloud Publik (Jalur Real-time)"]
        MQTT(("Publik MQTT Broker (broker.emqx.io)"))
    end

    %% Definisi Sisi Hosting
    subgraph Sisi_Hosting ["3. Sisi Server Hosting / Web Server"]
        WebFile["Web Pages (index.html)"]
        PHPAPI["Script Backend (api_simpan & api_baca)"]
        Database[("Database Server (MySQL)")]
    end

    %% Alur dari Client Browser ke Hosting
    UserClient -- "A1. Mengakses Halaman Web (HTTP GET)" --> WebFile
    UserClient -- "A2. Mengambil Status Terakhir (AJAX GET)" --> PHPAPI
    UserClient -- "A3. Menyimpan Pengaturan Baru (AJAX POST)" --> PHPAPI
    PHPAPI -- "A4. Query (SELECT / UPDATE)" --> Database

    %% Alur Realtime Klien ke Klien (Melewati Cloud)
    UserClient == "B1. Kirim Perintah Cepat (WebSocket Publish)" ==> MQTT
    MQTT == "B2. Pesan Diterima & Diteruskan (Subscribe)" ==> IoTClient

    %% Note untuk memperjelas bahwa IoT Client tidak langsung membebani hosting
    IoTClient -. "Tidak berkomunikasi ke Hosting langsung" .- PHPAPI
    
    %% Warna Tema
    classDef client fill:#e1f5fe,stroke:#03a9f4,stroke-width:2px,color:black;
    classDef hosting fill:#e8f5e9,stroke:#4caf50,stroke-width:2px,color:black;
    classDef cloud fill:#fff3e0,stroke:#ff9800,stroke-width:2px,color:black;
    
    class UserClient,IoTClient client;
    class WebFile,PHPAPI,Database hosting;
    class MQTT cloud;
```

### 💡 Analisis Arsitektur:

1. **Client Browser (HP/Laptop) ke Server Hosting:**
   * Klien membuka browser dan meminta halaman `index.html` dari Server Hosting.
   * Saat pertama kali dimuat, browser melakukan proses **AJAX/API Call** ke Server Hosting (`api_baca.php`) untuk melihat data terakhir di Database MySQL.
   * Saat pengguna menekan "Kirim", browser akan melakukan proses **AJAX POST** ke Server Hosting (`api_simpan.php`) untuk memperbarui data MySQL.

2. **Perangkat Alat IoT Klien (ESP8266) Tidak Menghubungi Hosting:**
   * Untuk menghindari Server Hosting dari beban *request* berulang setiap detiknya (yang sering menyebabkan server/hosting down), **Alat IoT tidak pernah meminta data langsung ke PHP/MySQL**.

3. **Peran Jembatan MQTT (Cloud Publik):**
   * Sebagai gantinya, komunikasi dari Browser Klien ke Alat Klien dilewatkan melalui pihak ketiga, yaitu **MQTT Broker (broker.emqx.io)**.
   * Ini memastikan respon pengiriman sangat cepat (*real-time*) dan tidak membebani limit *bandwidth* maupun *resource* dari Server Hosting Anda.
