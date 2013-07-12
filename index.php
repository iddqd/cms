<?php
//ini_set('display_errors', 'on');
//error_reporting(E_ALL);
session_name('SID');
session_start();
header('Content-type: text/html;charset=UTF-8');
define('DR', rtrim($_SERVER['DOCUMENT_ROOT'],'/'));
 
function __autoload($class) {
    include  'classes/' . $class . '.php';
    new $class;
}
DB::getDB()->set_charset('utf8');
Page::show();
DB::close();
