DROP TABLE IF EXISTS `u216319883_nodesens_db`.`node_devices`;
CREATE TABLE `u216319883_nodesens_db`.`node_devices` (
  `index` SMALLINT UNSIGNED AUTO_INCREMENT NOT NULL,
  `node_id` CHAR(4) NOT NULL,
  `rptr_rt` CHAR(4) NOT NULL,
  `disp_rt` CHAR(4) NULL,
  `on_serv` BOOLEAN NOT NULL,
  PRIMARY KEY (`index`),
  FOREIGN KEY (`rptr_rt`) REFERENCES `node_repeaters` (`rptr_id`),
  FOREIGN KEY (`disp_rt`) REFERENCES `node_displays` (`disp_id`),
  UNIQUE `UNIQUE_ID` (`node_id`)
) ENGINE = InnoDB;
INSERT INTO `u216319883_nodesens_db`.`node_devices` (
    `node_id`,
    `rptr_rt`,
    `disp_rt`,
    `on_serv`
  )
VALUES
  ('A001', 'AA01', 'AB01', true),
  ('A002', 'AA01', 'AB01', true),
  ('A003', 'AA02', NULL, true),
  ('A004', 'AA02', NULL, true);