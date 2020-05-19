DROP TABLE IF EXISTS `onspotmy_node_new_db`.`node_statuses`;
CREATE TABLE `onspotmy_node_new_db`.`node_statuses` (
  `index` INT UNSIGNED AUTO_INCREMENT NOT NULL,
  `lc_id` INT UNSIGNED NOT NULL,
  `node_id` CHAR(4) NOT NULL,
  `n_stats` BOOLEAN NOT NULL,
  `t_stamp` DATETIME DEFAULT NOW() NOT NULL,
  PRIMARY KEY (`lc_id`, `node_id`),
  FOREIGN KEY (`lc_id`, `node_id`) REFERENCES `node_devices` (`lc_id`, `node_id`),
  UNIQUE `UNIQUE_INDEX` (`index`)
) ENGINE = InnoDB;
INSERT INTO `onspotmy_node_new_db`.`node_statuses` (`lc_id`, `node_id`, `n_stats`, `t_stamp`)
VALUES
  (1, 'A001', false, '2020-01-01 07:00:00'),
  (1, 'A002', false, '2020-01-01 07:00:00'),
  (1, 'A003', false, '2020-01-01 07:00:00'),
  (1, 'A004', false, '2020-01-01 07:00:00'),
  (1, 'A005', false, '2020-01-01 07:00:00'),
  (1, 'A006', false, '2020-01-01 07:00:00'),
  (1, 'A007', false, '2020-01-01 07:00:00'),
  (1, 'A008', false, '2020-01-01 07:00:00');