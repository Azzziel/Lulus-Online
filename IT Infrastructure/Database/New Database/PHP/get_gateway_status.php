<?php

include 'config.php';

// $_GET
// $_POST

if (($auth_key != $_GET['auth_key']) && ($auth_key != $_POST['auth_key'])) {
    exit();
}

$gtwy_id = $_POST['gtwy_id'];
$lc_id = $_POST['lc_id'];

if (empty($gtwy_id)) {
    exit();
}

if (empty($lc_id) || $lc_id <= 0) {
    exit();
}

$query = "SELECT COUNT(*) FROM `node_gateways` WHERE `gtwy_id` = '$gtwy_id' AND `lc_id` = $lc_id";
$result = $mysqli->query($query);
$count = $result->fetch_row()[0];

if ($count > 0) {
    echo '1';
} else {
    echo '0';
}
