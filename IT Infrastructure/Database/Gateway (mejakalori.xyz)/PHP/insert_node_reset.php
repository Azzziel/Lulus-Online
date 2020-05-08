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

$query = "INSERT INTO `node_resets` (`node_id`) VALUES ('$node_id')";

if ($result = $mysqli->query($query)) {
    echo 'SUCCESS';
} else {
    echo 'FAILED';
}
