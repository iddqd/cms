<?php
ini_set('display_errors', 'on');
error_reporting(E_ALL);
header('Content-type: text/html;charset=UTF-8');
define('DR', rtrim($_SERVER['DOCUMENT_ROOT'], '/'));
define('CORE', '/usr/share/pear');

function __autoload($class) {
    include 'classes/' . $class . '.php';
    new $class;
}

DB::getDB()->set_charset('utf8');
if (isset($_GET['memberpass'])) {
    include 'includes/memberpass.inc';
    die;
} else {
    include 'includes/dst.inc';
}
if (isset($_GET['inc'])) {
    $includes = array_merge(scandir(DR . '/admin/includes/'), scandir(CORE . '/includes/'));
    if (in_array($_GET['inc'] . '.inc', $includes)) {
        $inc = $_GET['inc'];
    } else {
        $inc = 'main_page';
    }
} else {
    $inc = 'main_page';
}
include 'includes/' . $inc . '.inc';
try {
    if (isset($_GET['action']) && method_exists($inc, $_GET['action'])) {
        $inc::$_GET['action']();
    } else {
        $inc::main();
    }
} catch (Exception $e) {
    $inc::$content = '<p style="color:red;margin:30px">' . $e->getMessage() . '</p>';
}
include 'includes/view.inc';