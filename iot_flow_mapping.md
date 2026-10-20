# Visual Mapping Alur Sistem IoT P10 MQTT

Berikut adalah diagram alur kerja dari project IoT P10 MQTT. Diagram ini mengilustrasikan bagaimana 3 komponen utama (Web Frontend, Database/Backend, dan Hardware IoT) saling berinteraksi secara *real-time*.

```mermaid
flowchart TD
    User(["Pengguna"]) -->|"1. Isi Form & Klik 'Kirim Real-Time'"| WebApp

    subgraph Frontend ["1. Antarmuka Web"]
        WebApp["Web Browser (index.html)"]
    end

    subgraph Cloud ["2. Jalur Cepat / Real-Time"]
        MQTT(("MQTT Broker (broker.emqx.io)"))
        WebApp == "2a. Publish JSON via WebSockets" ==> MQTT
        MQTT -. "Menyimpan pesan terakhir (Retain)" .-> MQTT
    end

    subgraph Backend ["3. Jalur Penyimpanan / Database"]
        API_Simpan["api_simpan.php"]
        DB[("MySQL Database (db_iot.sql)")]
        API_Baca["api_baca.php"]
        
        WebApp -- "2b. AJAX POST Background" --> API_Simpan
        API_Simpan -- "UPDATE data" --> DB
        DB -- "SELECT data" --> API_Baca
        API_Baca -. "4. Fetch GET Saat halaman dimuat" .-> WebApp
    end

    subgraph Hardware ["4. Perangkat P10 MQTT"]
        ESP["Mikrokontroler ESP8266 & Panel LED P10"]
        LFS[("LittleFS (Memori Lokal)")]
        
        MQTT == "3. Subscribe & Terima JSON" ==> ESP
        ESP -- "Backup text otomatis" --> LFS
        LFS -. "Tampil saat restart tanpa koneksi" .-> ESP
    end
    
    %% Styling Colors
    classDef frontend fill:#e1f5fe,stroke:#03a9f4,stroke-width:2px,color:black;
    classDef backend fill:#e8f5e9,stroke:#4caf50,stroke-width:2px,color:black;
    classDef cloud fill:#fff3e0,stroke:#ff9800,stroke-width:2px,color:black;
    classDef hardware fill:#fce4ec,stroke:#e91e63,stroke-width:2px,color:black;
    
    class WebApp frontend;
    class API_Simpan,API_Baca,DB backend;
    class MQTT cloud;
    class ESP,LFS hardware;
```

### 💡 Penjelasan Alur Cara Kerja Sistem:

1. **Input Pengguna:** 
   Pengguna membuka halaman web `index.html`, mengisi pengaturan teks (seperti kecepatan dan kecerahan), lalu menekan tombol **Kirim Real-Time**.
2. **Pengiriman Ganda Secara Bersamaan:** 
   * **(2a) Jalur Cepat IoT:** Web *(Javascript)* langsung mem-publish data ke server MQTT (`broker.emqx.io`) menggunakan protokol WebSocket. Jalur ini sangat responsif dan melesat dalam hitungan milidetik.
   * **(2b) Jalur Penyimpanan Web:** Secara bersamaan dan diam-diam *(menggunakan AJAX)*, web juga mengirimkan data tersebut ke `api_simpan.php` untuk disimpan secara permanen di dalam Database MySQL (`db_iot.sql`).
3. **Penerimaan Real-Time di Hardware:** 
   Hardware NodeMCU ESP8266 yang sudah terkoneksi ke internet terus memantau (*subscribe*) dari Broker MQTT. Begitu data dari jalur cepat (2a) masuk ke MQTT, ESP8266 akan langsung menerimanya dan mengubah tampilan di Panel LED P10 detik itu juga.
4. **Sistem Pemulihan & Backup Data (Agar Data Tidak Hilang):**
   * **Untuk Web:** Ketika pengguna merefresh web atau membukanya keesokan hari, halaman tidak akan kosong. Web akan memanggil `api_baca.php` untuk menarik memori terakhir yang tersimpan di MySQL, sehingga formulir web kembali terisi sesuai dengan pengaturan terakhir alat.
   * **Untuk Hardware IoT:** Pesan MQTT menggunakan fitur *Retain*, yang berarti MQTT secara otomatis mencadangkan pesan terakhir. Selain itu, ESP8266 juga menyimpan pesan di memori lokalnya (*LittleFS*). Jika panel LED mati listrik dan menyala kembali, ia dapat menampilkan teks terakhirnya, bahkan ketika internet sedang putus sekalipun.
