DROP TABLE IF EXISTS `u216319883_nodesens_db`.`node_batteries`;
CREATE TABLE `u216319883_nodesens_db`.`node_batteries`
(
  `index` INT UNSIGNED AUTO_INCREMENT NOT NULL,
  `node_id` CHAR(4) NOT NULL,
  `battery` TINYINT(1) NOT NULL,
  `t_stamp` DATETIME DEFAULT NOW() NOT NULL,
  PRIMARY KEY (`index`),
  FOREIGN KEY (`node_id`) REFERENCES `node_devices` (`node_id`)
) ENGINE = InnoDB;

INSERT INTO `u216319883_nodesens_db`.`node_batteries`
(
  `node_id`,
  `battery`
)
VALUES
('A001', 3),
('A002', 2),
('A003', 4),
('A004', 1);