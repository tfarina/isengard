-- phpMyAdmin SQL Dump
-- version 3.5.2.2
-- http://www.phpmyadmin.net
--
-- Host: 127.0.0.1
-- Generation Time: Dec 03, 2015 at 09:16 PM
-- Server version: 5.5.27
-- PHP Version: 5.4.7

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

CREATE TABLE IF NOT EXISTS `articles` (
  `id` int(20) NOT NULL AUTO_INCREMENT,
  `text` varchar(1000) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=66;

--
-- Dumping data for table `articles`
--

INSERT INTO `articles` (`id`, `text`) VALUES
(1, 'article 1'),
(2, 'article 2'),
(3, 'article 3'),
(4, 'article 4'),
(5, 'article 5'),
(6, 'article 6'),
(7, 'article 7'),
(8, 'article 8'),
(9, 'article 9'),
(10, 'article 10'),
(11, 'article 11'),
(12, 'article 12'),
(13, 'article 13'),
(14, 'article 14'),
(15, 'article 15'),
(16, 'article 16'),
(17, 'article 17'),
(18, 'article 18'),
(19, 'article 19'),
(20, 'article 20'),
(21, 'article 21'),
(22, 'article 22'),
(23, 'article 23'),
(24, 'article 24'),
(25, 'article 25'),
(26, 'article 26'),
(27, 'article 27'),
(28, 'article 28'),
(29, 'article 29'),
(30, 'article 30'),
(31, 'article 31'),
(32, 'article 32'),
(33, 'article 33'),
(34, 'article 34'),
(35, 'article 35'),
(36, 'article 36'),
(37, 'article 37'),
(38, 'article 38'),
(39, 'article 39'),
(40, 'article 40'),
(41, 'article 41'),
(42, 'article 42'),
(43, 'article 43'),
(44, 'article 44'),
(45, 'article 45'),
(46, 'article 46'),
(47, 'article 47'),
(48, 'article 48'),
(49, 'article 49'),
(50, 'article 50'),
(51, 'article 51'),
(52, 'article 52'),
(53, 'article 53'),
(54, 'article 54'),
(55, 'article 55'),
(56, 'article 56'),
(57, 'article 57'),
(58, 'article 58'),
(59, 'article 59'),
(60, 'article 60'),
(61, 'article 61'),
(62, 'article 62'),
(63, 'article 63'),
(64, 'article 64'),
(65, 'article 65');

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
