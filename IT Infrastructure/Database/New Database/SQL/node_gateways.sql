DROP TABLE IF EXISTS `onspotmy_node_db`.`node_gateways`;
CREATE TABLE `onspotmy_node_db`.`node_gateways` (
  `index` INT UNSIGNED AUTO_INCREMENT NOT NULL,
  `lc_id` INT UNSIGNED NOT NULL,
  `gtwy_id` CHAR(4) NOT NULL,
  `fl_id` INT UNSIGNED NOT NULL,
  PRIMARY KEY (`lc_id`, `gtwy_id`),
  FOREIGN KEY (`lc_id`, `gtwy_id`) REFERENCES `node_nodes` (`lc_id`, `node_id`),
  FOREIGN KEY (`fl_id`) REFERENCES `node_floors` (`fl_id`),
  UNIQUE `UNIQUE_INDEX` (`index`)
) ENGINE = InnoDB;
INSERT INTO `onspotmy_node_db`.`node_gateways` (`lc_id`, `gtwy_id`, `fl_id`)
VALUES
  (1, 'AAA1', 1);