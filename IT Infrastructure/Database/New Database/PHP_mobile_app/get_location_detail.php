<?php

include 'config.php';

// $_GET
// $_POST

if (($auth_key != $_GET['auth_key']) && ($auth_key != $_POST['auth_key'])) {
    exit();
}

$location_id = $_POST['location_id'];

if (empty($location_id)) {
    exit();
}

$table = array();

$query = "
SELECT COUNT(`l`.`node_id`) AS `occupied_space` 
FROM   `node_statuses` AS `l` 
       LEFT JOIN `node_statuses` AS `r` 
              ON ( `l`.`lc_id` = `r`.`lc_id` 
                   AND `l`.`node_id` = `r`.`node_id` 
                   AND `l`.`t_stamp` < `r`.`t_stamp` ) 
WHERE  `r`.`t_stamp` IS NULL 
       AND `l`.`n_stats` = true 
       AND `l`.`lc_id` = $location_id";
$result = $mysqli->query($query);
$table['occupied_space'] = $result->fetch_row()[0];

$query = "
SELECT COUNT(`node_id`) AS `total_space` 
FROM   `node_devices` 
WHERE  `lc_id` = $location_id";
$result = $mysqli->query($query);
$table['total_space'] = $result->fetch_row()[0];

echo json_encode($table);