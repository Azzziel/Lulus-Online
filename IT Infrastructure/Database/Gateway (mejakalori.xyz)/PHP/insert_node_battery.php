<?php

include 'config.php';

// $_GET
// $_POST

if (($auth_key != $_GET['auth_key']) && ($auth_key != $_POST['auth_key'])) {
    exit();
}

$node_id = $_POST['node_id'];
$battery = $_POST['battery'];

if (empty($node_id)) {
    exit();
}

if ($battery != 1 && $battery != 2 && $battery != 3 && $battery != 4) {
    exit();
}

$query = "
SELECT `battery` 
FROM   `node_batteries` 
WHERE  `node_id` = '$node_id' 
ORDER  BY `t_stamp` DESC 
LIMIT  1";

$result = $mysqli->query($query);
$previous_battery = $result->fetch_row()[0];

if ($previous_battery == $battery) {
    echo 'SUCCESS';
    exit();
}

$query = "INSERT INTO `node_batteries` (`node_id`, `battery`) VALUES ('$node_id', '$battery')";

if ($result = $mysqli->query($query)) {
    echo 'SUCCESS';
} else {
    echo 'FAILED';
}
