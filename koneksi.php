<?php
// ====================================================================
// MATERI GURU DATABASE & WEB (File: koneksi.php)
// File ini berfungsi sebagai "Jembatan" antara aplikasi web PHP
// dengan sistem Database MySQL. Tanpa file ini, web tidak bisa menyimpan data.
// ====================================================================

// 1. Konfigurasi Server Database
$host = "localhost";  // Nama server (karena menggunakan XAMPP di komputer sendiri, nilainya 'localhost')
$user = "root";       // Username default dari XAMPP untuk mengakses MySQL
$pass = "";           // Password default XAMPP adalah kosong (tidak ada password)
$db   = "db_iot";     // Nama database yang kita buat di phpMyAdmin untuk project ini

// 2. Membuka Koneksi
// Fungsi mysqli_connect() mencoba membuka jalur komunikasi dengan data di atas.
$conn = mysqli_connect($host, $user, $pass, $db);

// 3. Pengecekan Status Koneksi
// Jika koneksi gagal (misal XAMPP MySQL belum di-start, atau nama database salah),
// maka hentikan seluruh sistem (die) dan tampilkan pesan error.
if (!$conn) { 
    die("Koneksi Database Gagal: Periksa apakah MySQL di XAMPP sudah menyala dan nama database benar."); 
}
?>
