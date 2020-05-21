DROP TABLE IF EXISTS `onspotmy_node_db`.`node_summaries`;
CREATE TABLE `onspotmy_node_db`.`node_summaries` (
  `index` INT UNSIGNED AUTO_INCREMENT NOT NULL,
  `lc_id` INT UNSIGNED NOT NULL,
  `node_id` CHAR(4) NOT NULL,
  `m_spent` SMALLINT NOT NULL,
  `t_in` DATETIME NOT NULL,
  `t_out` DATETIME NOT NULL,
  PRIMARY KEY (`index`),
  FOREIGN KEY (`lc_id`, `node_id`) REFERENCES `node_devices` (`lc_id`, `node_id`)
) ENGINE = InnoDB;
--
-- TRIGGER
DROP TRIGGER IF EXISTS `onspotmy_node_db`.`insert_summary`;
CREATE TRIGGER `onspotmy_node_db`.`insert_summary`
AFTER
INSERT ON `node_statuses` FOR EACH ROW
INSERT INTO `node_summaries` (
    `lc_id`,
    `node_id`,
    `m_spent`,
    `t_in`,
    `t_out`
  )
VALUES
  (null, null, null, null, null);
--
  -- SELECT: Last record in a group by (fast)
SELECT
  `l`.*
FROM `node_statuses` AS `l`
LEFT JOIN `node_statuses` AS `r` ON (
    `l`.`lc_id` = `r`.`lc_id`
    AND `l`.`node_id` = `r`.`node_id`
    AND `l`.`t_stamp` < `r`.`t_stamp`
  )
WHERE
  `r`.`t_stamp` IS NULL
ORDER BY
  `l`.`node_id`;