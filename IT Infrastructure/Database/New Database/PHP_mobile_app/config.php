<?php

$device_timezone = 'PT07H';

$auth_key = '98d11adf61a1fc9ab76ee375d0600170';

$db_host = 'dci06.dewaweb.com';
$db_user = 'onspotmy_tim_imba_sk20';
$db_pass = 'bersamalulusonline20';
$db_name = 'onspotmy_node_db';

$mysqli = new mysqli($db_host, $db_user, $db_pass, $db_name);

if ($mysqli->connect_error) {
    die('Connect Error (' . $mysqli->connect_errno . ') ' . $mysqli->connect_error);
}
