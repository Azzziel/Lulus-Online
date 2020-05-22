DROP TABLE IF EXISTS `onspotmy_node_db`.`node_summaries`;
CREATE TABLE `onspotmy_node_db`.`node_summaries` (
  `index` INT UNSIGNED AUTO_INCREMENT NOT NULL,
  `lc_id` INT UNSIGNED NOT NULL,
  `node_id` CHAR(4) NOT NULL,
  `s_spent` INT UNSIGNED NOT NULL,
  `t_in` DATETIME NOT NULL,
  `t_out` DATETIME NOT NULL,
  PRIMARY KEY (`index`),
  FOREIGN KEY (`lc_id`, `node_id`) REFERENCES `node_devices` (`lc_id`, `node_id`)
) ENGINE = InnoDB;