<?php

include 'config.php';

// $_GET
// $_POST

if (($auth_key != $_GET['auth_key']) && ($auth_key != $_POST['auth_key'])) {
    exit();
}

$lc_id = $_POST['lc_id'];

if (empty($lc_id) || $lc_id <= 0) {
    exit();
}

$query = "SELECT COUNT(*) FROM `node_devices` WHERE `lc_id` = $lc_id";
$result = $mysqli->query($query);
$row = $result->fetch_row()[0];

if (isset($row)) {
    echo $row;
} else {
    echo '0';
}
