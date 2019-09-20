-- phpMyAdmin SQL Dump
-- version 4.8.5
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1:3306
-- Generation Time: Sep 20, 2019 at 03:27 PM
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
(1, 1, 'Riddick', 15, 312, 2500, -1668.89, 13.3517, 98.989, 0, 0, 5, 2, 0, 1, 1, 0, 1);

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
  `rX` float DEFAULT NULL,
  `rY` float DEFAULT NULL,
  `rZ` float DEFAULT NULL,
  `virtualWorld` smallint(5) UNSIGNED DEFAULT NULL,
  `interiorId` tinyint(3) UNSIGNED DEFAULT NULL,
  `title` varchar(32) NOT NULL,
  `message` varchar(512) NOT NULL,
  `uid_owner` smallint(6) NOT NULL DEFAULT '-1',
  `is_created` tinyint(1) UNSIGNED NOT NULL DEFAULT '1',
  PRIMARY KEY (`uid`),
  UNIQUE KEY `uid_3` (`uid`),
  KEY `uid` (`uid`),
  KEY `uid_2` (`uid`)
) ENGINE=InnoDB AUTO_INCREMENT=39 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `collectibles_data`
--

INSERT INTO `collectibles_data` (`uid`, `id_item`, `id_type`, `x`, `y`, `z`, `rX`, `rY`, `rZ`, `virtualWorld`, `interiorId`, `title`, `message`, `uid_owner`, `is_created`) VALUES
(14, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Trading Note', 'Yesterday I met with some people who did not want to shoot me on sight. Shocking, I know. We traded some supplies and went on our merry way. They had kids with them and they seemed pretty scared. I almost told them about this place. What if they\'re like the others? What if... You know what? I don\'t care. What\'s the point of surviving if you don\'t have someone to laugh at your lame jokes? Tomorrow, I\'m going to search for them. See if they want to join me in here.~n~~n~Ish', 1, 0),
(15, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Boat Note', 'Well... It\'s looking like I\'ve dodged the chaos and the mayhem long enough. My time out at sea is coming to an end. I\'m short on supplies and this boat has seen better days. And you know what... This was bound to happen sooner or later. I guess it\'s time to go see what\'s left of mankind. What could possibly go wrong, right? If you happen to find my skeleton, please don\'t step on my skull. Thanks.~n~~n~Ish', 1, 0),
(16, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Sewers Note', 'I\'m gone for a few months and the world doesn\'t waste any time going to shit. Everywhere you turn there\'re infected — and non-infected — trying to kill you. Mankind is back to the food chain, baby! I\'m kind of shocked I\'ve survived this long. These sewers seem pretty safe. Unlimited exits/entrances make it easier to defend, and if anyone gets in here, I can lose them in the maze. Wish me luck.~n~~n~Ish', 1, 0),
(17, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Looting Note', 'Another night, another shooting. I could see the muzzel flashes coming from the Carsons\' windows. I stayed up the entire night with the whole family in one room. I doubt anybody got any sleep. It\'s only a matter of time before the looters try to break into our house. We have to get out of here. I don\'t think we can stay here anymore. He seems like a trustworthy guy. I\'m going to suggest to everyone tomorrow that we take him up on his offer. ~n~~n~Kyle', 1, 0),
(18, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Rain Catcher Note', 'Hey Susan,~n~~n~I just wanted to drop you a quick line and say these rain-catchers were a great idea. Super smart to gather water without leaving the place. I hope you don\'t mind, but I gave the kids a couple of water guns. So of course... I\'ve been drenched all day. If you don\'t want them to have it, let me know and I\'ll take them back. See you at dinner tonight. Fair warning though - Kyle is making his special \"meatloaf\" again. ~n~~n~Ish', 1, 0),
(19, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Cornered Note', 'We\'re trapped. I think everyone else is dead. Some of the little ones are with me. I got infected pounding at the door. I don\'t know how long we\'ll hold out. If Ish and the others are alive, maybe they can reach us. They have to reach us. If it comes down to it I\'ll make it quick. ~n~~n~Kyle', 1, 0),
(20, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Survivors Note', 'One open door. That\'s all it took. One of us forgot to close a door and a horde of those monsters entered our camp. We shut them in there and wrote a warning on the outside. Susan and a couple of the kids are with me. Every part of my being just wants to give up. It\'d be so easy to surrender to this world. I can\'t do that, though. I have too much faith in humanity. I\'ve seen that we\'re still capable of good. We can make it. I have to stay strong... For her. ~n~~n~Ish', 1, 0),
(32, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Note from Mom', 'My mom\'s note', 1, 0),
(33, 0, 0, 2503.19, -1686.33, 12.5409, 0, 0, 0, 0, 0, 'Do_not_enter', 'Seems_like_we_did_it_Do_not_open_the_garage', 1, 0),
(34, 1, 0, 2499.31, -1675.99, 12.3498, 0, 0, 0, 0, 0, 'MyTestTitle', 'MyTestMsg', 1, 0),
(37, 2, 1, 2487.43, -1659.7, 12.3438, 120, 0, 0, 0, 0, 'Firefly Pendant', 'Riddick~n~2019-09-16', 1, 0),
(38, 2, 1, 2475.03, -1665.76, 12.3195, 120, 0, 0, 0, 0, 'Firefly Pendant', 'Riddick~n~2019-09-16', 1, 0);

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
(1, 'Riddick', '127.0.0.1', '2019-09-19 20:14:58', '2019-09-20 17:20:47', '$2y$12$RVTYSBeuUFLJXR/qcTTqLek2BNq.ykaYQrKBjoYGxrEkCFHl5gaze', 0, 100, NULL, NULL);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
