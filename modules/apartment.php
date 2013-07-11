<?php

class apartment {

    private $html = '';

    public function __construct() {
        if (isset(Page::$get[0]) AND (0 < ($id = (int) Page::$get[0]))) {
            $this->showOne($id);
        } else {
            $this->showList();
        }
    }

    private function showList($part = 'part1') {
        $query = 'SELECT * FROM apartment';
        if (isset(Page::$get[0])) {
            if (0 === strpos(Page::$get[0], 'lvl')) {
                $query .=(2 > ($lvl = abs((int) ltrim(Page::$get[0], 'lv'))) || $lvl > 11) ? '' : ' WHERE lvl=' . $lvl;
            } elseif (0 === strpos(Page::$get[0], 'kv')) {
                $query .=($rooms = abs((int) ltrim(Page::$get[0], 'kv'))) ? ' WHERE rooms=' . $rooms : '';
            }
        }

        $result = DB::query($query . ' ORDER BY price DESC');
        $template = file_get_contents('modules/templates/apartment_showList.html');
        preg_match('#{for:}(.*){:endfor}#sUu', $template, $matches);
        $for = '';
        while ($row = $result->fetch_assoc()) {
            $row['price-fm'] = number_format($row['price-fm'], 0, '', ' ');
            $row['price'] = number_format($row['price'], 0, '', ' ');
            $lbls = array_map(function($a) {
                        return '{' . $a . '}';
                    }, array_keys($row));
            $for.= str_replace($lbls, $row, $matches[1]);
        }
//        $template = str_replace('{page}', $page, $template);
        $this->html = str_replace($matches[0], $for, $template);
    }

    private function showOne($id) {
        $template = file_get_contents('modules/templates/apartment_showOne.html');
        if ($row = DB::query('SELECT * FROM apartment WHERE id=' . $id)->fetch_assoc()) {
            $row['price-fm'] = number_format($row['price-fm'], 0, '', ' ');
            $row['price'] = number_format($row['price'], 0, '', ' ');
            if (file_exists(DR . '/pdf/' . $row['id'] . '.pdf')) {
                $row['pdf'] = '<a href="/pdf/' . $row['id'] . '.pdf">Скачать PDF</a><br>';
            } else {
                $row['pdf'] = '';
            }
            $lbls = array_map(function($a) {
                        return '{' . $a . '}';
                    }, array_keys($row));
            $this->html = str_replace($lbls, $row, $template);
        } else {
            $this->html = 'Квартиры с номером ' . $id . ' нет.';
        }
    }

    public function __toString() {
        return $this->html;
    }

}