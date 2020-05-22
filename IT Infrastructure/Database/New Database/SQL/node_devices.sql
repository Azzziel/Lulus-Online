DROP TABLE IF EXISTS `onspotmy_node_db`.`node_devices`;
CREATE TABLE `onspotmy_node_db`.`node_devices` (
  `index` INT UNSIGNED AUTO_INCREMENT NOT NULL,
  `lc_id` INT UNSIGNED NOT NULL,
  `node_id` CHAR(4) NOT NULL,
  `send_rt` CHAR(4) NOT NULL,
  `disp_rt` CHAR(4) NULL,
  `fl_id` INT UNSIGNED NOT NULL,
  `spc_nm` CHAR(3) NOT NULL,
  PRIMARY KEY (`lc_id`, `node_id`),
  FOREIGN KEY (`lc_id`, `node_id`) REFERENCES `node_nodes` (`lc_id`, `node_id`),
  FOREIGN KEY (`lc_id`, `send_rt`) REFERENCES `node_nodes` (`lc_id`, `node_id`),
  FOREIGN KEY (`lc_id`, `disp_rt`) REFERENCES `node_nodes` (`lc_id`, `node_id`),
  FOREIGN KEY (`fl_id`) REFERENCES `node_floors` (`fl_id`),
  UNIQUE `UNIQUE_INDEX` (`index`),
  UNIQUE `UNIQUE_SPACE_NAME` (`fl_id`, `spc_nm`)
) ENGINE = InnoDB;
INSERT INTO `onspotmy_node_db`.`node_devices` (
    `lc_id`,
    `node_id`,
    `send_rt`,
    `disp_rt`,
    `fl_id`,
    `spc_nm`
  )
VALUES
  (1, 'A001', 'AA01', null, 1, '1A'),
  (1, 'A002', 'AA01', null, 1, '2A'),
  (1, 'A003', 'AA01', null, 1, '3A'),
  (1, 'A004', 'AA01', null, 1, '4A'),
  (1, 'A005', 'AA01', null, 1, '5A'),
  (1, 'A006', 'AA01', null, 1, '6A'),
  (1, 'A007', 'AA01', null, 1, '7A'),
  (1, 'A008', 'AA01', null, 1, '8A'),
  (1, 'A009', 'AA01', null, 1, '9B'),
  (1, 'A010', 'AA01', null, 1, '10B'),
  (1, 'A011', 'AA01', null, 1, '11B'),
  (1, 'A012', 'AA01', null, 1, '12B'),
  (1, 'A013', 'AA01', null, 1, '13B'),
  (1, 'A014', 'AA01', null, 1, '14B'),
  (1, 'A015', 'AA01', null, 1, '15B'),
  (1, 'A016', 'AA02', null, 2, '16B'),
  (1, 'A017', 'AA02', null, 2, '1C'),
  (1, 'A018', 'AA02', null, 2, '2C'),
  (1, 'A019', 'AA02', null, 2, '3C'),
  (1, 'A020', 'AA02', null, 2, '4C'),
  (1, 'A021', 'AA02', null, 2, '5C'),
  (1, 'A022', 'AA02', null, 2, '6C'),
  (1, 'A023', 'AA02', null, 2, '7C'),
  (1, 'A024', 'AA02', null, 2, '8C'),
  (1, 'A025', 'AA02', null, 2, '9D'),
  (1, 'A026', 'AA02', null, 2, '10D'),
  (1, 'A027', 'AA02', null, 2, '11D'),
  (1, 'A028', 'AA02', null, 2, '12D'),
  (1, 'A029', 'AA02', null, 2, '13D'),
  (1, 'A030', 'AA02', null, 2, '14D'),
  (1, 'A031', 'AA02', null, 2, '15D'),
  (1, 'A032', 'AA02', null, 2, '16D');