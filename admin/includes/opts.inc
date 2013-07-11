<?php

class opts {

    public static $content = '', $title = 'Настройки системы';

    public static function main() {
        self::$content .= '
    <p>
        <big><a href="?inc=modules">Модули</a></big><br/>
        <small>Список зарегистрированных модулей.<br/> Регистрация модулей.</small>
    </p>
    <a href="?inc=opts&action=options">Настройки системы</a><br><small>Данные параметры будут применены вместо незаполненных полей</small><br><br>
    <a href="?inc=robots">robots.txt</a><br/><small>Изменение файла robots.txt</small><br><br>
    <a href="?inc=ftp">.ftpaccess</a><br/>
    <small>Изменение файла .ftpaccess</small><br><br>
    <a href="/admin/?inc=log">Лог доступа</a><br/>
    <small>Список вхождений в админку и журнал операций</small><br><br>';
    }

    public static function save() {
        foreach ($_POST as $option => $val) {
            $ids = array();
            $result = DB::query('SELECT DISTINCT pattern_id FROM pattern_history');
            while ($pattern_id = $result->fetch_array()[0]) {
                $res = DB::query('SELECT id FROM pattern_history WHERE pattern_id=' . $pattern_id .
                                ' ORDER BY time DESC LIMIT ' . $_POST['history_cnt']);
                while ($row = $res->fetch_array()[0]) {
                    $ids[] = $row;
                }
            }
            if ($ids = implode(',', $ids))
                DB::query('DELETE FROM pattern_history WHERE id NOT IN (' . $ids . ')');
            Options::$option[$option] = str_replace('"', '&quot;', html_entity_decode($val));
            file_put_contents(DR . '/options.cfg', serialize(Options::$option));
        }
        header('Location: /admin/?inc=options');
    }

    public static function options() {
        $bgcolor = '#FBFBFB';
        if (Options::$option['easy_admin_mode']) {
            $first_select = ' selected="selected"';
            $second_select = '';
        } else {
            $second_select = ' selected="selected"';
            $first_select = '';
        }
        self::$content.= '<b>Настройки по умолчанию</b><br><br>
                <table border="0" cellpadding="5" cellspacing="0" width="100%">
                <form method="POST" action="/admin/?inc=opts&action=save">
<tr bgcolor="#FBFBFB">
    <td align="left" valign="top">стандартный title
        <br>
        &lt;!--{page_name}--&gt; будет заменен на название страницы. <font color="silver"> (title)</font>
    </td>
    <td align="left" valign="top">
        <input type="text" name="title" size="80" value="' . Options::$option['title'] . '">
    </td>
</tr>
<tr bgcolor="#F3F3F3">
    <td align="left" valign="top">стандартный description
        <br>
        &lt;!--{page_name}--&gt; будет заменен на название страницы. <font color="silver"> (description)</font>
    </td>
    <td align="left" valign="top">
        <input type="text" name="description" size="80" value="' . Options::$option['description'] . '">
    </td>
</tr>
<tr bgcolor="#FBFBFB">
    <td align="left" valign="top">Что выводить на страницу с пустым контентом? <font color="silver"> (content)</font>
    </td>
    <td align="left" valign="top">
        <textarea name="content" rows="8" cols="52">' . Options::$option['content'] . '</textarea>
    </td>
</tr>
<tr bgcolor="#F3F3F3">
    <td align="left" valign="top">Упрощенный режим администрирования сайта
    </td>
    <td>
        <select size="1" name="easy_admin_mode">
            <option value="1"' . $first_select . '>да</option>
            <option value="0"' . $second_select . '>нет</option>
        </select>
    </td>
</td>
</tr>
<tr bgcolor="#FBFBFB">
    <td align="left" valign="top">Количество изменений шаблонов в истории.<br>
        При уменьшении более старые копии будут удалены.
    </td>
    <td align="left" valign="top">
        <input type="number" name="history_cnt" size="2" value="' . Options::$option['history_cnt'] . '">
    </td>
</tr>
<!--tr bgcolor="#F3F3F3">
    <td align="left" valign="top">Промежуток времени для сохранения истории в минутах
    </td>
    <td align="left" valign="top">
        <input type="number" name="history_t" size="2" value="' . Options::$option['history_t'] . '">
    </td>
</tr-->
</table>
<input type="submit" value="Сохранить"><br>
</form>';
    }

}