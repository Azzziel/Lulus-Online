<?php

include 'config.php';

// $_GET
// $_POST

if (($auth_key != $_GET['auth_key']) && ($auth_key != $_POST['auth_key'])) {
    exit();
}

$query_index = $_POST['query_index'];
$query_limit = $_POST['query_limit'];

if (empty($query_index) || $query_index <= 0) {
    exit();
}

if (empty($query_limit) || $query_limit <= 0) {
    exit();
}

$query = "
SELECT `node_id`, 
       `disp_rt`, 
       (SELECT `n_stats` 
        FROM   `node_statuses` 
        WHERE  `node_id` = `node_devices`.`node_id` 
        ORDER  BY `t_stamp` DESC 
        LIMIT  1) AS 'n_stats', 
       (SELECT `battery` 
        FROM   `node_batteries` 
        WHERE  `node_id` = `node_devices`.`node_id` 
        ORDER  BY `t_stamp` DESC 
        LIMIT  1) AS 'battery' 
FROM   `node_devices` 
WHERE  `node_devices`.`index` >= $query_index 
ORDER  BY `index` 
LIMIT  $query_limit";

$result = $mysqli->query($query);

echo json_encode($result->fetch_all(MYSQLI_ASSOC));
