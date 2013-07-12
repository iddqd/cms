<?php

ini_set('display_errors', 'on');
error_reporting(E_ALL);

class modules {

    public static $content = '', $title = 'Модули';

    public static function main() {
        self::$content .= '<p><a href="?inc=modules&action=add" class="button"><b>Добавить</b></a></p>';
        $content = '';
        $bgcolor = '#EAFFEA';
        foreach (glob(DR . '/modules/*.php', GLOB_NOSORT) as $mdl) {
            $fileIncl = $fileName = pathinfo($mdl, PATHINFO_FILENAME);
            if (strpos($fileName, '$') !== FALSE) {
                $bgcolor = '#D9DBDC';
                $incl = 'Включить';
                $fileName = rtrim($fileName, '$');
            } else {
                $incl = 'Выключить';
            }
            if (isset($_GET['active_row']) AND $fileName == $_GET['active_row']) {
                $bgcolor = "#79b465";
            }
            $file_exists = '<small> :: Фaйл не найден!<small>';
            $use = (DB::query('SELECT 1 FROM patterns,sections WHERE pattern_text LIKE "%<!--{' . $fileName . '%" OR 
            content LIKE "%<!--{' . $fileName . '%" LIMIT 1')->num_rows) ? 'Используется' : 'Не используется';
            $content .= "
                        <tr bgcolor='$bgcolor' >
                            <td><a href='?inc=modules&action=edit&mdl=$fileName'> $fileName </a></td>
                            <td> $fileName.php $file_exists</td>
                            <td><a href='?inc=modules&action=include_module&mdl=$fileIncl'>$incl</a></td>
                            <td>$use</td>
                            <td><a href='?inc=modules&action=delete&mdl=$fileIncl' class='confirm'>Безопасное удаление</a></td>
                            <td><small>" . @file_get_contents(DR . '/modules/' . $fileName . '.cfg') . "</small></td>
                        </tr>";
            $bgcolor = ($bgcolor == '#EAFFEA') ? '#DFEFFF' : '#EAFFEA';
        }
        if ($content) {
            $bgcolor = '#EAFFEA';
            self::$content .= '
                    <table border="0" cellspacing="0" cellpadding="5">
                        <tr bgcolor=silver>
                            <th>Метка</th>
                            <th>Имя файла</th>
                            <th>Вкл./Выкл.</th>
                            <th>Использование</th>
                            <th>Удалить*</th>
                            <th>Описание</th>
                        </tr>
                        ' . $content .
                    '</table>
                <!--p><a href="?inc=modules&action=unuse">Обнулить флаг "Использование"</a></p-->
                <p><small>* Удаление не желательно, т.к. происходит удаление файла,
                 что приводит к отключению модуля,<br/> НО не удалению порожденных работой модуля таблиц в базе данных, файлов и т.д.</small></p>';
            ;
        } else {
            self::$content .= '<font color="red">В данный момент список пуст!</font><br><br>';
        }
    }

    private static function _form($mdl = '', $comment = '') {
        self::$content = <<<FORM
            <a href="?inc=modules" class="button">К списку</a><br><br>
            <form method="POST" enctype="multipart/form-data" action="?inc=modules&action=process_add">
                <h3>Добавление</h3>
                <table>
                    <tr>
                        <td>Метка</td>
                        <td><input type="text" name="file_name" size="40" value="$mdl"></td>
                    </tr>
                    <tr>
                        <td>Файл модуля</td>
                        <td><input type="file" name="modul_file"></td>
                    </tr>
                    <tr>
                        <td>Описание</td>
                        <td> <textarea name="comment" cols="31" rows="3">$comment</textarea></td>
                    </tr>
                </table>
               <p><input type="submit" value="Загрузить"></p>
            </form>           
FORM;
    }

    public static function add() {
        self::_form();
        self::$content .= '<p>Для добавления модуля необходимо указать метку. Файл будет сохранен по именем совпадающем с меткой.<br>
                Если такой уже есть в системе он не будет перезаписан.<br>
		Нельзя использовать в имени файла символ "$"(он будет удален из имени).</p>';
    }

    public static function process_add() {
        if ($file_name = pathinfo(filter_input(INPUT_POST, 'file_name'), PATHINFO_FILENAME) AND is_uploaded_file($_FILES['modul_file']['tmp_name'])) {
            $comment = $_POST['comment'];
            $upload = DR . '/modules/' . $file_name . '.php';
            if (file_exists($upload)) {
                self::$content .= "<p>Внимание! Файл с таким именем уже есть! Будет использоватся старый файл.</p>";
            } else {
                if (move_uploaded_file($_FILES['modul_file']['tmp_name'], $upload)) {
                    file_put_contents(DR . '/modules/' . $file_name . '.cfg', $comment);
                    header('Location: /admin/?inc=modules&active_row=' . $file_name);
                    die;
                } else {
                    self::$content .= "<p>Внимание! Файл не был загружен!</p>";
                }
            }
        } else {
            self::$content .= "<p>Ошибка! Необходимо имя файла!</p>";
        }
        self::main();
    }

    public static function include_module() {
        if (isset($_GET['mdl']) && file_exists(DR . '/modules/' . ($mdl = str_replace('.', '', $_GET['mdl'])) . '.php')) {
            if (strpos($mdl, '$') !== FALSE) {
                rename(DR . '/modules/' . $mdl . '.php', DR . '/modules/' . rtrim($mdl, '$') . '.php');
            } else {
                rename(DR . '/modules/' . $mdl . '.php', DR . '/modules/' . $mdl . '$.php');
            }
            header('Location: /admin/?inc=modules&active_row=' . rtrim($mdl, '$'));
        }
        self::main();
    }

    public static function delete() {
        if ($mdl = filter_input(INPUT_GET, 'mdl')) {
            $file = DR . '/modules/' . $mdl . '.php';
            $cfg = DR . '/modules/' . rtrim($mdl, '$') . '.cfg';
            if (DB::query('SELECT 1 FROM patterns,sections WHERE pattern_text LIKE "%<!--{' . $mdl . '%" OR 
            content LIKE "%<!--{' . $mdl . '%" LIMIT 1')->num_rows) {
                self::$content .= '<p>Модуль используется!</p>';
                $_GET['active_row'] = $mdl;
            } else {
                if (file_exists($file)) {
                    unlink($file);
                }
                if (file_exists($cfg)) {
                    unlink($cfg);
                }
                header('Location: /admin/?inc=modules');
                die;
            }
        }
        self::main();
    }

    public static function edit() {
        if (isset($_GET['mdl']) AND file_exists(($file = DR . '/modules/' . ($mdl = str_replace('.', '', $_GET['mdl']))) . '.php')) {
            self::_form($mdl, file_get_contents($file . '.cfg'));
            self::$content .= '<p>Для добавления модуля необходимо указать метку.
                Файл будет сохранен под именем совпадающем с меткой.<br>
                Если такой уже есть в системе он не будет перезаписан.
		Нельзя использовать в имени файла символ "$"(он будет удален из имени).</p>';
        } else {
            self::main();
        }
    }

}
