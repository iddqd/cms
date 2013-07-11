<?php

class file_class {

    public static $file, $header, $content;

    public static function main() {
        $url = DR . static::$file;
        if (!file_exists($url)) {
            static::$content = '<p>Файл ' . $url . ' не найден!</p>';
        } else {
            static::$content = '
    <h2>' . static::$header . '</h2>
    <p>' . static::$file . '</p>
    <form method="POST" action="?inc=' . $_GET['inc'] . '&action=save">
        <p><textarea name="new" style="width:100%" rows="30">' . file_get_contents($url) . '</textarea></p>
        <p><input type="submit" value="Сохранить"></p>
    </form>';
        }
    }

    public static function save() {
        if (FALSE !== file_put_contents(DR . static::$file, $_POST['new'])) {
            static::$content = '<a href="?inc=' . $_GET['inc'] . '" class="button">Изменить ' . static::$file . '</a><br>
            <textarea rows="30" style="width:100%" disabled>' . $_POST['new'] . '</textarea>';
        } else {
            static::$content = '<a href="/admin/?inc=' . $_GET['inc'] . '" class="button">Назад...</a>
                <br><br><span class="error">Не удалось записать изменения в файл!</span>';
        }
    }

}
