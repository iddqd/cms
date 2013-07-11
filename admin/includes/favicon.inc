<?php

class favicon {

    public static $content = '', $title = 'Иконка сайта';

    public static function main() {
        $favicon = DR . '/favicon.ico';
        $img = 'img';
        if (file_exists($favicon)) {
            self::$content .= '<p>У сайта уже есть favicon.ico</p>';
        } else {
            self::$content .= '<p>Сайт не имеет favicon.ico.</p><p>Вы можеете загрузить её здесь.</p>';
        }
        self::$content .= '<p>Загружаемый файл д.б. в формате gif или ico.</p>
	<form enctype="multipart/form-data" method="post">
		<input type="file" name="' . $img . '">
		<input type="submit" value="Загрузить">
	</form>';

        if (isset($_FILES[$img]['type'])) {
            $_name = array_pop(explode('.', $_FILES[$img]['name']));
            if ($_name == 'ico' || $_name == 'gif') {
                $file = $_FILES[$img]['tmp_name'];
                $size = getimagesize($file);
                $im1 = imagecreatetruecolor(16, 16);
                $im = imagecreatefromgif($file);
                imagecopyresampled($im1, $im, 0, 0, 0, 0, 16, 16, $size[0], $size[1]);
                imagegif($im1, $favicon);
                self::$content .= '<p><b>Файл загружен.</b></p>';
            } else {
                self::$content .= '<p><b>Загружаемый файл д.б. в формате gif или ico.</b></p>';
            }
        }
        self::$content;
    }

}