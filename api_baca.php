<?php
// ====================================================================
// MATERI GURU WEB & IOT (File: api_baca.php)
// File ini berfungsi sebagai API (Application Programming Interface).
// Tugasnya adalah membaca data dari Database MySQL dan mengirimkannya 
// kembali dalam bentuk format JSON (format standar pertukaran data).
// ====================================================================

// 1. Mengatur header agar browser tahu bahwa outputnya adalah JSON, bukan HTML biasa.
header('Content-Type: application/json');

// 2. Memanggil file koneksi untuk menghubungkan ke database.
include 'koneksi.php';

// 3. Menyiapkan perintah SQL untuk mengambil (SELECT) data dari tabel 'setting_p10' 
// khusus untuk id=1 (karena kita hanya menyimpan satu konfigurasi alat).
$sql = "SELECT * FROM setting_p10 WHERE id=1";

// 4. Menjalankan perintah SQL ke database.
$result = mysqli_query($conn, $sql);

// 5. Mengecek apakah data ditemukan.
if ($result && mysqli_num_rows($result) > 0) {
    // Jika data ada, ubah bentuk data SQL (Array Assosiatif) menjadi JSON, lalu tampilkan (echo).
    echo json_encode(mysqli_fetch_assoc($result));
} else {
    // Jika data tidak ditemukan atau tabel kosong, kirimkan nilai default (bawaan) dalam bentuk JSON.
    echo json_encode(["teks"=>"KOSONG", "brightness"=>30, "speed"=>40, "mode"=>1]);
}
?>
