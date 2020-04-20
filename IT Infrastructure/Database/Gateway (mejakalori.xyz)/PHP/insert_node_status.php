<?php

include 'config.php';

// $_GET
// $_POST

if (($auth_key != $_GET['auth_key']) && ($auth_key != $_POST['auth_key'])) {
    exit();
}

$node_id = $_POST['node_id'];
$n_stats = $_POST['n_stats'];

if (empty($node_id)) {
    exit();
}

if ($n_stats != 0 && $n_stats != 1) {
    exit();
}

$query = "
SELECT `n_stats` 
FROM   `node_statuses` 
WHERE  `node_id` = '$node_id' 
ORDER  BY `t_stamp` DESC 
LIMIT  1";

$result = $mysqli->query($query);
$previous_n_stats = $result->fetch_row()[0];

if ($previous_n_stats == $n_stats) {
    echo 'SUCCESS';
    exit();
}

$query = "INSERT INTO `node_statuses` (`node_id`, `n_stats`) VALUES ('$node_id', '$n_stats')";

if ($result = $mysqli->query($query)) {
    echo 'SUCCESS';
} else {
    echo 'FAILED';
}
