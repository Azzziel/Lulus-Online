DROP TABLE IF EXISTS `u216319883_nodesens_db`.`node_repeaters`;
CREATE TABLE `u216319883_nodesens_db`.`node_repeaters` (
  `index` TINYINT UNSIGNED AUTO_INCREMENT NOT NULL,
  `rptr_id` CHAR(4) NOT NULL,
  -- -------------------------------------------------------
  /* 
     * The `send_rt` column can refer to either a repeater
     * or a gateway. Thus, it is impossible to do two foreign
     * key references to serve the true purpose of the column.
     * ALternatively, just be careful on inserting them or
     * find a new-revolutionary way to refer them both.
     */
  `send_rt` CHAR(4) NOT NULL,
  -- -------------------------------------------------------
  PRIMARY KEY (`index`),
  UNIQUE `UNIQUE_ID` (`rptr_id`)
) ENGINE = InnoDB;
INSERT INTO `u216319883_nodesens_db`.`node_repeaters` (`rptr_id`, `send_rt`)
VALUES
  ('AA01', 'AAA1'),
  ('AA02', 'AA01');