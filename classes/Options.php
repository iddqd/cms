<?

class Options {

    public static $option;

    public function __construct() {
        self::$option = unserialize(file_get_contents($_SERVER['DOCUMENT_ROOT'] . '/options.cfg'));
    }

}
new Options;
