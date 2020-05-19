<?php

$device_timezone = 'PT07H';

$auth_key = '5cb4bf3e42ff76ca9186850b9017bdc8';

$db_host = 'dci06.dewaweb.com';
$db_user = 'onspotmy_tim_imba_sk20';
$db_pass = 'bersamalulusonline20';
$db_name = 'onspotmy_node_new_db';

$mysqli = new mysqli($db_host, $db_user, $db_pass, $db_name);

if ($mysqli->connect_error) {
    die('Connect Error (' . $mysqli->connect_errno . ') ' . $mysqli->connect_error);
}
