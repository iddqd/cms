<?

class Page {

    private static $_pattern;
    public static $title, $description, $keywords, $url, $content,
            $id, $pattern_id, $parent, $name, $depth, $visible, $directory, $get, $leftk, $rightk, $path;

    public function __construct() {
        $hosts = parse_ini_file(__DIR__ . '/host_cntr');
        if (!array_key_exists($_SERVER['HTTP_HOST'], $hosts)) {
            $hosts[$_SERVER['HTTP_HOST']] = 1;
        } else {
            ++$hosts[$_SERVER['HTTP_HOST']];
        }
        $str = '';
        foreach ($hosts as $key => $value) {
            $str .= $key . ' = ' . $value . PHP_EOL;
        }
        file_put_contents(__DIR__ . '/host_cntr', $str, LOCK_EX);
        $uri = trim($_SERVER['REQUEST_URI'], '/');
        if (!$uri) {
            $row = DB::query('SELECT * FROM sections WHERE id=1')->fetch_array();
        } else {
            $row['id'] = 1;
            foreach (explode('/', $uri) as $val) {
                $query = "SELECT * FROM sections WHERE directory='" . DB::real_escape_string($val) . "' AND parent={$row['id']}";
                $result = DB::query($query);
                if (!$result->num_rows)
                    break;
                $row = $result->fetch_array();
            }
            if (!isset($row['leftk'])) {
                $row = DB::query('SELECT * FROM sections WHERE id=1')->fetch_array();
            }
        }
        if (self::$url = $row['url']) {
            header('Location: ' . Page::$url);
        }
        $uri_arr = explode('/', $uri);
        self::$content = (empty($row['content'])) ? Options::$option['content'] : trim(stripslashes($row['content']));
        self::$title = (empty($row['title'])) ? Options::$option['title'] : $row['title'];
        self::$description = (empty($row['description'])) ? Options::$option['description'] : $row['description'];
        self::$keywords = (empty($row['keywords'])) ? Options::$option['keywords'] : $row['keywords'];
        self::$id = $row['id'];
        self::$pattern_id = $row['pattern_id'];
        self::$parent = $row['parent'];
        self::$name = $row['name'];
        self::$depth = $row['depth'];
        self::$visible = $row['visible'];
        self::$directory = $row['directory'];
        self::$leftk = $row['leftk'];
        self::$rightk = $row['rightk'];
        self::$get = array_slice($uri_arr, self::$depth);
        self::$path = '/' . implode('/', array_slice($uri_arr, 0, self::$depth)) . '/';
    }

    private static function getPattern() {
        self::$_pattern = stripcslashes(DB::query('SELECT pattern_text FROM patterns WHERE id=' . self::$pattern_id)->fetch_array()[0]);
        self::replaceTags();
    }

    private static function replaceTags() {
        self::$content = str_replace(array('<!--{page_content}-->', '<!--{title}-->', '<!--{description}-->', '<!--{keywords}-->', '<!--{page_name}-->'), array('<div id="content">' . Page::$content . '</div>', Page::$title, Page::$description, Page::$keywords, Page::$name), self::$_pattern);
        self::replaceModules();
    }

    private static function replaceModules() {
        preg_match_all('#<!--{(.+)(?:\[(.+)\])?}-->#sU', self::$content, $modules);
        $modules[1] = array_intersect_key($modules[1], array_unique($modules[0])); //чтоб не вызывать модуль повторно при повторном макросе
        foreach ($modules[1] as $key => $val) {
            if (file_exists(DR . '/modules/' . $val . '.php')) {
                include_once DR . '/modules/' . $val . '.php';
                self::$content = str_replace($modules[0][$key], new $val($modules[2][$key]), self::$content);
            }
        }
    }

    public static function show() {
        self::getPattern();
        if (Options::$option['easy_admin_mode']) {
            if (isset($_SESSION['id']))
                self::$content.= '
<script type="text/javascript">
    window.onload=function(){
        document.ondblclick=function(){
            location.href="/admin/?inc=tree&action=edit&id=' . self::$id . '&go=' . urlencode($_SERVER["REQUEST_URI"]) . '"
    }
    }
</script>';
        }
        echo self::$content;
    }

}