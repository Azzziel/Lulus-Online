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

$query = "
SELECT `rptr_id`, 
       `send_rt` 
FROM   `node_repeaters` 
WHERE  `lc_id` = $lc_id";

$result = $mysqli->query($query);

echo json_encode($result->fetch_all(MYSQLI_ASSOC));
