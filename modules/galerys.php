<?php

class galerys {

    private $html = '
<link href="/css/galery.css" rel="stylesheet">
<link href="/css/lightbox.css" rel="stylesheet">
<script src="/js/lightbox.js"></script>
';

    public function __construct($attr) {
        $images = ($confStr = file_get_contents(DR . '/img/' . $attr . '/' . $attr . '.cfg')) ? explode(';', $confStr) : [];
        $descr = ($descrStr = file_get_contents(DR . '/img/' . $attr . '/' . $attr . '.descr')) ? unserialize($descrStr) : [];
        foreach ($images as $img) {
            $d = (isset($descr[$img])) ? '<div class="descr">' . $descr[$img] . '</div>' : '';
            $this->html.='<div class="interior">' . $d . '
                <a href="/img/' . $attr . '/' . $img .
                    '.jpg" rel="lightbox[' . $attr . ']"><img src="/img/' . $attr . '/thumbs/' . $img . '.jpg"></a></div>' . PHP_EOL;
        }
    }

    public function __toString() {
        return $this->html;
    }

}

