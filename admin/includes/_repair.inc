<?

class _repair {

    public static $content = 'ok', $title;

    public static function main() {
        self::$title = 'Скрипт переводит нулевые pattern_id в sections в родительские';
        $res = DB::query('SELECT id,leftk,rightk FROM sections WHERE pattern_id=0');
        while ($row = $res->fetch_array()) {
            $r = DB::query('SELECT pattern_id FROM sections WHERE leftk<' . $row['leftk'] . ' AND rightk>' . $row['rightk'] .
                            ' AND pattern_id<>0 ORDER BY depth DESC LIMIT 1')->fetch_array();
            DB::query('UPDATE sections SET pattern_id=' . $r[0] . ' WHERE id=' . $row['id']);
        }
    }

    public static function set_pattern_id() {
        self::$title = 'Скрипт проставляет pattern_id в sections';
        $res = DB::query('SELECT id,pattern_part FROM patterns');
        while ($row = $res->fetch_array()) {
            echo 'UPDATE sections SET pattern_id=' . $row['id'] . ' WHERE part="' . $row['pattern_part'] . '"<br>';
            DB::query('UPDATE sections SET pattern_id=' . $row['id'] . ' WHERE part="' . $row['pattern_part'] . '"');
        }
    }

    public static function repair_depth($id = 1, $d = 0) {
        DB::query('UPDATE sections SET depth=' . $d . ' WHERE id=' . $id);
        ++$d;
        $res = DB::query('SELECT id FROM sections WHERE parent=' . $id . ' ORDER BY leftk');
        while ($row = $res->fetch_array()) {
            self::repair_depth($row['id'], $d);
        }
    }

    public static function set_lables() {
        $standart = array('page_content', 'title', 'description', 'keywords', 'page_name');
        foreach ($standart as $lbl) {
            DB::query("UPDATE patterns SET pattern_text=REPLACE(pattern_text,'<!--$lbl-->','<!--\{$lbl}-->')");
            DB::query("UPDATE sections SET content=REPLACE(content,'<!--$lbl-->','<!--\{$lbl}-->')");
        }
        $lbls = array('cars_info'/*, 'actions_small', 'collor_cars', 'bu_auto', 'child_menu', 'baners', 'actions', 'constructor',
            'content', 'form', 'form_to', 'form_zap', 'gmenu', 'mail',
            'news', 'photo', 'poll', 'popup', 'uaz_menu', 'uaz_news', 'uaz_way', 'vmenu'*/);

//        $lbls = array('vmenu');
        $res = DB::query('SELECT content,id FROM sections');
        while ($row = $res->fetch_array()) {
            foreach ($lbls as $lbl) {
                self::$content .= '<br>yes1-' . $lbl;
                $row['content'] = preg_replace("#\<!--$lbl(\[.*\])?--\>#sUu", "<!--{" . $lbl . "$1}-->", $row['content']);
            }
            DB::query("UPDATE sections SET content='" . addslashes($row['content']) . "' WHERE id=" . $row['id']);
        }
        $res = DB::query('SELECT pattern_text,id FROM patterns');
        while ($row = $res->fetch_array()) {
            foreach ($lbls as $lbl) {
                $res = DB::query('SELECT pattern_text,id FROM patterns');
                self::$content .= '<br>yes2-' . $lbl;
                $row['pattern_text'] = preg_replace("#\<!--$lbl(\[.*\])?--\>#sUu", "<!--{" . $lbl . "$1}-->", $row['pattern_text']);
            }
            DB::query("UPDATE patterns SET pattern_text='" . addslashes($row['pattern_text']) . "' WHERE id=" . $row['id']);
        }
    }

    public static function mkKeys($id = 1) {
        static $k = 0;
        $lk = ++$k;
        $res = DB::query('SELECT id FROM sections WHERE parent=' . $id . ' ORDER BY priority');
        while ($row = $res->fetch_array()) {
            self::mkKeys($row['id']);
        }
        DB::query('UPDATE sections SET leftk=' . $lk . ',rightk=' . ++$k . ' WHERE id=' . $id);
    }

    public static function getmd5() {
        self::$content = md5($_GET['str']);
    }

}