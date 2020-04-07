<?php

include 'config.php';

// $_GET
// $_POST

if (($auth_key != $_GET['auth_key']) && ($auth_key != $_POST['auth_key'])) {
    exit();
}

$query = "SELECT COUNT(`node_id`) FROM `node_devices`";
$result = $mysqli->query($query);
$row = $result->fetch_array(MYSQLI_NUM);

if (isset($row)) {
    echo $row[0];
} else {
    echo '0';
}

$result->close();
$mysqli->close();
