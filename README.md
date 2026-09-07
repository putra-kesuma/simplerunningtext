# PANDUAN KOLABORASI PROJECT IOT P10 MQTT

Project ini adalah contoh nyata penerapan **Internet of Things (IoT)** yang memadukan 3 bidang keahlian (Database, Web Programming, dan Hardware IoT).

Dokumen ini adalah panduan pembagian tugas untuk Guru/Siswa agar proses pembelajaran lebih terstruktur.

---

## 1. 🗄️ TUGAS GURU/SISWA DATABASE
**Fokus Utama:** Merancang, mengonfigurasi, dan memastikan data dapat disimpan dengan baik di dalam server (MySQL).

### Langkah Praktik:
1. Pastikan XAMPP (Apache dan MySQL) sudah berjalan.
2. Buka **phpMyAdmin** (`http://localhost/phpmyadmin`).
3. Import file `db_iot.sql` yang sudah disediakan di folder ini.
4. **Materi yang bisa diajarkan:**
   * Konsep relasi database (jika nanti dikembangkan dengan banyak alat).
   * Perintah DDL (Data Definition Language) seperti `CREATE TABLE`.
   * Perintah DML (Data Manipulation Language) seperti `SELECT` (di file `api_baca.php`) dan `UPDATE` (di file `api_simpan.php`).
5. **Bedah Kode:** Buka file `koneksi.php` dan perhatikan komentar yang ada di sana untuk dipelajari bersama siswa.

---

## 2. 🌐 TUGAS GURU/SISWA WEB PROGRAMMING
**Fokus Utama:** Membuat antarmuka pengguna (UI/UX) dan API untuk menjembatani antara database dan MQTT broker.

### Langkah Praktik:
1. **Materi Front-end:** 
   * Buka file `index.html`. Pelajari penggunaan framework **Bootstrap 5** untuk membuat tampilan rapi dan responsif.
   * Modifikasi UI, misalnya menambahkan logo sekolah atau merubah warna tombol.
2. **Materi Back-end (PHP API):**
   * Pelajari file `api_baca.php` (Cara merubah data MySQL menjadi format JSON).
   * Pelajari file `api_simpan.php` (Cara menerima data form dengan metode POST dan mencegah SQL Injection menggunakan `mysqli_real_escape_string`).
3. **Materi Javascript (AJAX & Fetch API):**
   * Di dalam file `index.html`, pelajari blok `<script>` bagian `fetch()`. Ini adalah teknik mengirim dan membaca data tanpa harus me-reload halaman web.

---

## 3. 🤖 TUGAS GURU/SISWA IOT & HARDWARE
**Fokus Utama:** Merakit hardware, menghubungkan mikrokontroler ke internet, dan menghubungkannya dengan protokol komunikasi MQTT.

### Langkah Praktik:
1. **Persiapan Hardware:**
   * Rakit ESP8266 (NodeMCU) dengan Panel LED P10.
2. **Persiapan Software (Arduino IDE):**
   * Buka file `p10_mqtt/p10_mqtt.ino` menggunakan Arduino IDE.
   * Pastikan sudah menginstall library berikut melalui *Library Manager*:
     * `PubSubClient` (oleh Nick O'Leary) - Untuk MQTT
     * `DMDESP` - Untuk kontrol Panel P10
     * `ArduinoJson` - Untuk membaca data JSON
3. **Materi Inti (MQTT & WiFi):**
   * Sesuaikan `ssid` dan `password` WiFi di kodingan dengan hotspot yang ada di lokasi.
   * Jelaskan konsep **MQTT Broker** (`broker.emqx.io`). MQTT sangat cepat (Real-time) karena menggunakan metode *Publish/Subscribe*, bukan request/response biasa seperti HTTP.
   * Jelaskan bagian `mqtt_topic`. Topic ini ibarat "frekuensi radio" tempat web dan alat berkomunikasi.
4. **Materi Advanced (LittleFS):**
   * Pada kodingan Arduino, jelaskan bagaimana alat menyimpan memori terakhir (`LittleFS`) sehingga saat mati lampu, teks terakhir tidak hilang meski tidak ada koneksi internet.

---

## 🚀 CARA MENDEMONSTRASIKAN PROJECT SECARA BERSAMAAN:
1. **Guru Database** memastikan XAMPP menyala dan tabel `setting_p10` sudah ada datanya.
2. **Guru IoT** menyalakan alat P10 dan memastikan alat terhubung ke WiFi dan menampilkan status "Menunggu MQTT / Koneksi Sukses".
3. **Guru Web** membuka alamat web `http://localhost/iotjamapi_mqtt`.
4. Ubah teks, kecepatan, atau kecerahan di Web, lalu klik **"KIRIM REAL-TIME"**.
5. **BUM!** Teks di P10 langsung berubah secara instan (berkat MQTT), dan jika halaman web di-refresh, nilai di form tidak kembali kosong (berkat Database MySQL).

*Catatan: Seluruh kode (PHP, HTML, JS, dan Arduino) telah diberikan komentar edukatif yang sangat lengkap. Silakan buka masing-masing file untuk mempelajarinya lebih dalam.*
