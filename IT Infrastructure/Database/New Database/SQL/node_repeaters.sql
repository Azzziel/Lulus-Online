DROP TABLE IF EXISTS `onspotmy_node_new_db`.`node_repeaters`;
CREATE TABLE `onspotmy_node_new_db`.`node_repeaters` (
  `index` INT UNSIGNED AUTO_INCREMENT NOT NULL,
  `lc_id` INT UNSIGNED NOT NULL,
  `rptr_id` CHAR(4) NOT NULL,
  `send_rt` CHAR(4) NOT NULL,
  `fl_id` INT UNSIGNED NOT NULL,
  PRIMARY KEY (`lc_id`, `rptr_id`),
  FOREIGN KEY (`lc_id`, `rptr_id`) REFERENCES `node_nodes` (`lc_id`, `node_id`),
  FOREIGN KEY (`lc_id`, `send_rt`) REFERENCES `node_nodes` (`lc_id`, `node_id`),
  FOREIGN KEY (`fl_id`) REFERENCES `node_floors` (`fl_id`),
  UNIQUE `UNIQUE_INDEX` (`index`)
) ENGINE = InnoDB;
INSERT INTO `onspotmy_node_new_db`.`node_repeaters` (`lc_id`, `rptr_id`, `send_rt`, `fl_id`)
VALUES
  (1, 'AA01', 'AAA1', 1),
  (1, 'AA02', 'AA01', 2);