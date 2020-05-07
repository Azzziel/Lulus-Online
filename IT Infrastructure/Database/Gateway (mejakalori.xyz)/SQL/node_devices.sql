DROP TABLE IF EXISTS `onspotmy_node_db`.`node_devices`;
CREATE TABLE `onspotmy_node_db`.`node_devices` (
  `index` SMALLINT UNSIGNED AUTO_INCREMENT NOT NULL,
  `node_id` CHAR(4) NOT NULL,
  `rptr_rt` CHAR(4) NOT NULL,
  `disp_rt` CHAR(4) NULL,
  PRIMARY KEY (`index`),
  FOREIGN KEY (`rptr_rt`) REFERENCES `node_repeaters` (`rptr_id`),
  FOREIGN KEY (`disp_rt`) REFERENCES `node_displays` (`disp_id`),
  UNIQUE `UNIQUE_ID` (`node_id`)
) ENGINE = InnoDB;
INSERT INTO `onspotmy_node_db`.`node_devices` (`node_id`, `rptr_rt`, `disp_rt`)
VALUES
  ('A001', 'AA01', NULL),
  ('A002', 'AA01', NULL),
  ('A003', 'AA02', NULL),
  ('A004', 'AA02', NULL);