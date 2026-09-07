-- ==========================================================
-- FILE DATABASE UNTUK PROJECT IOT P10 MQTT
-- ==========================================================

-- 1. Buat Database
CREATE DATABASE IF NOT EXISTS `db_iot`;
USE `db_iot`;

-- 2. Buat Struktur Tabel (Hanya dijalankan jika tabel belum ada)
CREATE TABLE IF NOT EXISTS `setting_p10` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `teks` varchar(255) NOT NULL,
  `brightness` int(11) NOT NULL,
  `speed` int(11) NOT NULL,
  `mode` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- 3. Hapus data lama (jika ada) dan Masukkan Data Awal (Default)
TRUNCATE TABLE `setting_p10`;
INSERT INTO `setting_p10` (`id`, `teks`, `brightness`, `speed`, `mode`) VALUES
(1, 'Selamat Datang di Sekolah', 30, 40, 1);
