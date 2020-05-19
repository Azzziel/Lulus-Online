DROP TABLE IF EXISTS `onspotmy_node_new_db`.`node_hosts`;
CREATE TABLE `onspotmy_node_new_db`.`node_hosts` (
  `host_id` INT UNSIGNED AUTO_INCREMENT NOT NULL,
  `hostname` VARCHAR(32) NOT NULL,
  `password` VARCHAR(32) NOT NULL,
  `firstname` VARCHAR(32) NOT NULL,
  `lastname` VARCHAR(32) NOT NULL,
  `nametitle` ENUM('Mr.', 'Mrs.', 'Ms.') NOT NULL,
  `location` INT UNSIGNED NOT NULL,
  PRIMARY KEY (`host_id`),
  FOREIGN KEY (`location`) REFERENCES `node_locations` (`lc_id`),
  UNIQUE `UNIQUE_HOST` (`hostname`)
) ENGINE = InnoDB;
INSERT INTO `onspotmy_node_new_db`.`node_hosts` (
    `hostname`,
    `password`,
    `firstname`,
    `lastname`,
    `nametitle`,
    `location`
  )
VALUES
  (
    'Azzziel',
    'lulustajir',
    'Azriel',
    'Hutagalung',
    'Mr.',
    1
  ),
  (
    'ivan0kurnia',
    'lanjuts4',
    'Ivan',
    'Kurnia',
    'Mr.',
    1
  ),
  (
    'sangpenguji',
    'selamatandalulus',
    'Penguji',
    'Yth.',
    'Mr.',
    1
  );