<?php

class main_menu {

    private $menu = '<div align="center" class="menu">', $subMenu = '<div align="center" class="menu submenu">';

    function __construct() {
        $parent_directory = '';
        if (Page::$id == 1) {
            $id = $parent = DB::query('SELECT id FROM sections WHERE depth=1 AND visible=1 ORDER BY leftk LIMIT 1')->fetch_array()[0];
        } else {
            $id = Page::$id;
            $parent = (Page::$depth == 2) ? Page::$parent : Page::$id;
        }
        $result = DB::query('SELECT id,directory,name,depth FROM sections WHERE (depth=1 OR (depth=2 AND parent=' . $parent . ')) AND visible=1 ORDER BY depth,leftk');
        while ($row = $result->fetch_array()) {
            $current = ($row['id'] == $id || $parent == $row['id']) ? ' class="current"' : '';
            if ($row['id'] == $parent)
                $parent_directory = $row['directory'] . '/';
            if ($row['depth'] == 1) {
                $this->menu .='    <a href="/' . $row['directory'] . '"' . $current . '>' . $row['name'] . '</a>' . PHP_EOL;
            } else {
                $this->subMenu .='    <a href="/' . $parent_directory . $row['directory'] . '"' . $current . '>' . $row['name'] . '</a>' . PHP_EOL;
            }
        }
        $this->menu .='</div>';
        $this->subMenu .='</div>';
    }

    function __toString() {
        return $this->menu . $this->subMenu;
    }

}