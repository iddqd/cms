<?php

class Users {

    public static $content, $title = 'Пользователи';

    public static function checkRole() {
        if ($_SESSION['access'] == 'user') {
            self::$content = '<span style="color:red">У вас нет прав на просмотр данной страницы!</span>';
            return FALSE;
        }
        return TRUE;
    }

    public static function main() {
        if (static::checkRole()) {
            self::$content = '<a href="/admin/?inc=users&action=add" class="button">Добавить пользователя</a> <br /><br />
            <p/><table border="0" cellspacing="0" cellpadding="5">
	<tr bgcolor=silver>
            <th>Логин</th><th>E-mail</th><th>Имя</th><th>Доступ</th></tr>';
            $query = "SELECT id,login,email,name,access FROM users ORDER BY access";
            $result = DB::query($query);
            while ($row = $result->fetch_array()) {
                self::$content .= '<tr><td><a href="/admin/?inc=users&action=edit&id=' . $row['id'] . '" title="Сменить пароль">' . $row['login'] . '</a></td><td>' .
                        $row['email'] . ' </td><td>' . $row['name'] . '</td><td>' . $row['access'] . '</td></tr>';
            }
            self::$content .= '</table>';
            self::$content = self::$content;
        }
    }

    public static function delete() {
        if (static::checkRole()) {
            if ($id = filter_input(INPUT_GET, 'id', FILTER_VALIDATE_INT, 1)) {
                $query = "DELETE FROM users WHERE id=$id";
                $result = DB::query($query);
                header('Location: /admin/?inc=users');
                die;
            }
        }
    }

    public static function edit() {
        if (static::checkRole()) {
            if ($id = filter_input(INPUT_GET, 'id', FILTER_VALIDATE_INT, 1)) {
                $row = DB::query('SELECT access,login,email,name FROM users WHERE id=' . $id)->fetch_array();
                if ($_SESSION['access'] == 'admin') {
                    $access_arr = array('admin', 'power user', 'user');
                    $accss = '';
                    foreach ($access_arr as $val) {
                        $select = ($row['access'] == $val) ? ' selected="selected"' : '';
                        $accss.='<option value="' . $val . '"' . $select . '>' . $val . '</option>';
                    }
                    $selectaccess = '<tr>
			<td width="200">Доступ</td>
			<td><select name="access">
                            ' . $accss . '
                            </select>
                        </td>
		</tr>';
                } else {
                    $selectaccess = '';
                }
                self::$content = <<<FORM
<a href="/admin/?inc=users" class="button">Назад к списку</a>
<a href="/admin/?inc=users&action=delete&id=$id" class="button confirm">Удалить</a><br><br>
<form method="POST" action="/admin/?inc=users&action=saveedit">
<input type="hidden" name="id" value="$id">
	<table border="0" width="100%" cellpadding="0">
                <tr>
			<td width="200">Логин</td>
			<td><input type="text" name="login" size="55" value="{$row['login']}"></td>
		</tr>
		<tr>
			<td width="200">Пароль</td>
			<td><input type="password" name="pass" size="55"></td>
		</tr>
		<tr>
			<td width="200">E-mail</td>
			<td><input type="email" name="email" size="55" value="{$row['email']}"></td>
		</tr>
		<tr>
			<td width="200">Имя</td>
			<td><input type="text" name="name" size="55" value="{$row['name']}"></td>
		</tr>		
		$selectaccess		
	</table>
	<p><input type="submit" value="Отправить"></p>
</form>
FORM;
            }
        }
    }

    public static function add() {
        if (static::checkRole()) {
            self::$content = '<a href="/admin/?inc=users" class="button">Назад к списку</a> <br /><br />';
            if ($_SESSION['access'] == 'admin') {
                $selectaccess = '<tr>
			<td width="200">Доступ</td>
			<td><select size="1" name="access">
                        <option value="admin">администратор</option>
                    <option selected value="power user">опытный пользователь</option>
                    <option selected value="user">пользователь</option>
                    </select></td></tr>';
            } else {
                $selectaccess = '';
            }
            self::$content .= <<<FORM
<form method="POST" action="?inc=users&action=save">
	<table border="0" width="100%" cellpadding="0">
		<tr>
			<td width="200">Логин</td>
			<td><input type="text" name="login" size="55" value=""></td>
		</tr>
		<tr>
			<td width="200">Пароль</td>
			<td><input type="password" name="pass" size="55" value=""></td>
		</tr>
		<tr>
			<td width="200">E-mail</td>
			<td><input type="text" name="email" size="55" value=""></td>
		</tr>
		<tr>
			<td width="200">Имя</td>
			<td><input type="text" name="name" size="55" value=""></td>
		</tr>
		$selectaccess		
	</table>
	<p><input type="submit" value="Отправить"></p>
</form>
FORM;
            self::$content = self::$content;
        }
    }

    public static function save() {
        if (static::checkRole()) {
            if (isset($_POST['login']))
                $login = DB::real_escape_string($_POST['login']); else {
                echo 'Нет login!';
                exit;
            }
            if (isset($_POST['pass']))
                $pass = md5($_POST['pass']); else {
                echo 'Нет pass!';
                exit;
            };
            if (isset($_POST['email']))
                $email = DB::real_escape_string($_POST['email']); else {
                echo 'Нет email!';
                exit;
            };
            if (isset($_POST['name']))
                $name = DB::real_escape_string($_POST['name']); else {
                echo 'Нет name!';
                exit;
            };
            if (isset($_POST['access']) && $_SESSION['access'] == 'admin')
                $access = DB::real_escape_string($_POST['access']);
            else
                $access = 'user';
            if (DB::query("SELECT 1 FROM users WHERE login='$login'")->num_rows) {
                self::$content .= '<b><font color="red">Пользователь с таким логином уже существует.</font></b><p/>';
            } elseif ($email && DB::query("SELECT 1 FROM users WHERE email='$email'")->num_rows) {
                self::$content .= '<b><font color="red">Пользователь с таким email уже существует.</font></b><p/>';
            } else {
                $query = "INSERT INTO users SET login='$login', pass='$pass', email='$email', name='$name',
                access='$access'";
                DB::query($query);
                header('Location: /admin/?inc=users');
                die;
            }
        }
    }

    public static function saveedit() {
        if (static::checkRole()) {
            if ($id = filter_input(INPUT_POST, 'id', FILTER_VALIDATE_INT, 1)) {
                if (isset($_POST['login']))
                    $login = DB::real_escape_string($_POST['login']); else {
                    echo 'Нет login!';
                    exit;
                }
                if (isset($_POST['pass']) && !empty($_POST['pass']))
                    $pass = " pass='" . md5($_POST['pass']) . "',"; else {
                    $pass = '';
                };
                if (isset($_POST['email']))
                    $email = DB::real_escape_string($_POST['email']); else {
                    echo 'Нет email!';
                    exit;
                };
                if (isset($_POST['name']))
                    $name = DB::real_escape_string($_POST['name']); else {
                    echo 'Нет name!';
                    exit;
                };
                if (isset($_POST['access']) && $_SESSION['access'] == 'admin')
                    $access = DB::real_escape_string($_POST['access']);
                else
                    $access = 'user';
                $query = "UPDATE users SET$pass email='$email', name='$name', access='$access' WHERE id=$id";
                DB::query($query);
                header('Location: /admin/?inc=users');
                die;
            }
        }
    }

}