<?php

$admin_text = $design_text = '';
if ($_SESSION['access'] == 'admin' || $_SESSION['access'] == 'power_user') {
    $admin_text = '<a href="/admin/?inc=users">Пользователи</a>';
}
if ($_SESSION['access'] == 'admin') {
    $design_text = '<a href="/admin/?inc=pattern&action=start">Дизайн</a>';
}
$view_menu=(file_exists('includes/view_menu.inc'))? file_get_contents('includes/view_menu.inc'):'';
echo<<<TEXT
<html>
    <head>
        <title>{$inc::$title}</title>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <meta http-equiv="Content-Language" content="ru">
        <link rel="stylesheet" href="css/style.css" type="text/css">
        <link rel="SHORTCUT ICON" href="/favicon.ico">
        <script src="http://ajax.googleapis.com/ajax/libs/jquery/1.9.1/jquery.min.js" type="text/javascript"></script>
        <script type="text/javascript">
            $(document).ready(function(){
                $('.confirm').click(function(){
                    var text=$(this).data('confirm') || 'Подтвердите действие.';
                    if(confirm(text)) return true;
                    else return false;
                });
            });
        </script>
    </head>
    <body>
        <table class='w100' cellpadding='5' cellspacing='0'>
        <tbody>
            <tr><th colspan='2'>
			<img src='/admin/images/design/logo.png' align='left' />
			<table width='95%' class='tome'>
			<tr><td width='75%' style='color: cccccc;'>
			<a href='/' style='color: #cccccc; text-decoration: none;'>{$_SERVER['SERVER_NAME']}</a> | {$inc::$title}
			</td><td width='25%' align='right' style='color: cccccc;'>
			{$_SESSION['name']}
			<a href='/admin/?inc=login&action=out'>Выход</a>
			<td>
			</tr>
			</table>
			</th></tr>
            <tr>
                <td style='width: 180px;' valign='top'>
                    <div id='menu_left'>
                        <a href="/admin/">Главная</a>
                        <a href="/admin/?inc=tree">Древо разделов</a>
                        <a href="/admin/?inc=uploader">Загрузка в /files/</a>
                        <hr>
                        $view_menu
			<a href="/admin/?inc=opts">Настройка</a>
			<a href="/admin/?inc=favicon">Иконка сайта</a>
			$design_text
			$admin_text
                    </div>
                </td>
                <td valign='top'>
		<div id='context'>
                        {$inc::$content}
		</div>
                </td>
            </tr>
        </tbody>
        </table>
    </body>
</html>
TEXT;
