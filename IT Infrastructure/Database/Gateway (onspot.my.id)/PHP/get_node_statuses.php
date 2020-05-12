<?php

include 'config.php';

// $_GET
// $_POST

if (($auth_key != $_GET['auth_key']) && ($auth_key != $_POST['auth_key'])) {
    exit();
}

$node_id = $_POST['node_id'];

if (empty($node_id)) {
    exit();
}

if ($node_id === 'all') {
    // Do not query this from a microcontroller
    $query = "
    SELECT `left_set`.`node_id`, 
           `left_set`.`n_stats` 
    FROM   `node_statuses` `left_set` 
           LEFT JOIN `node_statuses` `right_set` 
                  ON ( `left_set`.`node_id` = `right_set`.`node_id` 
                       AND `left_set`.`index` < `right_set`.`index` ) 
    WHERE  `right_set`.`index` IS NULL 
    ORDER BY `node_id`";

    $result = $mysqli->query($query);

    echo json_encode($result->fetch_all(MYSQLI_ASSOC));

    exit();
}

$query = "SELECT `n_stats` FROM `node_statuses` WHERE `node_id`='$node_id' ORDER BY `t_stamp` DESC LIMIT 1";
$result = $mysqli->query($query);

echo $result->fetch_row()[0];
