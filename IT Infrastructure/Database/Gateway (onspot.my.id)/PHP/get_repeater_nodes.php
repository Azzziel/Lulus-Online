<?php

include 'config.php';

// $_GET
// $_POST

if (($auth_key != $_GET['auth_key']) && ($auth_key != $_POST['auth_key'])) {
    exit();
}

$query = "
SELECT `rptr_id`, 
       `send_rt` 
FROM   `node_repeaters`";

$result = $mysqli->query($query);

echo json_encode($result->fetch_all(MYSQLI_ASSOC));
