DROP TABLE IF EXISTS `u216319883_nodesens_db`.`node_gateways`;
CREATE TABLE `u216319883_nodesens_db`.`node_gateways`
(
  `index` TINYINT UNSIGNED AUTO_INCREMENT NOT NULL,
  `gtwy_id` CHAR(4) NOT NULL,
  `on_serv` BOOLEAN NOT NULL,
  PRIMARY KEY (`index`),
  UNIQUE `UNIQUE_ID` (`gtwy_id`)
) ENGINE = InnoDB;

INSERT INTO `u216319883_nodesens_db`.`node_gateways` (`gtwy_id`, `on_serv`) VALUES ('AAA1', true);