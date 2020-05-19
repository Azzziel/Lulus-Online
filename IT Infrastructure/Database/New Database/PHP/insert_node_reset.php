<?php

include 'config.php';

// $_GET
// $_POST

if (($auth_key != $_GET['auth_key']) && ($auth_key != $_POST['auth_key'])) {
    exit();
}

$node_id = $_POST['node_id'];
$lc_id = $_POST['lc_id'];

if (empty($node_id)) {
    exit();
}

if (empty($lc_id) || $lc_id <= 0) {
    exit();
}

$query = "INSERT INTO `node_resets` (`lc_id`, `node_id`) VALUES ($lc_id, '$node_id')";

if ($result = $mysqli->query($query)) {
    echo 'SUCCESS';
} else {
    echo 'FAILED';
}
