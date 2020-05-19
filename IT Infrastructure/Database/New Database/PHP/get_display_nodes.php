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
SELECT `disp_id`, 
       `recv_rt`, 
       `is_main` 
FROM   `node_displays` 
WHERE  `lc_id` = $lc_id 
ORDER BY `index`";

$result = $mysqli->query($query);

echo json_encode($result->fetch_all(MYSQLI_ASSOC));
