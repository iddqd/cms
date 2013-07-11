<?php

class uploader {

    public static $content = '', $title = 'Загрузка файлов';

    public static function main() {
        if (file_exists(DR . '/files/')) {
            self::$content .= '
<form method="POST" enctype="multipart/form-data" action="?inc=uploader&action=upload">
	<b>Загрузить файл</b><br>
	<input type="hidden" name="action" value="upload">
	<input type="file" name="up_file" size="20">
	<input type="submit" value="Загрузить" name="sender">
</form>
<form method="POST" action="?inc=uploader&action=really_delete">
	<b>Список файлов</b><br>
	<select size="30" name="file_name" style="width: 300">';
            $dir = opendir(DR . '/files/');
            $s = ' selected';
            while (($res = readdir($dir)) !== FALSE) {
                if (($res != '.') && ($res != '..')) {
                    self::$content .= '<option value="' . $res . '"' . $s . '>' . $res . '</option>';
                    $s = '';
                }
            }

            self::$content .= '</select>
               <input type="hidden" value="forma" name="action">
                <br><input type="submit" value="Удалить" name="delete" style="color: #FF0000; float: left;">
	</form>';
        } else {
            throw new Exception('Отсутствует диирктория "/files"!');
        }
    }

    public static function really_delete() {
        if (isset($_GET['file_name'])) {
            $files = scandir(DR . '/files/');
            if (in_array($_GET['file_name'], $files)) {
                unlink($_SERVER["DOCUMENT_ROOT"] . '/files/' . $_GET['file_name']);
                self::$content .= '<font color="red">Файл "' . $_GET['file_name'] . '" был удален.</font><br>';
            }
        }
        self::main();
    }

    public static function upload() {
        $way_for_picture = '/files';
        //$site_url = 'http://' . $system['site_url'];
        $fileformat = '';
        $number = time();
        if ($_FILES['up_file']['type']) {
            $picture = 'yes';
        } else {
            $picture = 'no';
        }
        $arr_pic2 = explode(".", $_FILES['up_file']['name']);
        // Загрузка рисунка
        $hpoints = count($arr_pic2);
        $arr_pic[1] = mb_strtolower($arr_pic2[$hpoints - 1]);
        unset($arr_pic2[$hpoints - 1]);
        $arr_pic[0] = Helper::transliterate(mb_strtolower(implode('.', $arr_pic2)));
        $fileformat = $arr_pic[1];
        if (is_uploaded_file($_FILES['up_file']['tmp_name'])) {
            move_uploaded_file($_FILES['up_file']['tmp_name'], DR . "$way_for_picture/$arr_pic[0].$arr_pic[1]");
        } else {
            echo "Ошибка! Не могу загрузить!";
            exit;
        }
        self::main();
    }

    public static function delete() {
        if (isset($_REQUEST['file_name'])) {
            $file_name = $_REQUEST['file_name'];
            self::$content .= '<font color="red"><b>Вы уверены, что хотите стереть файл "' . $file_name . '"?</b></font><br><br>
                файл будет стерт без возможности восстановления!<br><br>
                <table border="0" width="300" cellspacing="10" cellpadding="0">
	<tr>
		<td>
                <form method="POST" action="?inc=uploader&action=really_delete&file_name=' . $file_name . '">
		<input type="submit" value="Удалить"></form>
                </td><td>
                <form method="POST" action="?inc=uploader">
		<input type="submit" value="Отмена"></form>
                </td>
	</tr>
	</table>';
        } else {
            self::main();
        }
    }

}