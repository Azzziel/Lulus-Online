DROP TABLE IF EXISTS `onspotmy_node_db`.`node_displays`;
CREATE TABLE `onspotmy_node_db`.`node_displays` (
  `index` TINYINT UNSIGNED AUTO_INCREMENT NOT NULL,
  `disp_id` CHAR(4) NOT NULL,
  `recv_rt` CHAR(4) NOT NULL,
  `is_main` BOOLEAN NOT NULL,
  PRIMARY KEY (`index`),
  FOREIGN KEY (`recv_rt`) REFERENCES `node_repeaters` (`rptr_id`),
  UNIQUE `UNIQUE_ID` (`disp_id`)
) ENGINE = InnoDB;
INSERT INTO `onspotmy_node_db`.`node_displays` (`disp_id`, `recv_rt`, `is_main`)
VALUES
  ('AB01', 'AA02', true);