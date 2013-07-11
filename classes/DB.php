<?php

class DB {

    private static $_db;

    public function __construct() {
        include DR . '/system.php';
        self::$_db = new mysqli($Host, $User, $Password, $DB);
        if (self::$_db->connect_error) {
            die('Ошибка подключения (' . self::$_db->connect_errno . ') '
                    . self::$_db->connect_error);
        }
    }

    public static function getDB() {
        return self::$_db;
    }

    public static function query($query) {
        try {
            $result = self::$_db->query($query);
            if (self::$_db->errno) {
                throw new Exception($query);
            }
        } catch (Exception $exc) {
            die((isset($_SESSION['access']) && $_SESSION['access'] == 'admin') ? $exc->getMessage() . ';<br>
' . self::$_db->error . '<br>
' . nl2br($exc->getTraceAsString()) : 'Ошибка базы данных.');
        }
        return $result;
    }

    public static function real_escape_string($str) {
        return self::$_db->real_escape_string($str);
    }

    public static function insert_id() {
        return self::$_db->insert_id;
    }

    public static function close() {
        self::$_db->close();
    }

}