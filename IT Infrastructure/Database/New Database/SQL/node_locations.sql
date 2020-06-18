DROP TABLE IF EXISTS `onspotmy_node_db`.`node_locations`;
CREATE TABLE `onspotmy_node_db`.`node_locations` (
  `lc_id` INT UNSIGNED AUTO_INCREMENT NOT NULL,
  `lc_name` VARCHAR(255) NOT NULL,
  `lc_lat` DECIMAL(9, 6) NOT NULL,
  `lc_long` DECIMAL(9, 6) NOT NULL,
  `google_place_id` VARCHAR(512) NULL,
  `note` VARCHAR(512) NULL,
  PRIMARY KEY (`lc_id`)
) ENGINE = InnoDB;
INSERT INTO `node_locations` (
    `lc_name`,
    `lc_lat`,
    `lc_long`,
    `google_place_id`,
    `note`
  )
VALUES (
    'BINUS Syahdan',
    -6.200200,
    106.785400,
    'ChIJZyud-ML2aS4RfY4NIiv0jOE',
    NULL
  ),
  (
    'Monumen Nasional',
    -6.175400,
    106.827200,
    'ChIJLbFk59L1aS4RyLzp4OHWKj0',
    'dummy'
  ),
  (
    'Baywalk Pluit',
    -6.107800,
    106.779100,
    'ChIJp0WVVaQdai4ReTYEzGUVj7o',
    'dummy'
  ),
  (
    'Mal Kelapa Gading',
    -6.157300,
    106.908400,
    'ChIJScamvEv1aS4RRxz_II4nGhw',
    'dummy'
  ),
  (
    'Taman Mini Indonesia Indah',
    -6.302400,
    106.895200,
    'ChIJYaJ20FDtaS4RDNry7LknMQc',
    'dummy'
  );