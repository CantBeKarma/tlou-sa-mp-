-- phpMyAdmin SQL Dump
-- version 4.8.5
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1:3306
-- Generation Time: Sep 22, 2019 at 07:27 PM
-- Server version: 5.7.26
-- PHP Version: 7.2.18

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `tlou_prd`
--

-- --------------------------------------------------------

--
-- Table structure for table `chars_data`
--

DROP TABLE IF EXISTS `chars_data`;
CREATE TABLE IF NOT EXISTS `chars_data` (
  `guid` smallint(5) UNSIGNED DEFAULT NULL,
  `uid` smallint(5) UNSIGNED NOT NULL AUTO_INCREMENT,
  `name` varchar(24) NOT NULL,
  `health` float UNSIGNED NOT NULL DEFAULT '100',
  `skinid` smallint(4) UNSIGNED DEFAULT NULL,
  `last_x` float DEFAULT NULL,
  `last_y` float DEFAULT NULL,
  `last_z` float DEFAULT NULL,
  `last_ang` float DEFAULT NULL,
  `last_int` tinyint(3) UNSIGNED DEFAULT NULL,
  `last_vw` smallint(5) UNSIGNED DEFAULT NULL,
  `supplements` tinyint(3) UNSIGNED DEFAULT '0',
  `skill_maxhealth` tinyint(3) UNSIGNED DEFAULT '0',
  `skill_listenmode` tinyint(3) UNSIGNED DEFAULT '0',
  `skill_crafting` tinyint(3) UNSIGNED DEFAULT '0',
  `skill_healing` tinyint(3) UNSIGNED DEFAULT '0',
  `skill_weaponsway` tinyint(3) UNSIGNED DEFAULT '0',
  `skill_shivmaster` tinyint(3) UNSIGNED DEFAULT '0',
  PRIMARY KEY (`uid`),
  UNIQUE KEY `guid` (`guid`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `chars_data`
--

INSERT INTO `chars_data` (`guid`, `uid`, `name`, `health`, `skinid`, `last_x`, `last_y`, `last_z`, `last_ang`, `last_int`, `last_vw`, `supplements`, `skill_maxhealth`, `skill_listenmode`, `skill_crafting`, `skill_healing`, `skill_weaponsway`, `skill_shivmaster`) VALUES
(1, 1, 'Riddick', 15, 312, 2490.54, -1660.52, 13.3359, 86.557, 0, 0, 5, 2, 0, 1, 1, 0, 1);

-- --------------------------------------------------------

--
-- Table structure for table `collectibles_data`
--

DROP TABLE IF EXISTS `collectibles_data`;
CREATE TABLE IF NOT EXISTS `collectibles_data` (
  `uid` mediumint(8) UNSIGNED NOT NULL AUTO_INCREMENT,
  `id_item` mediumint(8) UNSIGNED DEFAULT NULL,
  `id_type` tinyint(3) UNSIGNED NOT NULL,
  `x` float DEFAULT NULL,
  `y` float DEFAULT NULL,
  `z` float DEFAULT NULL,
  `virtualWorld` smallint(5) UNSIGNED DEFAULT NULL,
  `interiorId` tinyint(3) UNSIGNED DEFAULT NULL,
  `title` varchar(20) NOT NULL,
  `message` varchar(512) NOT NULL,
  `uid_owner` smallint(6) NOT NULL DEFAULT '-1',
  `is_created` tinyint(1) UNSIGNED NOT NULL DEFAULT '1',
  PRIMARY KEY (`uid`),
  UNIQUE KEY `uid_3` (`uid`),
  KEY `uid` (`uid`),
  KEY `uid_2` (`uid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `items_data`
--

DROP TABLE IF EXISTS `items_data`;
CREATE TABLE IF NOT EXISTS `items_data` (
  `uid` mediumint(8) UNSIGNED NOT NULL AUTO_INCREMENT,
  `id_item` mediumint(8) UNSIGNED NOT NULL,
  `x` float DEFAULT NULL,
  `y` float DEFAULT NULL,
  `z` float DEFAULT NULL,
  `virtualWorld` smallint(5) UNSIGNED DEFAULT NULL,
  `interiorId` tinyint(3) UNSIGNED DEFAULT NULL,
  `amount` smallint(5) UNSIGNED NOT NULL DEFAULT '0',
  `meleeExtCnd` smallint(6) UNSIGNED DEFAULT NULL,
  `uid_owner` smallint(6) NOT NULL DEFAULT '-1',
  `is_created` tinyint(1) UNSIGNED NOT NULL DEFAULT '1',
  PRIMARY KEY (`uid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `player_data`
--

DROP TABLE IF EXISTS `player_data`;
CREATE TABLE IF NOT EXISTS `player_data` (
  `guid` smallint(5) UNSIGNED NOT NULL AUTO_INCREMENT,
  `name` varchar(24) NOT NULL,
  `ip_address` varchar(24) DEFAULT NULL,
  `register_date` datetime DEFAULT NULL,
  `last_login_date` datetime DEFAULT NULL,
  `password` varchar(129) NOT NULL,
  `admin_level` tinyint(3) UNSIGNED DEFAULT '0',
  `volume_level` tinyint(3) UNSIGNED DEFAULT '100',
  `ambient_toggle` tinyint(3) UNSIGNED DEFAULT NULL,
  `status` tinyint(3) UNSIGNED DEFAULT NULL,
  PRIMARY KEY (`guid`),
  KEY `volume_level` (`volume_level`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `player_data`
--

INSERT INTO `player_data` (`guid`, `name`, `ip_address`, `register_date`, `last_login_date`, `password`, `admin_level`, `volume_level`, `ambient_toggle`, `status`) VALUES
(1, 'Riddick', '127.0.0.1', '2019-09-19 20:14:58', '2019-09-22 21:17:58', '$2y$12$RVTYSBeuUFLJXR/qcTTqLek2BNq.ykaYQrKBjoYGxrEkCFHl5gaze', 0, 100, NULL, NULL);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
