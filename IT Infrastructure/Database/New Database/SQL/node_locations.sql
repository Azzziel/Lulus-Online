DROP TABLE IF EXISTS `onspotmy_node_new_db`.`node_locations`;
CREATE TABLE `onspotmy_node_new_db`.`node_locations` (
  `lc_id` INT UNSIGNED AUTO_INCREMENT NOT NULL,
  `lc_name` VARCHAR(255) NOT NULL,
  `lc_lat` DECIMAL(9, 6) NOT NULL,
  `lc_long` DECIMAL(9, 6) NOT NULL,
  `lc_tokn` CHAR(32) NOT NULL,
  `mx_tokn` TINYINT UNSIGNED NOT NULL,
  `note` VARCHAR(512) NULL,
  PRIMARY KEY (`lc_id`),
  UNIQUE `UNIQUE_TOKEN` (`lc_tokn`)
) ENGINE = InnoDB;
INSERT INTO `onspotmy_node_new_db`.`node_locations` (
    `lc_id`,
    `lc_name`,
    `lc_lat`,
    `lc_long`,
    `lc_tokn`,
    `mx_tokn`
  )
VALUES
  (
    1,
    'Gedung Parkir Jalan Syahdan',
    -6.200918,
    106.783644,
    '73156307c561ccc9f5926ebbdd0472e0',
    3
  );