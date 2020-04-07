DROP TABLE IF EXISTS `u216319883_nodesens_db`.`node_displays`;
CREATE TABLE `u216319883_nodesens_db`.`node_displays`
(
  `index` SMALLINT UNSIGNED AUTO_INCREMENT NOT NULL,
  `disp_id` CHAR(4) NOT NULL,
  `recv_rt` CHAR(4) NOT NULL,
  PRIMARY KEY (`index`),
  UNIQUE `UNIQUE_ID` (`disp_id`)
) ENGINE = InnoDB;

INSERT INTO `u216319883_nodesens_db`.`node_displays`
(
  `disp_id`,
  `recv_rt`
)
VALUES
('AB01', 'AA02');