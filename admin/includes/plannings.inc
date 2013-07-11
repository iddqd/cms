<?php

class apartment {

    public static $content, $title = 'Квартиры';

    static function main() {
        self::$content = '
<script src="/js/apartment.js" type="text/javascript"></script>
<link rel="stylesheet" href="css/apartment.css" type="text/css">
<div id="search"><input type="search" placeholder="№ квартиры"><br><input type="button" value="Быстрый переход"><br><hr><a href="#new" class="button" style="font-size:smaller">Добавить квартиру</a></div>
<br><br>
<div id="aprtmt">
<table>
    <thead>
            <tr>
                <th></th>
                <th>№</th>
                <th>Этаж</th>
                <th>Секция</th>
                <th>Комнат</th>
                <th>Пл. б/б</th>
                <th>Пл. с/б</th>
                <th>Цена р/м&sup2</th>
                <th>Ст-сть</th>
                <th>Статус</th>
                <th class="img">Планировка</th>
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
                <td class="data"><input type="text" name="lvl" value="' . $row['lvl'] . '" data-num="' . $row['id'] . '"><span>' . $row['lvl'] . '</span></td>
                <td class="data"><input type="text" name="section" value="' . $row['section'] . '" data-num="' . $row['id'] . '"><span>' . $row['section'] . '</span></td>
                <td class="data"><input type="text" name="rooms" value="' . $row['rooms'] . '" data-num="' . $row['id'] . '"><span>' . $row['rooms'] . '</span></td>
                <td class="data"><input type="text" name="square-nb" value="' . $row['square-nb'] . '" data-num="' . $row['id'] . '"><span>' . $row['square-nb'] . '</span></td>
                <td class="data"><input type="text" name="square-b" value="' . $row['square-b'] . '" data-num="' . $row['id'] . '"><span>' . $row['square-b'] . '</span></td>
                <td class="data"><input type="text" name="price-fm" value="' . $row['price-fm'] . '" data-num="' . $row['id'] . '"><span>' . $row['price-fm'] . '</span></td>
                <td class="data"><input type="text" name="price" value="' . $row['price'] . '" data-num="' . $row['id'] . '"><span>' . $row['price'] . '</span></td>
                <td class="data"><input type="text" name="status" value="' . $row['status'] . '" data-num="' . $row['id'] . '"><span>' . $row['status'] . '</span></td>
                <td class="img"><input type="file" name="img"><img src="/img/apartment/thumbs/' . $row['id'] . '.jpg" data-num="' . $row['id'] . '"></td>
            </tr>';
            $even = ($even) ? '' : ' even';
        }
        self::$content.='
    </tbody>       
   <form method="POST" id="apartment" enctype="multipart/form-data" action="?inc=apartment&action=add">
    <tbody>
            <tr id="new" class="new">
                <td></td>
                <td><input type="text" name="id"></td>
                <td><input type="text" name="lvl"></td>
                <td><input type="text" name="section"></td>
                <td><input type="text" name="rooms"></td>
                <td><input type="text" name="square-nb"></td>
                <td><input type="text" name="square-b"></td>
                <td><input type="text" name="price-fm"></td>
                <td><input type="text" name="price"></td>
                <td><input type="text" name="status"></td>
                <td class="img"><input type="file" name="img"></td>
            </tr>
            <tr class="new">
                <td colspan="10" style="text-align:left">Загрузка PDF <input type="file" name="pdf" style="width:auto"></td>
                <td class="img"><input type="submit" value="Добавить квартиру"></td>
    </tbody>
</table>
</form>
</div>
';
    }

    public static function add() {
        $ints = ['filter' => 257, 'flags' => FILTER_REQUIRE_SCALAR, 'options' => ['min_range' => 1]];
        $post = ['id' => $ints, 'lvl' => $ints, 'section' => 513, 'rooms' => $ints, 'square-nb' => $ints, 'square-b' => $ints, 'price-fm' => $ints, 'price' => $ints, 'status' => 513];
        if (isset($_FILES['img']) && is_uploaded_file($_FILES['img']['tmp_name']) && !in_array(FALSE, $post = filter_input_array(0, $post), 1)) {
            if (move_uploaded_file($_FILES['img']['tmp_name'], DR . '/img/apartment/' . $post['id'] . '.jpg')) {
                list($w_i, $h_i) = getimagesize(DR . '/img/apartment/' . $post['id'] . '.jpg');
                $height = $width = 140;
                if ($w_i > $h_i) {
                    $height = round(140 * $h_i / $w_i);
                } else {
                    $width = round(140 * $w_i / $h_i);
                }
                Resize::resizeToJPG(DR . '/img/apartment/' . $post['id'] . '.jpg', DR . '/img/apartment/thumbs/' . $post['id'] . '.jpg', $width, $height);
                if (is_uploaded_file($_FILES['pdf']['tmp_name']) && !move_uploaded_file($_FILES['pdf']['tmp_name'], DR . '/pdf/' . $post['id'] . '.pdf')) {
                    throw new Exception("Не удалось загрузить pdf!");
                }
                $query = 'INSERT INTO apartment VALUES(' . implode(',', array_map(function($p) {
                                            return is_int($p) ? $p : "'$p'";
                                        }, $post)) . ')';
                DB::query($query);
            } else {
                throw new Exception("Не удалось загрузить файл!");
            }
        }
        header('Location: ?inc=apartment#row-' . DB::insert_id());
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
            if (file_exists(DR . '/pdf/' . $id . '.pdf'))
                unlink(DR . '/pdf/' . $id . '.pdf');
            DB::query('DELETE FROM apartment WHERE id=' . $id);
            echo 'ok';
        }
        die;
    }

}