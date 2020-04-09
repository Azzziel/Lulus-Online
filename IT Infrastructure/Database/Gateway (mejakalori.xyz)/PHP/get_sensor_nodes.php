<?php

include 'config.php';

// $_GET
// $_POST

if (($auth_key != $_GET['auth_key']) && ($auth_key != $_POST['auth_key'])) {
    exit();
}

$query_index = $_POST['query_index'];
$query_limit = $_POST['query_limit'];

if ($query_index === 'all') {
    $query = "
    SELECT `node_id`, `disp_rt` 
    FROM   `node_devices` 
    WHERE  `on_serv`=true 
    ORDER  BY `index`";

    $result = $mysqli->query($query);

    echo json_encode($result->fetch_all(MYSQLI_ASSOC));

    $result->close();
    $mysqli->close();
    exit();
}

if (empty($query_index) || $query_index <= 0) {
    $mysqli->close();
    exit();
}

if (empty($query_limit) || $query_limit <= 0) {
    $mysqli->close();
    exit();
}

$query = "SELECT `index` FROM `node_devices` WHERE `on_serv`=true ORDER BY `index`";
$result = $mysqli->query($query);

$index_array = array();
while ($row_data = $result->fetch_row()[0]) {
    array_push($index_array, $row_data);
}

$filtered_query_index = $index_array[$query_index - 1];

$query = "
SELECT `node_id`, 
       `disp_rt` 
FROM   `node_devices` 
WHERE  `node_devices`.`index` >= $filtered_query_index 
       AND `on_serv`=true 
ORDER  BY `index` 
LIMIT  $query_limit";

$result = $mysqli->query($query);

echo json_encode($result->fetch_all(MYSQLI_ASSOC));

$result->close();
$mysqli->close();
