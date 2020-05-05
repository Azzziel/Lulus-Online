DROP TABLE IF EXISTS `onspotmy_node_db`.`node_batteries`;
CREATE TABLE `onspotmy_node_db`.`node_batteries` (
  `index` INT UNSIGNED AUTO_INCREMENT NOT NULL,
  `node_id` CHAR(4) NOT NULL,
  `battery` TINYINT(1) NOT NULL,
  `t_stamp` DATETIME DEFAULT NOW() NOT NULL,
  PRIMARY KEY (`index`),
  FOREIGN KEY (`node_id`) REFERENCES `node_devices` (`node_id`)
) ENGINE = InnoDB;
INSERT INTO `onspotmy_node_db`.`node_batteries` (`node_id`, `battery`, `t_stamp`)
VALUES
  ('A001', 3, '2020-01-02 13:45:00'),
  ('A002', 2, '2020-01-02 13:45:00'),
  ('A003', 4, '2020-01-02 13:45:00'),
  ('A004', 2, '2020-01-02 13:45:00'),
  ('A001', 2, '2020-02-04 13:45:00'),
  ('A002', 1, '2020-02-04 13:45:00'),
  ('A003', 3, '2020-02-04 13:45:00'),
  ('A004', 1, '2020-02-04 13:45:00');