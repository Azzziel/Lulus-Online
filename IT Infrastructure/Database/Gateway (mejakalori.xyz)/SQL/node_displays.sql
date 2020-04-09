DROP TABLE IF EXISTS `u216319883_nodesens_db`.`node_displays`;
CREATE TABLE `u216319883_nodesens_db`.`node_displays`
(
  `index` TINYINT UNSIGNED AUTO_INCREMENT NOT NULL,
  `disp_id` CHAR(4) NOT NULL,
  `recv_rt` CHAR(4) NOT NULL,
  `on_serv` BOOLEAN NOT NULL,
  PRIMARY KEY (`index`),
  FOREIGN KEY (`recv_rt`) REFERENCES `node_repeaters` (`rptr_id`),
  UNIQUE `UNIQUE_ID` (`disp_id`)
) ENGINE = InnoDB;

INSERT INTO `u216319883_nodesens_db`.`node_displays`
(
  `disp_id`,
  `recv_rt`,
  `on_serv`
)
VALUES
('AB01', 'AA02', true);