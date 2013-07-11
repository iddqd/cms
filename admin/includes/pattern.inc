<?php

class pattern {

    public static $content, $title = 'Шаблоны сайта';

    public static function main() {
        self::$content = '<a href="?inc=pattern&action=start" class="button">Назад...</a>
            <a href="?inc=pattern&action=add" class="button">Добавить дизайн</a><br><br>';
        $result = DB::query('SELECT * FROM patterns');
        $table_api = new table_api_for_module;
        $table_api->set_row_title(array('Номер', 'Описание', 'Количество использования', 'Удалить'));
        while ($row = $result->fetch_array()) {
            $count = DB::query("SELECT COUNT(*) as kol FROM sections WHERE pattern_id={$row['id']}")->fetch_array()[0];
            $add_row = array(
                $row['id'],
                '<a href="?inc=pattern&action=edit&id=' . $row['id'] . '">' . $row['pattern_desc'] . '</a>',
                $count . ' <a href="?inc=pattern&action=counter&id=' . $row['id'] . '&pattern_desc=' . urlencode($row['pattern_desc']) . '">(посмотреть)</a>'
            );
            if (!$row['pattern_text'] || !$count) {
                $add_row[] = '<a href="?inc=pattern&action=del&id=' . $row['id'] . '" class="confirm"> Удалить. </a>';
            } else {
                $add_row[] = '';
            }
            $table_api->set_row($add_row);
        }
        self::$content .= $table_api->print_table();
    }

    public static function start() {
        self::$content = '<p><a href="/admin/?inc=pattern">Шаблоны страниц</a></p>
<p><a href="/admin/?inc=css">Изменение файлов css</a></p>';
    }

    public static function counter() {
        if (isset($_GET['id']))
            $id = abs((int) $_GET['id']);
        if (isset($_GET['pattern_desc']))
            $pattern_desc = $_GET['pattern_desc'];
        self::$content = '<a href="/admin/?inc=pattern" class="button">Назад...</a><br><h3>Список страниц, которые используют дизайн "' . $pattern_desc . '"<br>';
        $result = DB::query("SELECT id,rightk,leftk,name FROM sections WHERE pattern_id=$id ORDER by leftk");
        while ($row = $result->fetch_array()) {
            $url = [];
            $result2 = DB::query('SELECT directory FROM sections WHERE leftk <= ' . $row['leftk'] .
                            ' AND rightk >= ' . $row['rightk'] . ' AND depth>0 ORDER BY depth');
            while ($row2 = $result2->fetch_array()) {
                $url[] = $row2['directory'];
            }
            self::$content.='<a href="?inc=tree&action=edit&id=' . $row['id'] . '"><img border="0" src="/admin/images/edit.gif"></a>
                 <a href="/' . implode('/', $url) . '" target="_blank">' . $row['name'] . '</a><br>';
        }
        self::$content.="<br>";
    }

    public static function save_add() {
        if (isset($_POST['pattern_name']))
            $pattern_name = DB::real_escape_string($_POST['pattern_name']); else {
            echo 'Нет номера!';
            exit;
        }
        if (isset($_POST['pattern_text']))
            $pattern_text = DB::real_escape_string($_POST['pattern_text']); else {
            echo 'Нет номера!';
            exit;
        }
        if (isset($_POST['pattern_desc']))
            $pattern_desc = DB::real_escape_string($_POST['pattern_desc']); else {
            echo 'Нет номера!';
            exit;
        }
        DB::query("INSERT INTO patterns SET pattern_name='$pattern_name', pattern_text='$pattern_text', pattern_part='$pattern_part', pattern_desc='$pattern_desc'");
        header('Location: /admin/?inc=pattern');
    }

    public static function save_edit() {
        if (isset($_GET['id']))
            $id = abs((int) $_GET['id']); else {
            echo 'Нет номера!';
            exit;
        }
        if (isset($_POST['pattern_name']))
            $pattern_name = DB::real_escape_string($_POST['pattern_name']); else {
            echo 'Нет номера!';
            exit;
        }
        if (isset($_POST['pattern_text']))
            $pattern_text = DB::real_escape_string($_POST['pattern_text']); else {
            echo 'Нет номера!';
            exit;
        }
        if (isset($_POST['pattern_desc']))
            $pattern_desc = DB::real_escape_string($_POST['pattern_desc']); else {
            echo 'Нет номера!';
            exit;
        }
        $pattern_old = DB::real_escape_string(DB::query('SELECT pattern_text FROM patterns WHERE id=' . $id)->fetch_array()[0]);
        $row = DB::query('SELECT COUNT(*),MIN(time) FROM pattern_history WHERE pattern_id=' . $id)->fetch_array();
        if ($row[0] < Options::$option['history_cnt']) {
            DB::query("INSERT INTO pattern_history VALUES(null,null,'$pattern_old',$id)");
        } else {
            DB::query("UPDATE pattern_history SET text='$pattern_old' WHERE time='{$row[1]}'");
        }
        DB::query("UPDATE patterns SET pattern_name='$pattern_name', pattern_text='$pattern_text', pattern_part='$pattern_part', pattern_desc='$pattern_desc' WHERE id=$id");
        header('Location: /admin/?inc=pattern');
        die;
    }

    public static function del() {
        if (isset($_GET['id'])) {
            $id = abs((int) $_GET['id']);
            DB::query("DELETE FROM patterns WHERE id=$id");
            DB::query("DELETE FROM pattern_history WHERE pattern_id=$id");
            header('Location: /admin/?inc=pattern');
            die;
        } else {
            echo 'Нет номера!';
            die;
        }
    }

    public static function add() {
        self::$content = '<a href="/admin/?inc=pattern" class="button">Назад...</a> <br>
	<form method="POST" action="?inc=pattern&action=save_add">
	<table border="0" width="100%" cellspacing="4" cellpadding="0">
		<tr>
			<td align="left" valign="top"><b>Название </b> </td>
			<td><input type="text" name="pattern_name"/></td>
		</tr>
		<tr>
			<td align="left" valign="top"><b>Шаблон</b><p>&nbsp;</td>
			<td><textarea rows="25" name="pattern_text" cols="125"></textarea></td>
		</tr>
		<tr>
			<td align="left" valign="top"><b>Описание</b></td>
			<td><textarea rows="5" name="pattern_desc" cols="75"></textarea></td>
		</tr>
	</table>
	<p><input type="submit" value="Сохранить"></p>
	</form>';
    }

    public static function edit() {
        if (isset($_GET['id']))
            $id = filter_input(INPUT_GET, 'id', FILTER_VALIDATE_INT, 1); else {
            echo 'Нет номера!';
            die;
        }
        $row = DB::query("SELECT * FROM patterns WHERE id=$id")->fetch_array();
        $row['pattern_text'] = stripslashes($row['pattern_text']);
        $row['pattern_desc'] = htmlspecialchars(stripslashes($row['pattern_desc']));
        self::$content .= <<<FORM
                <a href="/admin/?inc=pattern" class="button">Назад...</a>
            <a href="/admin/?inc=pattern_history&id=$id" class="button">История</a><br>
	<form method="POST" action="?inc=pattern&action=save_edit&id=$id">
	<table border="0" width="100%" cellspacing="4" cellpadding="0">
		<tr>
			<td align="left" valign="top"><b>Название </b> </td>
			<td><input type="text" name="pattern_name" value="{$row['pattern_name']}" /></td>
		</tr>
		<tr>
			<td align="left" valign="top"><b>Шаблон</b><p>&nbsp;</td>
			<td><textarea rows="25" name="pattern_text" cols="125">{$row['pattern_text']}</textarea></td>
		</tr>
		<tr>
			<td align="left" valign="top"><b>Описание</b></td>
			<td><textarea rows="5" name="pattern_desc" cols="75">{$row['pattern_desc']}</textarea></td>
		</tr>
	</table>	
	<p><input type="submit" value="Сохранить"></p>
	</form>
FORM;
    }

}