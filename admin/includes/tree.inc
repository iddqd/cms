<?php

class tree {

    public static $content, $title = 'Дерево разделов';
    private static $_items = array('content' => '', 'parent' => '', 'name' => '', 'title' => '', 'description' => '', 'keywords' => '', 'url' => '', 'pattern_id' => '', 'visible' => '', 'directory' => '');

    private static function move($id, $up_id, $right_key_near) {
        $query = 'SELECT depth,leftk,rightk FROM sections WHERE id=' . $id;
        $result = DB::query($query);
        list( $depth, $left_key, $right_key) = $result->fetch_array(MYSQLI_NUM);
        $query = 'SELECT depth FROM sections WHERE id=' . $up_id;
        $result = DB::query($query);
        list($up_depth) = $result->fetch_array(MYSQLI_NUM);
        $skew_level = $up_depth - $depth + 1;
        $skew_tree = $right_key - $left_key + 1;
        if ($right_key_near < $right_key) {
            $skew_edit = $right_key_near - $left_key + 1;
            $query = "UPDATE sections 
            SET rightk= IF(leftk>= $left_key,rightk+ $skew_edit,IF(rightk<$left_key,rightk+$skew_tree,rightk)), 
            depth=IF(leftk>=$left_key, depth + $skew_level, depth), 
            leftk=IF(leftk>=$left_key,leftk+$skew_edit,IF(leftk>$right_key_near,leftk+$skew_tree,leftk))
            WHERE rightk>$right_key_near AND leftk< $right_key";
        } else {
            $skew_edit = $right_key_near - $left_key + 1 - $skew_tree;
            $query = "UPDATE sections 
            SET leftk=IF(rightk<= $right_key,leftk+$skew_edit,IF(leftk>$right_key,leftk-$skew_tree,leftk)), 
            depth=IF(rightk<=$right_key,depth+$skew_level,depth), 
            rightk=IF(rightk<=$right_key,rightk+$skew_edit,IF(rightk<=$right_key_near,rightk-$skew_tree,rightk))
            WHERE rightk>$left_key AND leftk<=$right_key_near";
        }
        DB::query($query);
        DB::query('UPDATE sections SET parent=' . $up_id . ' WHERE id=' . $id);
    }

    public static function pr_up() {
        if ($parent = abs((int) $_GET['parent']) and $leftk = abs((int) $_GET['leftk']) and $id = abs((int) $_GET['id'])) {
            $query = 'SELECT rightk  FROM sections WHERE parent=' . $parent . ' AND rightk<' . $leftk . '-1 ORDER BY leftk DESC LIMIT 1';
            $result = DB::query($query);
            if (!$result->num_rows) {
                $query = 'SELECT leftk  FROM sections WHERE id=' . $parent;
                $result = DB::query($query);
            }
        }
        if ($result->num_rows) {
            $row = $result->fetch_array();
            $right_key_near = $row[0];
            self::move($id, $parent, $right_key_near);
        }
        header('Location: /admin/?inc=tree');
    }

    public static function pr_down() {
        if ($_GET['action'] == 'pr_down' && $parent = abs((int) $_GET['parent']) and $leftk = abs((int) $_GET['leftk']) and $id = abs((int) $_GET['id'])) {
            $query = 'SELECT rightk  FROM sections WHERE parent=' . $parent . ' AND leftk>' . $leftk . ' ORDER BY leftk LIMIT 1';
            $result = DB::query($query);
        }
        if ($result->num_rows) {
            $row = $result->fetch_array();
            $right_key_near = $row[0];
            self::move($id, $parent, $right_key_near);
        }
        header('Location: /admin/?inc=tree');
    }

    public static function delete() {
        if (isset($_GET['rightk'], $_GET['leftk']) and $leftk = abs((int) $_GET['leftk']) and $rightk = abs((int) $_GET['rightk'])) {
            DB::query("DELETE FROM sections WHERE leftk>=$leftk AND rightk<=$rightk");
            DB::query("UPDATE sections SET leftk= 
            IF(leftk>$leftk,leftk-($rightk-$leftk+1),leftk),rightk=rightk-($rightk-$leftk+1) 
                WHERE rightk>$rightk");
            header('Location: /admin/?inc=tree');
        }
    }

    public static function edit() {
        if (isset($_GET['id']) and $id = abs((int) $_GET['id'])) {
            $result = DB::query('SELECT rightk,leftk,content,parent,name,title,description,keywords,url,pattern_id,visible,directory FROM sections WHERE id=' . $id);
            if ($result->num_rows) {
                if ($_SERVER['REQUEST_METHOD'] == 'POST') {
                    self::save_edit($result, $id);
                } else {
                    $row = $result->fetch_array(MYSQLI_ASSOC);
                    foreach ($row as $key => $val) {
                        self::$_items[$key] = $val;
                    }
                    include 'includes/make_page_form.inc';
                    self::$title = 'Редактирование раздела';
                }
            } else {
                self::main();
            }
        } else {
            self::main();
        }
    }

    public static function add() {
        if (isset($_GET['id']) and self::$_items['parent'] = abs((int) $_GET['id'])) {
            if ($_SERVER['REQUEST_METHOD'] == 'POST') {
                self::save('add');
            } else {
                include 'includes/make_page_form.inc';
                self::$title = 'Создание раздела';
            }
        } else {
            self::main();
        }
    }

    public static function save_edit($result, $id) {
        $row = $result->fetch_array(MYSQLI_ASSOC);
        foreach ($row as $key => $val) {
            $$key = (isset($_POST[$key])) ? DB::real_escape_string(trim($_POST[$key])) : '';
        }
        if (empty($directory)) {
            $directory = $name;
        }
        $directory = Helper::transliterate(str_replace(' ', '-', $directory));
        $directory = preg_replace('#[^\w-]#su', '', strtolower($directory));
        $count = 1;
        $directory_buf = $directory;
        do {
            $query = 'SELECT id FROM sections WHERE directory="' . $directory_buf . '" AND parent=' . $parent . ' AND id<>' . $id;
            $result = DB::query($query);
            if ($result->num_rows) {
                $directory_buf = $directory . '-' . $count++;
            }
        } while ($result->num_rows);
        $directory = $directory_buf;
        if (!$pattern_id) {
            $pattern_id = DB::query('SELECT pattern_id FROM sections WHERE id=' . $parent)->fetch_array()[0];
        }
        DB::query("UPDATE sections SET name='$name', directory='$directory', title='$title', description='$description',
    keywords='$keywords', content='$content', url='$url', pattern_id='$pattern_id', visible='$visible' WHERE id=$id");
        if ($row['parent'] != $parent) {
            $result = DB::query('SELECT rightk FROM sections WHERE id=' . $parent);
            $row = $result->fetch_array();
            self::move($id, $parent, $row['rightk'] - 1);
        }
        $go = (isset($_GET['go'])) ? $_GET['go'] : '/admin/?inc=tree';
        header("Location: $go");
    }

    public static function save($action) {
        foreach (self::$_items as $key => $val) {
            $$key = (isset($_POST[$key])) ? DB::real_escape_string(trim($_POST[$key])) : '';
        }
        $query = 'SELECT depth,rightk FROM sections WHERE id=' . self::$_items['parent'];
        $result = DB::query($query);
        if ($result->num_rows) {
            $row = $result->fetch_array();
            $depth = ++$row['depth'];
            $right = $row['rightk'];
            if (empty($directory)) {
                $directory = $name;
            }
            $directory = Helper::transliterate(str_replace(' ', '-', $directory));
            $directory = preg_replace('#[^\w-]#su', '', strtolower($directory));
            $count = 1;
            $directory_buf = $directory;
            do {
                $query = 'SELECT id FROM sections WHERE directory="' . $directory_buf . '" AND parent=' . $parent;
                $result = DB::query($query);
                if ($result->num_rows) {
                    $directory_buf = $directory . '-' . $count++;
                } else {
                    $directory_buf = $directory;
                    break;
                }
            } while (true);
            $directory = $directory_buf;
            if ($action == 'add') {
                DB::query("UPDATE sections SET rightk=rightk+2,leftk=IF(leftk>$right,leftk+2,leftk) WHERE rightk>=$right");
                if (!$pattern_id) {
                    $pattern_id = DB::query('SELECT pattern_id FROM sections WHERE leftk<' . $right . ' AND rightk>' . ($right + 1)
                                    . ' AND pattern_id<>0 ORDER BY depth DESC LIMIT 1')->fetch_array()[0];
                }
                $query = "INSERT INTO sections SET parent=$parent, name='$name', directory='$directory', title='$title', description='$description',
    keywords='$keywords', content='$content', url='$url', pattern_id='$pattern_id', visible='$visible', depth=$depth,
    leftk=$right, rightk=$right+1";
            }
            $result = DB::query($query);
            header("Location: /admin/?inc=tree");
        } else {
            self::main();
        }
    }

    public static function main() {
        $path_arr = array();
        $up_id[0] = '1';
        $col = array(1 => 'EEE', 'DDD', 'CCC', 'BBB', 'AAA', '999', '888', '777', '666');
        $query = 'SELECT id,name,depth,directory,visible,leftk,rightk,parent FROM sections ORDER BY leftk';
        $result = DB::query($query);
        $row = $result->fetch_array();
        if (isset($_COOKIE['show_all']) && $_COOKIE['show_all'] == 1) {
            self::$content = '';
            $show_all = '[-] Свернуть';
            $show = '<span class="show">[-]</span>';
        } else {
            self::$content = '
<style type="text/css">
    .tree div{
        display:none;
    }
</style>';
            $show_all = '[+] Развернуть';
            $show = '<span class="show">[+]</span>';
        }
        self::$content .= '
<script type="text/javascript">
function setCookie(name, value) {
    if (!name || !value) return false;
    var str = name + \'=\' + encodeURIComponent(value)+\'; path=/admin/\';	
    document.cookie = str;
    return true;
}
function getCookie(name) {
    var pattern = "(?:; )?" + name + "=([^;]*);?";
    var regexp  = new RegExp(pattern);	
    if (regexp.test(document.cookie))
        return decodeURIComponent(RegExp["$1"]);	
    return false;
}
    $(document).ready(function(){
        var nodes=$(\'.tree > div\');
        $(\'.show\').click(function(){
            $(\'~div\',this).stop(true,true).slideToggle(150);
            if($(this).text()==\'[+]\'){
                $(this).text(\'[-]\');
            }else{
                $(this).text(\'[+]\');
            }
            return false;
        });
        $(\'#show_all\').click(function(){
            if(!nodes.length) return false;
            var now=new Date;
            var ndate=new Date;
            if(getCookie(\'show_all\')==1){
                nodes.css(\'display\',\'none\');
                $(\'.show\').text(\'[+]\');
                setCookie(\'show_all\', \'2\');
                $(\'#show_all\').text(\'[+] Развернуть\');
            }else{
                nodes.css(\'display\',\'block\');
                $(\'.show\').text(\'[-]\');
                setCookie(\'show_all\', \'1\');
                $(\'#show_all\').text(\'[-] Свернуть\');
            }
            return false;
        });
    });
</script>
<a href="#" id="show_all">' . $show_all . '</a>
<div id="root"><div><a href="?inc=tree&action=add&id=1">
<img border="0" src="/admin/images/new.gif"></a> <a target="_blank" href="?inc=tree&action=edit&id=1">
<img border="0" src="/admin/images/edit.gif"></a> 
<small><a href="/" style="' . ((isset($_GET['id']) && $_GET['id'] == 1) ? 'color:red;' : '') . '">[ссылка]</a></small> ' . $row['name'] . '
';
        $depth = 1;
        $section1 = $section2 = '';
        while ($row = $result->fetch_array()) {
            $ls = (isset($_GET['id']) && $_GET['id'] == $row['id']) ? 'color:red;' : '';
            $visible = (!$row['visible']) ? '<img border="0" src="/admin/images/invis.gif">' : $visible = '';
            if ($row['depth'] <= $depth) {
                self::$content .=$section1 . $section2 . str_repeat('</div>', $depth - $row['depth'] + 1);
            } elseif ($row['depth'] > $depth) {
                self::$content .=$section1 . $show . $section2;
            }
            $depth = $row['depth'];
            $path_arr = array_splice($path_arr, 0, $depth - 1);
            $path_arr[$depth] = $row['directory'];
            $section1 = '<div class="tree" style="background-color:#' . $col[$depth] . '">';
            $section2 = ' <a href="?inc=tree&action=add&id=' . $row['id'] .
                    '"><img border="0" src="/admin/images/new.gif"></a> <a href="?inc=tree&action=edit&id=' .
                    $row['id'] . '"><img border="0" src="/admin/images/edit.gif"></a> <a href="?inc=tree&action=pr_down&id=' .
                    $row['id'] . '&parent=' . $row['parent'] . '&leftk=' . $row['leftk'] . '&rightk=' . $row['rightk'] . '&depth=' . $row['depth']
                    . '"><img border="0" src="/admin/images/bottom.gif"></a> <a href="?inc=tree&action=pr_up&id=' .
                    $row['id'] . '&parent=' . $row['parent'] . '&leftk=' . $row['leftk'] . '&rightk=' . $row['rightk'] . '&depth=' . $row['depth']
                    . '"><img border="0" src="/admin/images/up.gif"></a> <a href="?inc=tree&action=delete&leftk=' . $row['leftk'] . '&rightk=' .
                    $row['rightk'] . '" class="confirm" id="' . $row['name'] . '"><img border="0" src="/admin/images/delete.gif"></a> '
                    . $visible . ' <small> <a href="/' . implode('/', $path_arr) . '" style="' . $ls . '">[ссылка]</a> </small>' . $row['name'];
        }
        self::$content .=$section1 . $section2 . str_repeat('</div>', $depth - $row['depth'] + 1);
        self::$content = self::$content;
    }

}