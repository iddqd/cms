<?php

class slider {

    private $html = '<div id="slider">';

    public function __construct() {
        $images = ($confStr = file_get_contents(DR . '/img/slider/slider.cfg')) ? explode(';', $confStr) : [];
        foreach ($images as $key => $img) {
            $this->html.='<img src="/img/slider/' . $img . '.jpg" id="img_' . $key . '">';
        }
        $this->html.='<div id="left_arrow" class="arrows"></div>
                                    <div id="right_arrow" class="arrows"></div>
                                </div>';
    }

    public function __toString() {
        return $this->html;
    }

}
