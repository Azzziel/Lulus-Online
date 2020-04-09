<?php

include 'config.php';

// $_GET
// $_POST

if (($auth_key != $_GET['auth_key']) && ($auth_key != $_POST['auth_key'])) {
    exit();
}

$query = "
SELECT `disp_id`, 
       `recv_rt` 
FROM   `node_displays` 
WHERE `on_serv`=true";

$result = $mysqli->query($query);

echo json_encode($result->fetch_all(MYSQLI_ASSOC));

$result->close();
$mysqli->close();
