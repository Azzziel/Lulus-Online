<?php

$device_timezone = 'PT07H';

$auth_key = '2fad7ae4be04ce093c8a9e48511fdf6c';

$db_host = 'mysql.hostinger.in';
$db_user = 'u216319883_vankur';
$db_pass = 'atmberasgagal';
$db_name = 'u216319883_nodesens_db';

$mysqli = new mysqli($db_host, $db_user, $db_pass, $db_name);

if ($mysqli->connect_error) {
    die('Connect Error (' . $mysqli->connect_errno . ') ' . $mysqli->connect_error);
}
