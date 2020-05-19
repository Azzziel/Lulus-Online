<?php

include 'config.php';

// $_GET
// $_POST

if (($auth_key != $_GET['auth_key']) && ($auth_key != $_POST['auth_key'])) {
    exit();
}

$query_index = $_POST['query_index'];
$query_limit = $_POST['query_limit'];
$lc_id = $_POST['lc_id'];

if (empty($lc_id) || $lc_id <= 0) {
    exit();
}

if ($query_index === 'all') {
    // Do not query this from a microcontroller
    $query = "
    SELECT `node_id`, 
           `disp_rt`, 
           (SELECT `n_stats` 
            FROM   `node_statuses` 
            WHERE  `node_id` = `node_devices`.`node_id` 
                   AND `lc_id` = $lc_id 
            ORDER  BY `t_stamp` DESC 
            LIMIT  1) AS 'n_stats', 
           (SELECT `battery` 
            FROM   `node_batteries` 
            WHERE  `node_id` = `node_devices`.`node_id` 
                   AND `lc_id` = $lc_id 
            ORDER  BY `t_stamp` DESC 
            LIMIT  1) AS 'battery' 
    FROM   `node_devices` 
    WHERE  `lc_id` = $lc_id";

    $result = $mysqli->query($query);

    echo json_encode($result->fetch_all(MYSQLI_ASSOC));

    exit();
}

if (empty($query_index) || $query_index <= 0) {
    exit();
}

if (empty($query_limit) || $query_limit <= 0) {
    exit();
}

$query = "SELECT `index` FROM `node_devices` WHERE `lc_id` = $lc_id ORDER BY `index`";
$result = $mysqli->query($query);

$index_array = array();
while ($row_data = $result->fetch_row()[0]) {
    array_push($index_array, $row_data);
}

$filtered_query_index = $index_array[$query_index - 1];

$query = "
SELECT `node_id`, 
       `disp_rt`, 
       (SELECT `n_stats` 
        FROM   `node_statuses` 
        WHERE  `node_id` = `node_devices`.`node_id` 
               AND `lc_id` = $lc_id 
        ORDER  BY `t_stamp` DESC 
        LIMIT  1) AS 'n_stats', 
       (SELECT `battery` 
        FROM   `node_batteries` 
        WHERE  `node_id` = `node_devices`.`node_id` 
               AND `lc_id` = $lc_id 
        ORDER  BY `t_stamp` DESC 
        LIMIT  1) AS 'battery' 
FROM   `node_devices` 
WHERE  `node_devices`.`index` >= $filtered_query_index 
       AND `lc_id` = $lc_id 
ORDER  BY `index` 
LIMIT  $query_limit";

$result = $mysqli->query($query);

echo json_encode($result->fetch_all(MYSQLI_ASSOC));
