DROP TABLE IF EXISTS `u216319883_nodesens_db`.`node_repeaters`;
CREATE TABLE `u216319883_nodesens_db`.`node_repeaters`
(
  `index` SMALLINT UNSIGNED AUTO_INCREMENT NOT NULL,
  `rptr_id` CHAR(4) NOT NULL,
  `send_rt` CHAR(4) NOT NULL,
  PRIMARY KEY (`index`),
  UNIQUE `UNIQUE_ID` (`rptr_id`)
) ENGINE = InnoDB;

INSERT INTO `u216319883_nodesens_db`.`node_repeaters`
(
  `rptr_id`,
  `send_rt`
)
VALUES
('AA01', 'A001'),
('AA02', 'AA01');