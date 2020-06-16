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

$query = "
SELECT `node_floors`.`fl_id`, 
       `fl_name`, 
       SUM(`l`.`n_stats`) AS `floor_occupancy`, 
       COUNT(*)           AS `floor_space` 
FROM   `node_floors` 
       LEFT JOIN `node_devices` 
              ON ( `node_floors`.`fl_id` = `node_devices`.`fl_id` ) 
       LEFT JOIN `node_statuses` AS `l` 
              ON ( `node_devices`.`lc_id` = `l`.`lc_id` 
                   AND `node_devices`.`node_id` = `l`.`node_id` ) 
       LEFT JOIN `node_statuses` AS `r` 
              ON ( `l`.`lc_id` = `r`.`lc_id` 
                   AND `l`.`node_id` = `r`.`node_id` 
                   AND `l`.`t_stamp` < `r`.`t_stamp` ) 
WHERE  `r`.`t_stamp` IS NULL 
       AND `node_floors`.`lc_id` = $location_id 
GROUP  BY `fl_name` 
ORDER BY `node_floors`.`fl_id`";
$result = $mysqli->query($query);

$table = array();
while ($row = $result->fetch_array(MYSQLI_ASSOC)) {
    array_push($table, $row);
}

echo json_encode($table);