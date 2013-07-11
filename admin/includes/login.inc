<?php

class login {

    public static function main() {
        $_SESSION['attempt'] = (isset($_SESSION['attempt'])) ? $_SESSION['attempt'] : 0;
        $action = (isset($_REQUEST['action'])) ? $_REQUEST['action'] : null;

        $content = '<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Вход в систему</title>
</head>
<style>
BODY { background-image: url("/admin/images/entfon.jpg"); background-repeat: repeat-x; background-position: bottom ; background-attachment: fixed;
}
</style>
<body>';

        if ($action == 'error') {
            $content .= '<p style="color:red">Неверное имя пользователя или пароль.</p>';
            unset($action);
        }

        if ($_SESSION['attempt'] >= 3) {
            $content = "Для восстановления пароля перейдите по ссылке внизу.";
            $message = "Зафиксирована попытка взлома админки. Время: " . date("d-m-Y H:i:s") . ". ip клиента: " . getenv("HTTP_X_FORWARDED_FOR") . ". Браузер: " . getenv("HTTP_USER_AGENT") . "  ";
            //mail('pw@itsite.ru', 'Попытка взлома админки ' . $_SERVER["SERVER_NAME"], $message);
        }
// Узнаем инфу про комп
        $user_ipbr = DB::real_escape_string($_SERVER["REMOTE_ADDR"] . ', прокси:' . $_SERVER["HTTP_X_FORWARDED_FOR"]);

        if (isset($_POST['login'], $_POST['password'])) {
            ++$_SESSION['attempt'];
            $login = DB::real_escape_string($_POST['login']);
            $password = md5($_POST['password']);
//Находим последнюю запись в таблице
            $last_log_sql = DB::query('SELECT id FROM log ORDER BY last_time DESC LIMIT 1');
            if ($last_log_sql->num_rows) {
                $last_log = $last_log_sql->fetch_array();
                $last_log_id = ($last_log['id']);
            } else {
                $last_log_id = 0;
            }
            if ($last_log_id >= 1000) {
                $insert_id = 1;
            } else {
                $insert_id = $last_log_id + 1;
            }
            $next_result = DB::query("SELECT * FROM log WHERE id=$insert_id");
            if ($next_result->num_rows) {       //если есть такая строка, то удаляем
                DB::query("DELETE FROM `log` WHERE id=$insert_id");
            }
            $query = "SELECT id,name,access FROM users WHERE login='$login' AND pass='$password'";
            $result = DB::query($query);
            $dimensions = $_REQUEST['desktop'] . ", ";
            $browser = $_SERVER['HTTP_USER_AGENT'];
            if ($result->num_rows != 1) {
                $insert_log = "INSERT INTO log SET id=$insert_id,  ip_info='" . $dimensions . $browser . "', ip='$user_ipbr', pass='"
                        . DB::real_escape_string($_POST['password']) . "', login='$login'";
                DB::query($insert_log);
                $content .= '<p style="color:red">Неверное имя пользователя или пароль.</p>';
                unset($action);
            } else {
                $row = $result->fetch_array();
                unset($_SESSION['attempt']);
                $user = $_SESSION['name'] = $row['name'];
                $id = $_SESSION['id'] = $row['id'];
                $_SESSION['access'] = $row['access'];
                $insert_log = "INSERT INTO log SET id=$insert_id,  ip_info='" . $dimensions . $browser . "', ip='$user_ipbr', login='$login', pass='****'";
                DB::query($insert_log);
                // Сохраним инфу в таблицу
                $query = "UPDATE users SET ip_info='$user_ipbr', ip='$user_ipbr',session='" . session_id() . "' WHERE id=$id";
                $result = DB::query($query);
                header("Location: /admin/");
                exit;
            }
        }
        echo $content .= '
	<table border="0" width="100%" cellspacing="0" cellpadding="0" height="100%">
		<tr>
			<td align="center">
<form method="POST" action="">
<input type="hidden" name="action" value="enter">
<input type="hidden" name="desktop" id="desktop">
<table border="0" width="100%" cellspacing="0" cellpadding="2">
	<tr>
		<td width="30%"></td>
		<td colspan="2">
		<h3>Вход</h3>
		</td>
		<td width="30%"></td>
	</tr>
	<tr>
		<td width="30%" bgcolor="#808080"></td>
		<td bgcolor="#808080" width="90" style="color:white;"><b>Логин</b></td>
		<td bgcolor="#808080"><input type="text" name="login" size="20"></td>
		<td width="30%"></td>
	</tr>
	<tr>
		<td width="30%"></td>
		<td bgcolor="#999999" width="90" style="color:white;"><b>Пароль</b></td>
		<td bgcolor="#999999"><input type="password" name="password" size="20"></td>
		<td width="30%" bgcolor="#999999"></td>
	</tr>
	<tr>
		<td width="30%"></td>
		<td></td>
		<td><input type="submit" value="Войти"></td>
		<td width="30%"></td>
	</tr>
</table>
	</form>

</td>
</tr>
<tr>
<td height="30">
<a href="/admin/?memberpass" style="color:red; text-decoration:none;">Забыли пароль?</a>
</td>
</tr>
</table>
<script language="javascript">
    document.getElementById(\'desktop\').value=(screen.width +\'x\'+screen.height);
</script>';
    }

    public static function out() {
        session_destroy();
        header("Location: /admin/");
        die;
    }

}