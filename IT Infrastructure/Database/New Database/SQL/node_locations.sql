DROP TABLE IF EXISTS `onspotmy_node_db`.`node_locations`;
CREATE TABLE `onspotmy_node_db`.`node_locations` (
  `lc_id` INT UNSIGNED AUTO_INCREMENT NOT NULL,
  `lc_name` VARCHAR(255) NOT NULL,
  `lc_lat` DECIMAL(9, 6) NOT NULL,
  `lc_long` DECIMAL(9, 6) NOT NULL,
  `lc_tokn` CHAR(32) NOT NULL,
  `mx_tokn` TINYINT UNSIGNED NOT NULL,
  `google_place_id` VARCHAR(512) NULL,
  `note` VARCHAR(512) NULL,
  PRIMARY KEY (`lc_id`),
  UNIQUE `UNIQUE_TOKEN` (`lc_tokn`)
) ENGINE = InnoDB;
INSERT INTO `node_locations` (
    `lc_name`,
    `lc_lat`,
    `lc_long`,
    `lc_tokn`,
    `mx_tokn`,
    `google_place_id`,
    `note`
  )
VALUES (
    'BINUS Syahdan',
    -6.200200,
    106.785400,
    '73156307c561ccc9f5926ebbdd0472e0',
    3,
    'ChIJZyud-ML2aS4RfY4NIiv0jOE',
    NULL
  ),
  (
    'Monumen Nasional',
    -6.175400,
    106.827200,
    '15bea9fdb26aa9ea1b86eeb077343138',
    1,
    'ChIJLbFk59L1aS4RyLzp4OHWKj0',
    'dummy'
  ),
  (
    'Baywalk Pluit',
    -6.107800,
    106.779100,
    '9f2a15e4d17d3ac7394da8a2759604c9',
    1,
    'ChIJp0WVVaQdai4ReTYEzGUVj7o',
    'dummy'
  ),
  (
    'Mal Kelapa Gading',
    -6.157300,
    106.908400,
    '4aff01533258c8070900cc84c6ca58f8',
    1,
    'ChIJScamvEv1aS4RRxz_II4nGhw',
    'dummy'
  ),
  (
    'Taman Mini Indonesia Indah',
    -6.302400,
    106.895200,
    '064512a09e8a2a4e074ee240a332217b',
    1,
    'ChIJYaJ20FDtaS4RDNry7LknMQc',
    'dummy'
  );