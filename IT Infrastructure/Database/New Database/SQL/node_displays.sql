DROP TABLE IF EXISTS `onspotmy_node_new_db`.`node_displays`;
CREATE TABLE `onspotmy_node_new_db`.`node_displays` (
  `index` INT UNSIGNED AUTO_INCREMENT NOT NULL,
  `lc_id` INT UNSIGNED NOT NULL,
  `disp_id` CHAR(4) NOT NULL,
  `recv_rt` CHAR(4) NOT NULL,
  `is_main` BOOLEAN NOT NULL,
  `fl_id` INT UNSIGNED NOT NULL,
  PRIMARY KEY (`lc_id`, `disp_id`),
  FOREIGN KEY (`lc_id`, `disp_id`) REFERENCES `node_nodes` (`lc_id`, `node_id`),
  FOREIGN KEY (`lc_id`, `recv_rt`) REFERENCES `node_nodes` (`lc_id`, `node_id`),
  FOREIGN KEY (`fl_id`) REFERENCES `node_floors` (`fl_id`),
  UNIQUE `UNIQUE_INDEX` (`index`)
) ENGINE = InnoDB;
INSERT INTO `onspotmy_node_new_db`.`node_displays` (
    `lc_id`,
    `disp_id`,
    `recv_rt`,
    `is_main`,
    `fl_id`
  )
VALUES
  (
    1,
    'AB01',
    'AA02',
    true,
    2
  );