<?php

include 'config.php';

// $_GET
// $_POST

if (($auth_key != $_GET['auth_key']) && ($auth_key != $_POST['auth_key'])) {
    exit();
}

$query = "SELECT COUNT(*) FROM `node_devices` LIMIT 1";
$result = $mysqli->query($query);
$row = $result->fetch_row()[0];

if (isset($row)) {
    echo $row;
} else {
    echo '0';
}
