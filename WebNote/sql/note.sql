-- phpMyAdmin SQL Dump
-- version 4.8.3
-- https://www.phpmyadmin.net/
--
-- 主机： 127.0.0.1:3306
-- 生成日期： 2018-12-17 11:50:58
-- 服务器版本： 5.7.23
-- PHP 版本： 7.2.10

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- 数据库： `php`
--

-- --------------------------------------------------------

--
-- 表的结构 `note`
--

DROP TABLE IF EXISTS `note`;
CREATE TABLE IF NOT EXISTS `note` (
  `id` int(5) UNSIGNED NOT NULL AUTO_INCREMENT,
  `who` int(5) NOT NULL DEFAULT '0',
  `title` varchar(255) NOT NULL,
  `content` text NOT NULL,
  `date_created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `date_modified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=23 DEFAULT CHARSET=utf8;

--
-- 转存表中的数据 `note`
--

INSERT INTO `note` (`id`, `who`, `title`, `content`, `date_created`, `date_modified`) VALUES
(1, 0, 'å…±äº«ç¬”è®°', '<p>æ‰€æœ‰ç”¨æˆ·éƒ½èƒ½çœ‹åˆ°å¹¶èƒ½ç¼–è¾‘ã€‚</p>', '2018-12-16 01:09:52', '2018-12-17 11:46:07'),
(19, 3, 'Guest NoteBook', '<p>å†…å®¹ä¸èƒ½ä¸ºç©ºå•¦ã€‚</p>', '2018-12-16 04:00:11', '2018-12-17 11:47:46'),
(22, 19, 'æˆ‘çš„ç§äººç¬”è®°', '<p>åˆ«äººæ˜¯çœ‹ä¸åˆ°çš„å•¦ã€‚</p>', '2018-12-17 04:50:36', '2018-12-17 11:44:49');
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
