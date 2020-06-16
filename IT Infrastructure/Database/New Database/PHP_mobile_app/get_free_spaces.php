<?php

include 'config.php';

// $_GET
// $_POST

if (($auth_key != $_GET['auth_key']) && ($auth_key != $_POST['auth_key'])) {
    exit();
}

// Minified SQL
$query = "SELECT `lc_id`,(SELECT COUNT(`node_id`) FROM `node_devices` WHERE `lc_id` = `node_locations`.`lc_id`) - (SELECT COUNT(`l`.`node_id`) FROM `node_statuses` AS `l` LEFT JOIN `node_statuses` AS `r` ON ( `l`.`lc_id` = `r`.`lc_id` AND `l`.`node_id` = `r`.`node_id` AND `l`.`t_stamp` < `r`.`t_stamp`) WHERE `r`.`t_stamp` IS NULL AND `l`.`n_stats` = true AND `l`.`lc_id` = `node_locations`.`lc_id`) AS `free_space` FROM `node_locations` ORDER BY `lc_name`";
$result = $mysqli->query($query);

echo json_encode($result->fetch_all(MYSQLI_ASSOC));
