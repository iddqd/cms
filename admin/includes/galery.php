<?php

abstract class galery {

    public static $content, $title;
    protected static $_path, $_cfgName, $_width, $_height;

    public static function del() {
        if (isset($_GET['img'])) {
            @unlink(DR . static::$_path . $_GET['img'] . '.jpg');
            $images = ($confStr = file_get_contents(DR . static::$_path . static::$_cfgName)) ? explode(';', $confStr) : [];
            echo array_search($_GET['img'], $images);
            unset($images[array_search($_GET['img'], $images)]);
            file_put_contents(DR . static::$_path . static::$_cfgName, implode(';', $images));
        }
        header('Location: ?inc=' . $_GET['inc']);
        die;
    }

    public static function add() {
        if (isset($_FILES['image']['tmp_name']) && is_uploaded_file($_FILES['image']['tmp_name'])) {
            if (!exif_imagetype($_FILES['image']['tmp_name'])) {
                throw new Exception("Неверный тип файла!");
            }
            $fname = uniqid();
            if (!Resize::resizeToJPG($_FILES['image']['tmp_name'], DR . static::$_path . $fname . '.jpg', static::$_width, static::$_height))
                throw new Exception("Ошибка загрузки файла!");
            $images = ($confStr = file_get_contents(DR . static::$_path . static::$_cfgName)) ? explode(';', $confStr) : [];
            $images[] = $fname;
            file_put_contents(DR . static::$_path . static::$_cfgName, implode(';', $images));
        }
        header('Location: ?inc=' . $_GET['inc']);
        die;
    }

    public static function main() {
        $msg = static::$_msg;
        static::$content = <<<HTML
<style>
.myDragClass{
    background-color:#ccc;
    filter:progid:DXImageTransform.Microsoft.Alpha(opacity=50); /* IE 5.5+*/
    -moz-opacity: 0.5; /* Mozilla 1.6 и ниже */
    -khtml-opacity: 0.5; /* Konqueror 3.1, Safari 1.1 */
    opacity: 0.5; /* CSS3 - Mozilla 1.7b +, Firefox 0.9 +, Safari 1.2+, Opera 9+ */
}
#list .hand{width:88px;height:50px;background:url(/img/hand.png) center no-repeat}
#list td{padding:0px;margin:0px}
#list .confirm{margin-left:30px;}
</style>
<script type="text/javascript" src="/js/dnd_tbl.js"></script>
<script type="text/javascript">
$(document).ready(function() {
    // Initialise the table 3
    $("#list").tableDnD({
        onDragClass: "myDragClass",
        onDrop: function(table, row) {
            var rows = table.tBodies[0].rows;
            var w = "";
            for (var i = 0; i < rows.length; i++) {
                w += rows[i].id + ";";
            }
            $.ajax({
                type: "POST",
                url: "?inc={$_GET['inc']}&action=dragndrop",
                timeout: 5000,
                data: "w=" + w,
                error: function(data) {
                    alert('Error!')
                }
            });
        }
    });
});
</script>
<form method="post" action="?inc={$_GET['inc']}&action=add" enctype="multipart/form-data">
    Описание: <input type="text" name="descr"> | <input type="file" name="image">
    <input type="submit" value="Добавить фото">
</form>
<em><span style="font-size:smaller;">$msg</span><em><br><br>
<table id="list" style="margin-left:30px">
HTML;
        $images = ($confStr = file_get_contents(DR . static::$_path . static::$_cfgName)) ? explode(';', $confStr) : [];
        foreach ($images as $img) {
            static::$content.='<tr id="' . $img . '"><td class="hand"></td><td><img src="' . static::$_path . $img .
                    '.jpg" style="width:88px;height:50px"></td><td><input type="text" class="descr"></td><td><a href="?inc=' . $_GET['inc'] .
                    '&action=del&img=' . $img . '" class="button confirm" data-confirm="Удалить '
                    . $img . '.jpg ?">Удалить</a></td></tr>
    ';
        }
        static::$content.='</table>';
    }

    public static function dragndrop() {
        file_put_contents(DR . static::$_path . static::$_cfgName, rtrim($_POST['w'], ';'));
        die;
    }

}