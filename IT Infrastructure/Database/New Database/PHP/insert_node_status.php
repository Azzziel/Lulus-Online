<?php

include 'config.php';

// $_GET
// $_POST

if (($auth_key != $_GET['auth_key']) && ($auth_key != $_POST['auth_key'])) {
    exit();
}

$node_id = $_POST['node_id'];
$n_stats = $_POST['n_stats'];
$lc_id = $_POST['lc_id'];

if (empty($node_id)) {
    exit();
}

if ($n_stats != 0 && $n_stats != 1) {
    exit();
}

if (empty($lc_id) || $lc_id <= 0) {
    exit();
}

$query = "
SELECT `n_stats`, 
       `t_stamp` 
FROM   `node_statuses` 
WHERE  `node_id` = '$node_id' 
       AND `lc_id` = 1 
ORDER  BY `t_stamp` DESC 
LIMIT  1";

$result = $mysqli->query($query);
$row = $result->fetch_row();

$previous_n_stats = $row[0];
$previous_t_stamp = $row[1];

$current_unix_stamp = time();
$current_time_stamp = date("Y-m-d H:i:s", $current_unix_stamp);

if ($previous_n_stats == $n_stats) {
    echo 'SUCCESS';
} else {
    if ($n_stats == 0 && isset($previous_t_stamp)) {
        $previous_unix_stamp = strtotime($previous_t_stamp);
        $s_spent = $current_unix_stamp - $previous_unix_stamp;

        $query = "
        INSERT INTO `node_summaries` 
                    (`lc_id`, 
                     `node_id`, 
                     `s_spent`, 
                     `t_in`, 
                     `t_out`) 
        VALUES      ($lc_id, 
                     '$node_id', 
                     $s_spent, 
                     '$previous_t_stamp', 
                     '$current_time_stamp')";

        if (!($result = $mysqli->query($query))) {
            echo 'FAILED';
            exit();
        }
    }

    $query = "
    INSERT INTO `node_statuses` 
                (`lc_id`, 
                 `node_id`, 
                 `n_stats`, 
                 `t_stamp`) 
    VALUES      ($lc_id, 
                 '$node_id', 
                 '$n_stats', 
                 '$current_time_stamp')";

    if ($result = $mysqli->query($query)) {
        echo 'SUCCESS';
    } else {
        echo 'FAILED';
    }
}
