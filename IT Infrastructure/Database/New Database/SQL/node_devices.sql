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
  (1, 'A001', 'AA01', null, 1, '1V'),
  (1, 'A002', 'AA01', null, 1, '2V'),
  (1, 'A003', 'AA01', null, 1, '3V'),
  (1, 'A004', 'AA01', null, 1, '4V'),
  (1, 'A005', 'AA01', null, 1, '5V'),
  (1, 'A006', 'AA01', null, 1, '6V'),
  (1, 'A007', 'AA01', null, 1, '7V'),
  (1, 'A008', 'AA01', null, 1, '8V'),
  (1, 'A009', 'AA01', null, 1, '9V'),
  (1, 'A010', 'AA01', null, 1, '10V'),
  (1, 'A011', 'AA01', null, 1, '11V'),
  (1, 'A012', 'AA01', null, 1, '12V'),
  (1, 'A013', 'AA01', null, 1, '13V'),
  (1, 'A014', 'AA01', null, 1, '14V'),
  (1, 'A015', 'AA01', null, 1, '15V'),
  (1, 'A016', 'AA01', null, 1, '16V'),
  (1, 'A017', 'AA01', null, 1, '17V'),
  (1, 'A018', 'AA01', null, 1, '18V'),
  (1, 'A019', 'AA01', null, 1, '19V'),
  (1, 'A020', 'AA01', null, 1, '20V'),
  (1, 'A021', 'AA02', null, 2, '1G'),
  (1, 'A022', 'AA02', null, 2, '2G'),
  (1, 'A023', 'AA02', null, 2, '3G'),
  (1, 'A024', 'AA02', null, 2, '4G'),
  (1, 'A025', 'AA02', null, 2, '5G'),
  (1, 'A026', 'AA02', null, 2, '6G'),
  (1, 'A027', 'AA02', null, 2, '7G'),
  (1, 'A028', 'AA02', null, 2, '8G'),
  (1, 'A029', 'AA02', null, 2, '9G'),
  (1, 'A030', 'AA02', null, 2, '10G'),
  (1, 'A031', 'AA02', null, 2, '11G'),
  (1, 'A032', 'AA02', null, 2, '12G'),
  (1, 'A033', 'AA02', null, 2, '13G'),
  (1, 'A034', 'AA02', null, 2, '14G'),
  (1, 'A035', 'AA02', null, 2, '15G'),
  (1, 'A036', 'AA02', null, 2, '16G'),
  (1, 'A037', 'AA02', null, 2, '17G'),
  (1, 'A038', 'AA02', null, 2, '18G'),
  (1, 'A039', 'AA02', null, 2, '19G'),
  (1, 'A040', 'AA02', null, 2, '20G'),
  (1, 'A041', 'AA03', null, 2, '21G'),
  (1, 'A042', 'AA03', null, 2, '22G'),
  (1, 'A043', 'AA03', null, 2, '23G'),
  (1, 'A044', 'AA03', null, 2, '24G'),
  (1, 'A045', 'AA03', null, 2, '25G'),
  (1, 'A046', 'AA03', null, 2, '26G'),
  (1, 'A047', 'AA03', null, 2, '27G'),
  (1, 'A048', 'AA03', null, 2, '28G'),
  (1, 'A049', 'AA03', null, 2, '29G'),
  (1, 'A050', 'AA03', null, 2, '30G'),
  (1, 'A051', 'AA03', null, 2, '31G'),
  (1, 'A052', 'AA03', null, 2, '32G'),
  (1, 'A053', 'AA03', null, 2, '33G'),
  (1, 'A054', 'AA03', null, 2, '34G'),
  (1, 'A055', 'AA03', null, 2, '35G'),
  (1, 'A056', 'AA03', null, 2, '36G'),
  (1, 'A057', 'AA03', null, 2, '37G'),
  (1, 'A058', 'AA03', null, 2, '38G'),
  (1, 'A059', 'AA03', null, 2, '39G'),
  (1, 'A060', 'AA03', null, 2, '40G');