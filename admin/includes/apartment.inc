<?php

class apartment {

    public static $content, $title = 'Квартиры';

    static function main() {
        self::$content = '
<script src="/js/apartment.js" type="text/javascript"></script>
<link rel="stylesheet" href="css/apartment.css" type="text/css">
<div id="search"><input type="search" placeholder="№ квартиры"><br><input type="button" value="Быстрый переход"></div>
<br><br>
<div id="aprtmt">
<table>
    <thead>
            <tr>
                <th></th>
                <th>№</th>
                <th>Этаж</th>
                <th>Секция</th>
                <th>Номер на пл-ке</th>
                <th>Комнат</th>
                <th>Пл. б/б</th>
                <th>Пл. с/б</th>
                <th>Цена р/м&sup2</th>
                <th>Ст-сть</th>
                <th>Статус</th>
            </tr>
    <thead>
    <tbody>';
        $result = DB::query('SELECT * FROM apartment ORDER BY id');
        $even = '';
        while ($row = $result->fetch_array()) {
            self::$content.='
            <tr class="all' . $even . '" id="row-' . $row['id'] . '">
                <td class="confirm delete" data-confirm="Удалить квартиру № ' . $row['id'] . '" data-num="' . $row['id'] . '"><img src="/img/delete.png" style="width:12px;height:12px"></td>
                <td>' . $row['id'] . '</td>
                <td>' . $row['lvl'] . '</td>
                <td>' . $row['section'] . '</td>
                <td>' . $row['number'] . '</td>
                <td>' . $row['rooms'] . '</td>
                <td>' . $row['square-nb'] . '</td>
                <td>' . $row['square-b'] . '</td>
                <td class="data"><input type="text" name="price-fm" size="4" value="' . $row['price-fm'] . '" data-num="' . $row['id'] . '"><span>' . $row['price-fm'] . '</span></td>
                <td class="data"><input type="text" name="price" size="4" value="' . $row['price'] . '" data-num="' . $row['id'] . '"><span>' . $row['price'] . '</span></td>
                <td class="data" style="width:20%"><input type="text" name="status" value="' . $row['status'] . '" data-num="' . $row['id'] . '"><span>' . $row['status'] . '</span></td>
            </tr>';
            $even = ($even) ? '' : ' even';
        }
        self::$content.='
    </tbody>       
</table>
</div>
';
    }

    public static function edit() {
        $post = ['id' => ['filter' => 257, 'flags' => FILTER_REQUIRE_SCALAR, 'options' => ['min_range' => 1]], 'col' => 513, 'val' => 513];
        if (!in_array(FALSE, $post = filter_input_array(0, $post), 1)) {
            $post['val'] = (is_int($post['val']))? : '"' . $post['val'] . '"';
            $result = DB::query('UPDATE apartment SET `' . $post['col'] . '`=' . $post['val'] . ' WHERE id=' . $post['id']);
            echo 'ok';
        }
        die;
    }

    public static function delete() {
        if (isset($_POST['id']) and $id = abs((int) $_POST['id'])) {
            unlink(DR . '/img/apartment/' . $id . '.jpg');
            unlink(DR . '/img/apartment/thumbs/' . $id . '.jpg');
            DB::query('DELETE FROM apartment WHERE id=' . $id);
            echo 'ok';
        }
        die;
    }

}