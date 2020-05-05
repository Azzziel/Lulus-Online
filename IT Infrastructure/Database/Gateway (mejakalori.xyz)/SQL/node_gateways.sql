DROP TABLE IF EXISTS `onspotmy_node_db`.`node_gateways`;
CREATE TABLE `onspotmy_node_db`.`node_gateways` (
  `index` TINYINT UNSIGNED AUTO_INCREMENT NOT NULL,
  `gtwy_id` CHAR(4) NOT NULL,
  PRIMARY KEY (`index`),
  UNIQUE `UNIQUE_ID` (`gtwy_id`)
) ENGINE = InnoDB;
INSERT INTO `onspotmy_node_db`.`node_gateways` (`gtwy_id`)
VALUES
  ('AAA1');