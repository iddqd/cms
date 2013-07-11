<?php

class css {

    public static $content = '', $title = 'Стили CSS';

    public static function main() {
        self::$content .= '<a href="?inc=pattern&action=start" class="button"><b>Назад</b></a>
            <a href="?inc=css&action=add" class="button"><b>Добавить</b></a><br><br>
            <center><table width="90%" cellpadding="3" cellspacing="3">
	<tr style="border: 1px solid #000000; background: #aaa; color: #2275AA; font-weight: bold;">
	<td>Файл</td><td>Описание</td><td>Удаление</td></tr>';
        $query = DB::query("SELECT * FROM css");
        while ($css = $query->fetch_array()) {
            self::$content.='<tr style="border: 1px solid #000000; background: #fff; color:#2275AA;">
                <td><a href="?inc=css&action=edit&id=' . $css['id'] .
                    '">' . $css['css_path'] . '</a></td><td><a href="?inc=css&action=edit&id=' . $css['id'] . '">'
                    . $css['css_desc'] .
                    '</a></td><td><a href="?inc=css&action=delete&id=' . $css['id'] . '" class="confirm"> Удалить </a></td></tr>';
        }
        self::$content.='</table></center>';
    }

    public static function add() {
        self::$content = '
	<a href="?inc=css" class="button">К списку</a><br><br>
	<form method="POST" enctype="multipart/form-data" action="?inc=css&action=process_add">
		<h3>Добавление</h3>
		<table>
			<tr>
				<td>Имя файла</td>
				<td> <input type="text" name="file_name" /><br/></td>
			</tr>
			<tr>
				<td>Файл css</td>
				<td><input type="file" name="modul_file"><br/></td>
			</tr>
			<tr>
				<td>Описание</td>
				<td> <textarea name="comment" cols="31" rows="3"></textarea></td>
			</tr>
		</table>
	   <p><input type="submit" value="Загрузить"></p>
	</form>
	';
    }

    public static function process_add() {
        if (isset($_POST['file_name']) && !empty($_POST['file_name'])) {
            $file_name = $_POST['file_name'];
            $comment = $_POST['comment'];
            $uploaddir = DR . '/css/';
            if (!file_exists($uploaddir . $_POST['file_name'])) {
                if (move_uploaded_file($_FILES['modul_file']['tmp_name'], $uploaddir . $_POST['file_name'])) {
                    if (DB::query("INSERT INTO css SET css_path='$file_name',css_desc='$comment'")) {
                        header('Location: /admin/?inc=css');
                        die;
                    } else {
                        self::$content .= "<p>Ошибка! Новая запись в таблицу css не была добавленна</p>";
                    }
                } else {
                    self::$content .= "<p>Внимание! Файл не был загружен</p>";
                }
            } else {
                self::$content .= "<p>Внимание! Файл с таким именем уже есть! Будет использоватся старый файл.</p>";
            }
        } else {
            self::$content .= "<p>Ошибка! Необходимо имя файла </p>";
        }
        self::main();
    }

    public static function delete() {
        if (isset($_GET['id']) && $id = abs((int) $_GET['id'])) {
            $file_url = DR . '/css/' . DB::query("SELECT css_path FROM css WHERE id=$id")->fetch_array()[0];
            if (file_exists($file_url)) {
                unlink($file_url);
            }
            DB::query("DELETE FROM css WHERE id=$id");
            header('Location: /admin/?inc=css');
            die;
        }
    }

    public static function edit() {
        if (isset($_GET['id']) && $id = abs((int) $_GET['id'])) {
            $row = DB::query("SELECT css_path,css_desc FROM css WHERE id=$id")->fetch_array();
            $file_url = DR . '/css/' . $row['css_path'];
            if (file_exists($file_url)) {
                $text = stripcslashes(file_get_contents($file_url));
            }
            self::$content .= '
		<a href="?inc=css" class="button">К списку</a>
		<a href="?inc=css&action=delete&id=' . $id . '" class="button confirm">*Удалить</a><br>
		<form method="POST" enctype="multipart/form-data" action="?inc=css&action=process_edit">
			<input type="hidden" name="id" value="' . $id . '">
			<table>
				<tr><td>Имя файла<div style="float:right;text-align:right">/css/</div></td>
                                <td><input type="text" name="file_name" value="' . $row['css_path'] . '"><br></td></tr>
				<tr><td>Содержимое файла</td><td><textarea name="content" cols="60" rows="30">' . $text . '</textarea><br></td></tr>
				<tr><td>Описание</td><td><textarea name="comment" cols="31" rows="3">' . $row['css_desc'] . '</textarea></td></tr>
			</table>
			<p><input type="submit" value="Загрузить"></p>
		</form>
		';
        } else {
            self::main();
        }
    }

    public static function process_edit() {

        if (isset($_POST['id']) && $id = abs((int) $_POST['id'])) {
            if ($_POST['file_name']) {
                $data['css_path'] = $_POST['file_name'];
                $data['css_desc'] = $_POST['comment'];
                $text = stripcslashes($_POST['content']);
                $query .=" UPDATE css SET ";
                foreach ($data as $key => $value) {
                    $query .= "$key='$value',";
                }
                $query .= " id=$id WHERE id=$id";
                $result = DB::query($query);
                $file_url = DR . '/css/' . $_POST['file_name'];
                file_put_contents($file_url, $text);
                header('Location: /admin/?inc=css');
                die;
            } else {
                self::$content .= "<p>Ошибка! Необходимо имя файла</p>";
                self::main();
            }
        }
    }

}