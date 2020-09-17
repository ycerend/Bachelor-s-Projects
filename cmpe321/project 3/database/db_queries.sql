-- phpMyAdmin SQL Dump
-- version 4.8.3
-- https://www.phpmyadmin.net/
--
-- Host: localhost
-- Generation Time: May 15, 2019 at 02:44 AM
-- Server version: 10.1.36-MariaDB
-- PHP Version: 5.6.38

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `cmpe321_project3`
--

DELIMITER $$
--
-- Procedures
--
CREATE DEFINER=`root`@`localhost` PROCEDURE `getCoAuthorsOfAnAuthor` (IN `name` VARCHAR(30), IN `surname` VARCHAR(30))  BEGIN
SELECT author_table.* FROM author_table, author_paper_table WHERE author_id = a_id AND NOT author_name = name AND NOT author_surname = surname AND p_id IN(
SELECT p_id FROM author_paper_table
WHERE a_id IN (SELECT author_id FROM author_table WHERE author_name = name AND author_surname = surname));
    END$$

DELIMITER ;

-- --------------------------------------------------------

--
-- Table structure for table `author_paper_table`
--

CREATE TABLE `author_paper_table` (
  `ap_id` int(11) NOT NULL,
  `p_id` int(11) NOT NULL,
  `a_id` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `author_paper_table`
--

-- --------------------------------------------------------

--
-- Table structure for table `author_table`
--

CREATE TABLE `author_table` (
  `author_id` int(11) NOT NULL,
  `author_name` varchar(30) NOT NULL,
  `author_surname` varchar(30) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `author_table`
--

-- --------------------------------------------------------

--
-- Table structure for table `paper_table`
--

CREATE TABLE `paper_table` (
  `paper_id` int(11) NOT NULL,
  `paper_title` varchar(100) NOT NULL,
  `paper_abstract` varchar(10000) NOT NULL,
  `paper_result` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `paper_table`
--


--
-- Triggers `paper_table`
--
DELIMITER $$
CREATE TRIGGER `delete_paper_update_Sota` AFTER DELETE ON `paper_table` FOR EACH ROW BEGIN
UPDATE topic_table SET topic_sota=(
SELECT MAX(paper_result)
    FROM topic_paper_table,paper_table
    WHERE paper_table.paper_id = topic_paper_table.p_id and topic_id = topic_paper_table.t_id ) ;
END
$$
DELIMITER ;
DELIMITER $$
CREATE TRIGGER `updateSotaValueOnUpdate` AFTER UPDATE ON `paper_table` FOR EACH ROW BEGIN
UPDATE topic_table SET topic_sota=(
SELECT MAX(paper_result)
    FROM paper_table,topic_paper_table
    WHERE paper_table.paper_id = topic_paper_table.p_id and topic_id = topic_paper_table.t_id ) ;
END
$$
DELIMITER ;

-- --------------------------------------------------------

--
-- Table structure for table `topic_paper_table`
--

CREATE TABLE `topic_paper_table` (
  `tp_id` int(11) NOT NULL,
  `p_id` int(11) NOT NULL,
  `t_id` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `topic_paper_table`
--

--
-- Triggers `topic_paper_table`
--
DELIMITER $$
CREATE TRIGGER `UpdateSotaValue` AFTER INSERT ON `topic_paper_table` FOR EACH ROW BEGIN
UPDATE topic_table SET topic_sota=(
SELECT MAX(paper_result)
    FROM topic_paper_table,paper_table
    WHERE paper_table.paper_id = topic_paper_table.p_id and topic_id = topic_paper_table.t_id ) 
    WHERE topic_id = NEW.t_id;
END
$$
DELIMITER ;
DELIMITER $$
CREATE TRIGGER `delete_update_sota` AFTER DELETE ON `topic_paper_table` FOR EACH ROW BEGIN
UPDATE topic_table SET topic_sota=(
SELECT MAX(paper_result)
    FROM topic_paper_table,paper_table
    WHERE paper_table.paper_id = topic_paper_table.p_id and topic_id = topic_paper_table.t_id ) ;
END
$$
DELIMITER ;

-- --------------------------------------------------------

--
-- Table structure for table `topic_table`
--

CREATE TABLE `topic_table` (
  `topic_id` int(11) NOT NULL,
  `topic_name` varchar(30) NOT NULL,
  `topic_sota` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `topic_table`
--

--
-- Indexes for dumped tables
--

--
-- Indexes for table `author_paper_table`
--
ALTER TABLE `author_paper_table`
  ADD PRIMARY KEY (`ap_id`),
  ADD KEY `p_id` (`p_id`),
  ADD KEY `a_id` (`a_id`);

--
-- Indexes for table `author_table`
--
ALTER TABLE `author_table`
  ADD PRIMARY KEY (`author_id`),
  ADD UNIQUE KEY `unique_author` (`author_name`,`author_surname`);

--
-- Indexes for table `paper_table`
--
ALTER TABLE `paper_table`
  ADD PRIMARY KEY (`paper_id`);
ALTER TABLE `paper_table`
  ADD UNIQUE KEY (`paper_title`);
--
-- Indexes for table `topic_paper_table`
--
ALTER TABLE `topic_paper_table`
  ADD PRIMARY KEY (`tp_id`),
  ADD KEY `p_id` (`p_id`),
  ADD KEY `a_id` (`t_id`);

--
-- Indexes for table `topic_table`
--
ALTER TABLE `topic_table`
  ADD PRIMARY KEY (`topic_id`),
  ADD UNIQUE KEY `topic_name` (`topic_name`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `author_paper_table`
--
ALTER TABLE `author_paper_table`
  MODIFY `ap_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=41;

--
-- AUTO_INCREMENT for table `author_table`
--
ALTER TABLE `author_table`
  MODIFY `author_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=11;

--
-- AUTO_INCREMENT for table `paper_table`
--
ALTER TABLE `paper_table`
  MODIFY `paper_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=37;

--
-- AUTO_INCREMENT for table `topic_paper_table`
--
ALTER TABLE `topic_paper_table`
  MODIFY `tp_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=45;

--
-- AUTO_INCREMENT for table `topic_table`
--
ALTER TABLE `topic_table`
  MODIFY `topic_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=20;

--
-- Constraints for dumped tables
--

--
-- Constraints for table `author_paper_table`
--
ALTER TABLE `author_paper_table`
  ADD CONSTRAINT `author_paper_table_ibfk_1` FOREIGN KEY (`p_id`) REFERENCES `paper_table` (`paper_id`) ON DELETE CASCADE,
  ADD CONSTRAINT `author_paper_table_ibfk_2` FOREIGN KEY (`a_id`) REFERENCES `author_table` (`author_id`) ON DELETE CASCADE;

--
-- Constraints for table `topic_paper_table`
--
ALTER TABLE `topic_paper_table`
  ADD CONSTRAINT `topic_paper_table_ibfk_1` FOREIGN KEY (`p_id`) REFERENCES `paper_table` (`paper_id`) ON DELETE CASCADE,
  ADD CONSTRAINT `topic_paper_table_ibfk_2` FOREIGN KEY (`t_id`) REFERENCES `topic_table` (`topic_id`) ON DELETE CASCADE;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
