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

if (empty($battery) || $battery <= 0 || $battery >= 5) {
    exit();
}

$query = "INSERT INTO `node_batteries` (`node_id`, `battery`) VALUES ('$node_id', $battery)";

if ($result = $mysqli->query($query)) {
    echo 'SUCCESS';
} else {
    echo 'FAILED';
}

$result->close();
$mysqli->close();
