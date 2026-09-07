<?php
// ====================================================================
// MATERI GURU WEB & DATABASE (File: api_simpan.php)
// File ini berfungsi untuk menerima data (teks, kecepatan, kecerahan)
// yang dikirim dari form web, dan menyimpannya (UPDATE) ke Database MySQL.
// ====================================================================

// 1. Panggil file koneksi untuk membuka akses ke database.
include 'koneksi.php';

// 2. Pastikan file ini hanya dijalankan ketika ada data yang dikirim melalui metode POST.
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    
    // 3. Menangkap dan Mengamankan Data.
    // mysqli_real_escape_string digunakan untuk mencegah serangan Hacker (SQL Injection).
    // (int) digunakan untuk memastikan tipe data berupa angka bulat (Integer).
    $teks = mysqli_real_escape_string($conn, $_POST['teks']);
    $brightness = (int)$_POST['brightness'];
    $speed = (int)$_POST['speed'];
    $mode = (int)$_POST['mode'];
    
    // 4. Menulis Perintah SQL.
    // Kita gunakan UPDATE untuk memperbarui baris data yang ada pada id=1 (kita tidak menggunakan INSERT karena alat hanya membaca 1 data seting).
    $sql = "UPDATE setting_p10 SET teks='$teks', brightness=$brightness, speed=$speed, mode=$mode WHERE id=1";
    
    // 5. Eksekusi perintah SQL.
    if (mysqli_query($conn, $sql)) {
        // Jika sukses di-update di MySQL, kirimkan pesan sukses.
        echo "Tersimpan di Database MySQL";
    } else {
        // Jika gagal, tampilkan pesan error dari MySQL.
        echo "Gagal: " . mysqli_error($conn);
    }
}
?>
