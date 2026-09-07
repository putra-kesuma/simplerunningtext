<?php
// ====================================================================
// MATERI GURU WEB & DATABASE (File: api_status_db.php)
// File ini khusus dibuat untuk mengecek status koneksi dari Web Hosting
// ====================================================================
header('Content-Type: application/json');

// Kita langsung panggil file koneksi.php agar tidak perlu setting password 2 kali.
// Jika koneksi.php berhasil, maka kode di bawahnya akan dieksekusi.
// Jika gagal, file koneksi.php akan mengeluarkan 'die()' dan membatalkan sisa kode ini.

require 'koneksi.php';

// Jika sampai di baris ini, berarti koneksi di koneksi.php SUKSES.
echo json_encode(["status" => "online"]);
?>
