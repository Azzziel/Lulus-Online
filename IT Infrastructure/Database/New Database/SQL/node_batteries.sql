DROP TABLE IF EXISTS `onspotmy_node_db`.`node_batteries`;
CREATE TABLE `onspotmy_node_db`.`node_batteries` (
  `index` INT UNSIGNED AUTO_INCREMENT NOT NULL,
  `lc_id` INT UNSIGNED NOT NULL,
  `node_id` CHAR(4) NOT NULL,
  `battery` TINYINT NOT NULL,
  `t_stamp` DATETIME DEFAULT NOW() NOT NULL,
  PRIMARY KEY (`index`),
  FOREIGN KEY (`lc_id`, `node_id`) REFERENCES `node_devices` (`lc_id`, `node_id`)
) ENGINE = InnoDB;
INSERT INTO `onspotmy_node_db`.`node_batteries` (`lc_id`, `node_id`, `battery`, `t_stamp`)
VALUES
  (1, 'A001', 4, '2020-01-01 07:00:00'),
  (1, 'A002', 4, '2020-01-01 07:00:00'),
  (1, 'A003', 4, '2020-01-01 07:00:00'),
  (1, 'A004', 4, '2020-01-01 07:00:00'),
  (1, 'A005', 4, '2020-01-01 07:00:00'),
  (1, 'A006', 4, '2020-01-01 07:00:00'),
  (1, 'A007', 4, '2020-01-01 07:00:00'),
  (1, 'A008', 4, '2020-01-01 07:00:00');