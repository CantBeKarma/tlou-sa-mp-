-- phpMyAdmin SQL Dump
-- version 4.8.5
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1:3306
-- Generation Time: Aug 15, 2019 at 10:46 AM
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
  `supplements` tinyint(3) UNSIGNED DEFAULT NULL,
  `skill_maxhealth` tinyint(3) UNSIGNED DEFAULT NULL,
  `skill_listenmode` tinyint(3) UNSIGNED DEFAULT NULL,
  `skill_crafting` tinyint(3) UNSIGNED DEFAULT NULL,
  `skill_healing` tinyint(3) UNSIGNED DEFAULT NULL,
  `skill_weaponsway` tinyint(3) UNSIGNED DEFAULT NULL,
  `skill_shivmaster` tinyint(3) UNSIGNED DEFAULT NULL,
  PRIMARY KEY (`uid`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `chars_data`
--

INSERT INTO `chars_data` (`guid`, `uid`, `name`, `health`, `skinid`, `last_x`, `last_y`, `last_z`, `last_ang`, `last_int`, `last_vw`, `supplements`, `skill_maxhealth`, `skill_listenmode`, `skill_crafting`, `skill_healing`, `skill_weaponsway`, `skill_shivmaster`) VALUES
(1, 1, 'Riddick', 80, 312, 2467.17, -1661.69, 13.2855, 304.711, 0, 0, 40, 0, 0, 3, 0, 2, 0);

-- --------------------------------------------------------

--
-- Table structure for table `items_data`
--

DROP TABLE IF EXISTS `items_data`;
CREATE TABLE IF NOT EXISTS `items_data` (
  `uid` mediumint(8) UNSIGNED NOT NULL AUTO_INCREMENT,
  `id_item` mediumint(8) DEFAULT NULL,
  `x` float DEFAULT NULL,
  `y` float DEFAULT NULL,
  `z` float DEFAULT NULL,
  `rX` float DEFAULT NULL,
  `rY` float DEFAULT NULL,
  `rZ` float DEFAULT NULL,
  `virtualWorld` smallint(5) UNSIGNED DEFAULT NULL,
  `interiorId` tinyint(3) UNSIGNED DEFAULT NULL,
  `amount` tinyint(3) UNSIGNED DEFAULT NULL,
  `meleeExtCnd` smallint(6) UNSIGNED DEFAULT NULL,
  `collectibleTitle` varchar(32) DEFAULT NULL,
  `collectibleInnerMsg` varchar(128) DEFAULT NULL,
  `uid_owner` smallint(6) DEFAULT '-1',
  `is_created` tinyint(1) NOT NULL,
  PRIMARY KEY (`uid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `player_data`
--

DROP TABLE IF EXISTS `player_data`;
CREATE TABLE IF NOT EXISTS `player_data` (
  `guid` smallint(5) UNSIGNED NOT NULL,
  `password` varchar(129) NOT NULL,
  `admin_level` tinyint(3) UNSIGNED DEFAULT NULL,
  `volume_level` tinyint(3) UNSIGNED DEFAULT NULL,
  `ambient_toggle` tinyint(3) UNSIGNED DEFAULT NULL,
  `status` tinyint(3) UNSIGNED DEFAULT NULL,
  PRIMARY KEY (`guid`),
  KEY `volume_level` (`volume_level`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `player_data`
--

INSERT INTO `player_data` (`guid`, `password`, `admin_level`, `volume_level`, `ambient_toggle`, `status`) VALUES
(1, 'C0FB3E94DABA675E3FD32E8C1C018D2A6435CA5145738AE86429CA5D3AA14069', 4, NULL, NULL, NULL);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
