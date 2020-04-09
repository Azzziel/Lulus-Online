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

if (empty($n_stats) || ($n_stats != 0 && $n_stats != 1)) {
    exit();
}

$query = "INSERT INTO `node_statuses` (`node_id`, `n_stats`) VALUES ('$node_id', $n_stats)";

if ($result = $mysqli->query($query)) {
    echo 'SUCCESS';
} else {
    echo 'FAILED';
}

$result->close();
$mysqli->close();
