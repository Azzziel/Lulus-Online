DROP TABLE IF EXISTS `onspotmy_node_db`.`node_statuses`;
CREATE TABLE `onspotmy_node_db`.`node_statuses` (
  `index` INT UNSIGNED AUTO_INCREMENT NOT NULL,
  `node_id` CHAR(4) NOT NULL,
  `n_stats` BOOLEAN NOT NULL,
  `t_stamp` DATETIME DEFAULT NOW() NOT NULL,
  PRIMARY KEY (`index`),
  FOREIGN KEY (`node_id`) REFERENCES `node_devices` (`node_id`)
) ENGINE = InnoDB;
INSERT INTO `onspotmy_node_db`.`node_statuses` (`node_id`, `n_stats`, `t_stamp`)
VALUES
  ('A001', false, '2020-01-01 06:00:00'),
  ('A002', false, '2020-01-01 06:00:00'),
  ('A003', false, '2020-01-01 06:00:00'),
  ('A004', false, '2020-01-01 06:00:00'),
  ('A001', true, '2020-01-01 07:00:00'),
  ('A003', true, '2020-01-01 07:30:00'),
  ('A004', true, '2020-01-01 09:30:00'),
  ('A001', false, '2020-01-01 10:00:00'),
  ('A003', false, '2020-01-01 10:30:00'),
  ('A004', false, '2020-01-01 21:00:00'),
  ('A001', false, '2020-02-02 06:00:00'),
  ('A002', false, '2020-02-02 06:00:00'),
  ('A003', false, '2020-02-02 06:00:00'),
  ('A004', false, '2020-02-02 06:00:00'),
  ('A001', true, '2020-02-02 07:00:00'),
  ('A003', true, '2020-02-02 07:30:00'),
  ('A004', true, '2020-02-02 09:30:00'),
  ('A001', false, '2020-02-02 10:00:00'),
  ('A003', false, '2020-02-02 10:30:00'),
  ('A004', false, '2020-02-02 21:00:00');