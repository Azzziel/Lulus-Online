DROP TABLE IF EXISTS `onspotmy_node_db`.`node_floors`;
CREATE TABLE `onspotmy_node_db`.`node_floors` (
  `fl_id` INT UNSIGNED AUTO_INCREMENT NOT NULL,
  `lc_id` INT UNSIGNED NOT NULL,
  `fl_name` VARCHAR(32) NOT NULL,
  `fl_spc` INT UNSIGNED NOT NULL,
  PRIMARY KEY (`lc_id`, `fl_name`),
  FOREIGN KEY (`lc_id`) REFERENCES `node_locations` (`lc_id`),
  UNIQUE `UNIQUE_FLOOR_ID` (`fl_id`)
) ENGINE = InnoDB;
INSERT INTO `onspotmy_node_db`.`node_floors` (`fl_id`, `lc_id`, `fl_name`, `fl_spc`)
VALUES
  (1, 1, 'L1', 16),
  (2, 1, 'L2', 16);