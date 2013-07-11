<?php
ini_set('display_errors','on');
error_reporting(E_ALL);
class modules {

    public static $content = '', $title = 'Модули', $active_row;

    public static function unuse() {
        $result = DB::query("SELECT * FROM modules");
        while ($row = $result->fetch_object()) {
            DB::query("UPDATE `modules` SET `use` = 'no' WHERE id=$row->id");
        }
        self::$action = 'show';
        self::$show();
    }

    public static function main() {
        self::$content .= '<p><a href="?inc=modules&action=add" class="button"><b>Добавить</b></a></p>';
        $result = DB::query("SELECT * FROM modules ORDER BY mark");
        if ($result->num_rows) {
            $bgcolor = '#DFEFFF';
            self::$content .= '
                    <table border="0" cellspacing="0" cellpadding="5">
                        <tr bgcolor=silver>
                            <td><b>Метка</b></td>
                            <td><b>Имя файла</b></td>
                            <td><b>Вкл./выкл.</b></td>
                            <td><b>Использование</b></td>
                            <td><b>Удалить*</b></td>
                            <td><b>Описание</b></td>
                        </tr>';
            while ($row = $result->fetch_object()) {
                if (!file_exists(DR . '/modules/' . $row->file_name))
                    $file_exists = '<small> :: Фaйл не найден!<small>'; else
                    $file_exists = null;
                if ($bgcolor == '#DFEFFF')
                    $bgcolor = '#EAFFEA'; else
                    $bgcolor = '#DFEFFF';
                if ($row->include == 'off')
                    $bgcolor = '#D9DBDC';
                if (self::$active_row == $row->id)
                    $bgcolor = "#79b465";
                if ($row->include == 'on')
                    $incl = 'Выключить';
                else
                    $incl = 'Включить</a>';
                self::$content .= "
                        <tr bgcolor='$bgcolor' >
                            <td><a href='?inc=modules&action=edit&id=$row->id'> $row->mark </a></td>
                            <td> $row->file_name $file_exists</td>
                            <td><a href='?inc=modules&action=include_module&id=$row->id'>$incl</a></td>
                            <td>$row->use</td>
                            <td><a href='?inc=modules&action=delete&id=$row->id' class='confirm'> Удалить </a></td>
                            <td><small>$row->comment</small></td>
                        </tr>";
            }
            self::$content .= '</table>
                <p><a href="?inc=modules&action=unuse">Обнулить флаг "Использование"</a></p>
                <p><small>* Удаление не желательно, т.к. происходит удаление строки о модуле в таблице modules,  и удаление файла,
                 что приводит к отключению модуля,<br/> НО не удалению порожденных работой модуля таблиц в базе данных, файлоы и т.д.</small></p>';
        } else {
            self::$content .= '<font color="red">В данный момент список пуст!</font><br><br>';
        }
    }

    public static function add() {
        self::$content .= '
            <a href="?inc=modules&action=show" class="button">К списку</a><br><br>
            <form method="POST" enctype="multipart/form-data" action="?inc=modules&action=process_add">
                <h3>Добавление</h3>
                <table>
                    <tr>
                        <td>Имя файла</td>
                        <td><input type="text" name="file_name" size="40"></td>
                    </tr>
                    <tr>
                        <td>Файл модуля</td>
                        <td><input type="file" name="modul_file"></td>
                    </tr>
                    <tr>
                        <td>Описание</td>
                        <td> <textarea name="comment" cols="31" rows="3"></textarea></td>
                    </tr>
                </table>
               <p><input type="submit" value="Загрузить"></p>
            </form>
            <p>Для добавления модуля необходимо добавить название метки,<br/>
            также добавить имя файла, если он уже есть в системе,<br/>
			или добавить этот файл. </p>
            ';
    }

    public static function process_add() {
        if ($_REQUEST['mark'] AND $_REQUEST['file_name'] OR $_REQUEST['mark'] AND is_uploaded_file($_FILES['modul_file']['tmp_name'])) {
            $mark = $_REQUEST['mark'];
            $mark = preg_replace('/[^a-z0-9\s]/', '', $mark);
            $file_name = $_REQUEST['file_name'];
            $comment = $_REQUEST['comment'];
            $uploaddir = CORE . '/modules/';
            if (!file_exists($uploaddir . $_FILES['modul_file']['name'])) {
                if (move_uploaded_file($_FILES['modul_file']['tmp_name'], $uploaddir . $_FILES['modul_file']['name'])) {
                    $file_name = $_FILES['modul_file']['name'];
                }
                else
                    self::$content .= "<p>Внимание! Файл не был загружен</p>";
            }
            else
                self::$content .= "<p>Внимание! Файл с таким именем уже есть! Будет использоватся старый файл.</p>";
            if (DB::query("
                    INSERT INTO modules
                    SET
                        mark='$mark',
                        file_name='$file_name',
                        comment='$comment'")) {
                self::$active_row = $id;
            } else {
                self::$active_row = null;
                self::$content .= "<p>Ошибка! Новая запись в таблицу modules не была добавленна</p>";
            }
        }else
            self::$content .= "<p>Ошибка! Необходима метка и имя файла </p>";
        self::$action = 'show';
        self::$show();
    }

    public static function include_module() {
        if (isset($_REQUEST['id']))
            $id = $_REQUEST['id']; else {
            echo 'Нет! 1';
            exit;
        };

        $query = "SELECT * FROM modules WHERE id=$id LIMIT 1";
        $result = DB::query($query);
        $row = $result->fetch_object();

        if ($row->include == 'on')
            $include = 'off'; else
            $include = 'on';

        $query = "UPDATE modules SET include='$include' WHERE id=$id";
        $result = DB::query($query);

        self::$active_row = $id;
        self::$action = 'show';
        self::$show();
    }

    public static function delete() {
        if (isset($_REQUEST['id']))
            $id = $_REQUEST['id']; else {
            echo 'Нет! 1';
            exit;
        };
        $query = "SELECT * FROM modules WHERE id=$id LIMIT 1";
        $result = DB::query($query);
        $row = $result->fetch_object();
        $uploaddir = CORE . '/modules/';
        if ($row->include == 'off') {
            if (file_exists($uploaddir . $row->file_name)) {
                unlink($uploaddir . $row->file_name);
            }
            DB::query("DELETE FROM modules WHERE id=$id LIMIT 1");
            self::$active_row = null;
            self::$content .= '<p>Модуль удален!</p>';
            self::$action = 'show';
            self::$show();
        } else {
            self::$content .= '<p>Модуль включен! Сначала выключите.</p>';
            self::$action = 'show';
            self::$show();
        }
    }

    public static function edit() {
        if (isset($_REQUEST['id']))
            $id = abs((int)$_REQUEST['id']); else {
            echo 'Нет! 1';
            exit;
        };
        $result = DB::query("SELECT * FROM modules WHERE id=$id");
        $row = $result->fetch_object();
        self::$content .= "
                <a href='?inc=modules&action=show' class='button'>К списку</a>
                <a href='?inc=modules&action=delete&id=$id' class='button confirm'>*Удалить</a><br><br>
                <form method='POST' enctype='multipart/form-data' action='?inc=modules&action=process_edit&id=$id'>
                    <table>
                        <tr><td>Метка</td><td><input type='text' name='mark' size='40' value='$row->mark'></td></tr>
                        <tr><td>Имя файла</td><td><input type='text' name='file_name' size='40' value='$row->file_name'></td></tr>
                        <tr><td>Файл модуля</td><td><input type='file' name='modul_file'></td> </tr>
                        <tr><td>Описание</td><td><textarea name='comment' cols='31' rows='3'>$row->comment</textarea></td></tr>
                    </table>
                    <p><input type='submit' value='Загрузить'></p>
                </form>
                <p><small>* Удаление не желательно, т.к. происходит удаление строки о модуле в таблице modules, и удаление файла,
                 что приводит к отключению модуля,<br/> НО не удалению порожденных работой модуля таблиц в базе данных, файлоы и т.д.</small></p>
                ";
    }

    public static function process_edit() {
        if (isset($_REQUEST['id']))
            $id = abs((int)$_REQUEST['id']); else {
            echo 'Нет! 1';
            exit;
        };
        if ($_REQUEST['mark'] && $_REQUEST['file_name']) {
            if (move_uploaded_file($_FILES['modul_file']['tmp_name'], DR . '/modules/' . $_FILES['modul_file']['name'])) {
                $file_name = $_FILES['modul_file']['name'];
                //echo $file_name;
            }
            else
                self::$content .= "<p>Внимание! Файл не был загружен</p>";
            $data['mark'] = $_REQUEST['mark'];
            $data['file_name'] = $_REQUEST['file_name'];
            $data['comment'] = $_REQUEST['comment'];

            $query =" UPDATE modules SET ";
            foreach ($data as $key => $value) {
                $query .= "$key='$value',";
            }
            $query=rtrim($query,',');
            $query .= " WHERE id=$id";
            $result = DB::query($query);
            self::$active_row = $id;
            self::main();
        }else
            self::$content .= "<p>Ошибка! Необходима метка и имя файла</p>";
    }

}
