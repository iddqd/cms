<?php

if (isset($_GET['id']) and $id = abs((int) $_GET['id'])) {
    include DR.'/admin/includes/dst.inc';
    include DR.'/classes/DB.php';
    $result = DB::query('SELECT content FROM sections WHERE id=' . $id);
    $row = $result->fetch_array();
    echo '<textarea id="ta" style="width:100%;height:100%">' . $row['content'] . '</textarea><button id="easySubmit" value="Сохранить">';
}
