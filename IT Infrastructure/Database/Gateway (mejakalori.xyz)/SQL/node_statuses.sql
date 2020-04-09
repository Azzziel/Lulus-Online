DROP TABLE IF EXISTS `u216319883_nodesens_db`.`node_statuses`;
CREATE TABLE `u216319883_nodesens_db`.`node_statuses`
(
  `index` INT UNSIGNED AUTO_INCREMENT NOT NULL,
  `node_id` CHAR(4) NOT NULL,
  `n_stats` BOOLEAN NOT NULL,
  `t_stamp` DATETIME DEFAULT NOW() NOT NULL,
  PRIMARY KEY (`index`),
  FOREIGN KEY (`node_id`) REFERENCES `node_devices` (`node_id`)
) ENGINE = InnoDB;

INSERT INTO `u216319883_nodesens_db`.`node_statuses`
(
  `node_id`,
  `n_stats`
)
VALUES
('A001', false),
('A001', true),
('A002', false),
('A004', false),
('A004', true),
('A004', false);