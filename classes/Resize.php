<?php

/**
 * Description of Resize
 *
 * @author LIME
 */
class Resize {

    private static $img_o;

    private static function create($file_input, $w_o, $h_o) {
        switch (exif_imagetype($file_input)) {
            case 1:
                $img = imagecreatefromgif($file_input);
                break;
            case 2:
                $img = imagecreatefromjpeg($file_input);
                break;
            case 3:
                $img = imagecreatefrompng($file_input);
                break;
        }
        list($w_i, $h_i) = getimagesize($file_input);
        self::$img_o = imagecreatetruecolor($w_o, $h_o);
        imagecopyresampled(self::$img_o, $img, 0, 0, 0, 0, $w_o, $h_o, $w_i, $h_i);
    }

    static function resizeToJPG($file_input, $file_output, $w_o, $h_o) {
        self::create($file_input, $w_o, $h_o);
        return imagejpeg(self::$img_o, $file_output, 100);
    }

    static function resizeToGIF($file_input, $file_output, $w_o, $h_o) {
        self::create($file_input, $w_o, $h_o);
        return imagegif(self::$img_o, $file_output);
    }

    static function resizeToPNG($file_input, $file_output, $w_o, $h_o) {
        self::create($file_input, $w_o, $h_o);
        return imagepng(self::$img_o, $file_output);
    }

}