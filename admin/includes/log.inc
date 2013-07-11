<?php

class log {

    public static $content, $title = 'Лог доступа';

    public static function main() {
        if (isset($_POST['truncate'])) {
            DB::query('TRUNCATE TABLE log');
        }
        $page = (isset($_REQUEST['page'])) ? $_REQUEST['page'] : 1;
        $limit = 50;
// Сколько всего страниц
        $pages_list = '';
        $all_pages = ceil(DB::query('SELECT COUNT(*) FROM log')->fetch_array()[0] / $limit);
        for ($count = 1; $count <= $all_pages; $count++) {
            if ($page != $count)
                $pages_list .= '<a href="?page=' . $count . '">' . $count . '</a> ';
            else
                $pages_list .= $count . ' ';
        }
        $start = ($page - 1) * $limit;
        self::$content = '<table><tr><th>Время</th><th>Логин</th><th>Пароль</th><th>IP</th><th>Экран и браузер</th></tr>';
        $result = DB::query("SELECT * FROM log ORDER BY last_time DESC LIMIT $start, $limit");
        while ($log = $result->fetch_array()) {
            if ($log['pass'] != '****')
                $color = "style='color:red'"; else
                $color = "";
            self::$content.="<tr $color><td>" . $log['last_time'] . "</td><td><a href='?inc=users&action=edit&id=" .
                    $log['id'] . "'>" . $log['login'] . "</a></td><td>" . $log['pass'] . "</td><td>" .
                    $log['ip'] . "</td><td>" . $log['ip_info'] . "</td></tr>";
        }
        self::$content.='</table><form method="post" action="/admin/?inc=log">Очистить логи: да <input type="checkbox" name="truncate"><input type="submit" value="Ok">
                    </form>';
    }

}