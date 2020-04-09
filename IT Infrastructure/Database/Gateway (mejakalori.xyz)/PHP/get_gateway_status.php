<?php

include 'config.php';

// $_GET
// $_POST

if (($auth_key != $_GET['auth_key']) && ($auth_key != $_POST['auth_key'])) {
    exit();
}

$gtwy_id = $_POST['gtwy_id'];

if (empty($gtwy_id)) {
    exit();
}

$query = "SELECT COUNT(*) FROM `node_gateways` WHERE `gtwy_id`='$gtwy_id' AND `on_serv`=true";
$result = $mysqli->query($query);

echo $result->fetch_row(MYSQLI_NUM)[0];

$result->close();
$mysqli->close();
