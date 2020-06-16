DROP TABLE IF EXISTS `onspotmy_node_db`.`node_resets`;
CREATE TABLE `onspotmy_node_db`.`node_resets` (
  `index` INT UNSIGNED AUTO_INCREMENT NOT NULL,
  `lc_id` INT UNSIGNED NOT NULL,
  `node_id` CHAR(4) NOT NULL,
  `t_stamp` DATETIME DEFAULT NOW() NOT NULL,
  PRIMARY KEY (`index`),
  FOREIGN KEY (`lc_id`, `node_id`) REFERENCES `node_devices` (`lc_id`, `node_id`)
) ENGINE = InnoDB;
INSERT INTO `onspotmy_node_db`.`node_resets` (`lc_id`, `node_id`, `t_stamp`)
VALUES
  (1, 'A005', '2020-05-03 09:50:00'),
  (1, 'A022', '2020-05-14 11:45:00'),
  (1, 'A017', '2020-05-22 20:07:00');