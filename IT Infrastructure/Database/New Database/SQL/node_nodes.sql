DROP TABLE IF EXISTS `onspotmy_node_db`.`node_nodes`;
CREATE TABLE `onspotmy_node_db`.`node_nodes` (
  `lc_id` INT UNSIGNED NOT NULL,
  `node_id` CHAR(4) NOT NULL,
  `n_type` ENUM('GTWY', 'RPTR', 'SNSR', 'DISP') NOT NULL,
  PRIMARY KEY (`lc_id`, `node_id`),
  FOREIGN KEY (`lc_id`) REFERENCES `node_locations` (`lc_id`)
) ENGINE = InnoDB;
INSERT INTO `onspotmy_node_db`.`node_nodes` (`lc_id`, `node_id`, `n_type`)
VALUES
  (1, 'AAA1', 'GTWY'),
  (1, 'AA01', 'RPTR'),
  (1, 'AA02', 'RPTR'),
  (1, 'AA03', 'RPTR'),
  (1, 'AB01', 'DISP'),
  (1, 'AB02', 'DISP'),
  (1, 'A001', 'SNSR'),
  (1, 'A002', 'SNSR'),
  (1, 'A003', 'SNSR'),
  (1, 'A004', 'SNSR'),
  (1, 'A005', 'SNSR'),
  (1, 'A006', 'SNSR'),
  (1, 'A007', 'SNSR'),
  (1, 'A008', 'SNSR'),
  (1, 'A009', 'SNSR'),
  (1, 'A010', 'SNSR'),
  (1, 'A011', 'SNSR'),
  (1, 'A012', 'SNSR'),
  (1, 'A013', 'SNSR'),
  (1, 'A014', 'SNSR'),
  (1, 'A015', 'SNSR'),
  (1, 'A016', 'SNSR'),
  (1, 'A017', 'SNSR'),
  (1, 'A018', 'SNSR'),
  (1, 'A019', 'SNSR'),
  (1, 'A020', 'SNSR'),
  (1, 'A021', 'SNSR'),
  (1, 'A022', 'SNSR'),
  (1, 'A023', 'SNSR'),
  (1, 'A024', 'SNSR'),
  (1, 'A025', 'SNSR'),
  (1, 'A026', 'SNSR'),
  (1, 'A027', 'SNSR'),
  (1, 'A028', 'SNSR'),
  (1, 'A029', 'SNSR'),
  (1, 'A030', 'SNSR'),
  (1, 'A031', 'SNSR'),
  (1, 'A032', 'SNSR'),
  (1, 'A033', 'SNSR'),
  (1, 'A034', 'SNSR'),
  (1, 'A035', 'SNSR'),
  (1, 'A036', 'SNSR'),
  (1, 'A037', 'SNSR'),
  (1, 'A038', 'SNSR'),
  (1, 'A039', 'SNSR'),
  (1, 'A040', 'SNSR'),
  (1, 'A041', 'SNSR'),
  (1, 'A042', 'SNSR'),
  (1, 'A043', 'SNSR'),
  (1, 'A044', 'SNSR'),
  (1, 'A045', 'SNSR'),
  (1, 'A046', 'SNSR'),
  (1, 'A047', 'SNSR'),
  (1, 'A048', 'SNSR'),
  (1, 'A049', 'SNSR'),
  (1, 'A050', 'SNSR'),
  (1, 'A051', 'SNSR'),
  (1, 'A052', 'SNSR'),
  (1, 'A053', 'SNSR'),
  (1, 'A054', 'SNSR'),
  (1, 'A055', 'SNSR'),
  (1, 'A056', 'SNSR'),
  (1, 'A057', 'SNSR'),
  (1, 'A058', 'SNSR'),
  (1, 'A059', 'SNSR'),
  (1, 'A060', 'SNSR');