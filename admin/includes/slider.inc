<?php

require 'galery.php';

class slider extends galery {

    public static $title = 'Слайдер';
    protected static $_path = '/img/slider/', $_cfgName = 'slider.cfg', $_width = 622, $_height = 516,
        $_msg='Размер изображения должен быть 622x516 пикселей в формате jpeg, jpg. Для сохранения верных пропорций.<br>Для изменения приоритета перетащите изображение, схватив его за палец.';

}

