<?php

require 'galery.php';

class galerys extends galery {

    public static $title = 'Планировки';
    protected static $_path, $_cfgName, $_descrName, $_width = 140, $_height = 140,
            $_msg = 'Для изменения приоритета перетащите изображение,  схватив его за палец.';

    public static function add() {
        if (!file_exists(DR . self::$_path) or !is_dir(DR . self::$_path)) {
            mkdir(DR . self::$_path, 0757);
            mkdir(DR . self::$_path . 'thumbs/', 0757);
        }
        if (is_uploaded_file($_FILES['image']['tmp_name'])) {
            if (!exif_imagetype($_FILES['image']['tmp_name'])) {
                throw new Exception("Неверный тип файла!");
            }
            $fname = uniqid();
            if (move_uploaded_file($_FILES['image']['tmp_name'], DR . static::$_path . $fname . '.jpg')) {
                list($w_i, $h_i) = getimagesize(DR . static::$_path . $fname . '.jpg');
                if ($w_i > $h_i) {
                    static::$_height = round(static::$_height * $h_i / $w_i);
                } else {
                    static::$_width = round(static::$_height * $w_i / $h_i);
                }
                if (!Resize::resizeToJPG(DR . static::$_path . $fname . '.jpg', DR . static::$_path . 'thumbs/' . $fname . '.jpg', static::$_width, static::$_height))
                    throw new Exception("Ошибка загрузки миниатюры!");
                $images = ($confStr = file_get_contents(DR . static::$_path . static::$_cfgName)) ? explode(';', $confStr) : [];
                $images[] = $fname;
                $descr = ($descrStr = file_get_contents(DR . static::$_path . static::$_cfgName)) ? unserialize($descrStr) : [];
                $descr[$fname] = htmlspecialchars($_POST['descr']);
                file_put_contents(DR . static::$_path . static::$_cfgName, implode(';', $images));
                file_put_contents(DR . static::$_path . static::$_descrName, serialize($descr));
            } else {
                throw new Exception("Ошибка загрузки файла!");
            }
        }
        header('Location: ?inc=' . $_GET['inc']);
        die;
    }

}
