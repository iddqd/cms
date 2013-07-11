<?php

class pattern_history {

    public static $title = 'История изменений шаблона', $content = '';

    private static function _dateConvert($str) {
        list($year, $time) = explode(' ', $str);
        $monthArr = array(1 => 'Янв', 'Фев', 'Мар', 'Апр', 'Мая', 'Июн', 'Июл',
            'Авг', 'Сен', 'Окт', 'Ноя', 'Дек');
        list($year, $month, $day) = explode('-', $year);
        return $day . ' ' . $monthArr[(int) $month] . ' ' . $year . ' ' . $time;
    }

    public static function main() {
        if ($id = filter_input(INPUT_GET, 'id', FILTER_VALIDATE_INT, 1)) {
            self::$content.='<style type="text/css">
.reestablish{
    display:none;
    background-color:fff;
    white-space:pre-wrap
}
.reestablish_form{
    display:inline;
}
.show_text{text-align:center}
</style>
<script type="text/javascript">
$(document).ready(function(){
    $(\'.show_text\').click(function(){
        if($(this).text()==\'Просмотр\'){
            $(this).css(\'width\',$(this).css(\'width\')).text(\'Скрыть\');
        }else{
            $(this).text(\'Просмотр\');
        }
        $(\'#r\'+$(this).attr(\'data\')).slideToggle(200);
        return false;
    });
    $(\'.reestablish_form\').change(function(){
        if($(\'select option:selected\',this).val()){
            $(this).submit();
        }else{
            return false;
        }
    });
});
</script>
<a href="?inc=pattern&action=edit&id=' . $id . '" class="button">Назад...</a>';
            $result = DB::query('SELECT pattern_history.id,time,pattern_desc,text FROM pattern_history JOIN patterns ON pattern_id=patterns.id WHERE patterns.id=' . $id.' ORDER BY time DESC');
            if ($result->num_rows) {   
                $content = '';
                while ($row = $result->fetch_array()) {
                    $name=$row['pattern_desc'];
                    $options = '<option value="">Просмотр на страницах:</option>';
                    $result2 = DB::query('SELECT name,leftk,rightk,depth FROM sections WHERE pattern_id=' . $id);
                    while ($row2 = $result2->fetch_array()) {
                        $path = array();
                        $result3 = DB::query('SELECT directory FROM sections WHERE leftk<=' . $row2['leftk'] . ' AND rightk>=' . $row2['rightk'] . ' ORDER BY leftk');
                        while ($row3 = $result3->fetch_array()[0]) {
                            $path[] = $row3;
                        }
                        unset($path[0]);
                        $path = implode('/', $path);
                        $options.='<option value="/' . $path . '">' . str_repeat('&nbsp;&nbsp;&nbsp;&nbsp', $row2['depth']) . $row2['name'] . '</option>';
                    }
                    $content.='<li><a href="/admin/?inc=pattern_history&action=reestablish&id=' . $row['id'] .
                            '" class="button confirm">Восстановить</a><a href="#" class="button show_text" data="' . $row['id'] . '">Просмотр</a>
                        <form action="/admin/?inc=pattern_history&action=show_page&id=' . $row['id'] . '" class="reestablish_form" method="POST" target="_blank">
                            <select name="path">
                               ' . $options . '
                            </select>
                        </form>' . self::_dateConvert($row['time']) . '<div class="reestablish" id="r' . $row['id'] . '"><pre>' .
                            htmlspecialchars(stripslashes($row['text'])) . '</pre></div></li>';
                }
                self::$content.='<h3 style="margin-left:30px">История изменений шаблона "' . $name .
                        '".</h3><ol type="1">' . $content . '</ol>';
            }else{
                self::$content.='<h3 style="margin-left:30px">В истории не сохранено изменений.</h3>';
            }
        }
    }

    public static function reestablish() {
        if ($id = filter_input(INPUT_GET, 'id', FILTER_VALIDATE_INT, 1)) {
            list($text, $id) = DB::query('SELECT text,pattern_id FROM pattern_history WHERE id=' . $id)->fetch_row();
            DB::query('UPDATE patterns SET pattern_text="' . DB::real_escape_string($text) . '" WHERE id=' . $id);
            header('Location: /admin/?inc=pattern');
        }
    }

    public static function show_page() {
        if ($id = filter_input(INPUT_GET, 'id', FILTER_VALIDATE_INT, 1)) {
            echo $_SERVER['REQUEST_URI'] = $_POST['path'];
            Page::show($id);
            die;
        }
    }

}