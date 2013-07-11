<?php

if ((isset(self::$_items['parent']) && self::$_items['parent']) || (isset($id) && $id != 1)) {
    $select_parent = '<tr>
            <td>Родитель</td>
            <td>
<select size="1" name="parent">';
    $id_str = (isset($id)) ? ' WHERE id<>' . $id : '';
    $query = "SELECT id,name FROM sections$id_str ORDER BY leftk";
    $result = DB::query($query);
    while ($row = $result->fetch_array()) {
        $selected = ($row['id'] == self::$_items['parent']) ? ' selected' : '';
        $select_parent .='  <option value="' . $row['id'] . '"' . $selected . '>' . $row['name'] . '</option>';
    }
    $select_parent .='</select></td>
        </tr>
';
} else {
    $select_parent = '<input type="hidden" value="0" name="parent">';
}
$design = '<select size="1" name="pattern_id">';
if (!isset($id) || $id != 1) {
    $design .= '    <option value="0">Наследовать дизайн родителя</option>';
}
$query = 'SELECT id,pattern_desc FROM patterns';
$result = DB::query($query);
while ($row = $result->fetch_array()) {
    $selected = ($row['id'] == self::$_items['pattern_id']) ? ' selected' : '';
    $design .= '    <option value="' . $row['id'] . '"' . $selected . '>' . $row['pattern_desc'] . '</option>';
}
$design .= '</select>';
self::$content = '
<script src="/ckeditor/ckeditor.js" type="text/javascript"></script>
<script type="text/javascript">
    $(document).ready(function(){
        CKEDITOR.replace( \'ta\',{});
        $(\'#allparam_q\').click(function(){
            $(\'#allparam\').stop(true,true).slideToggle(300);
            $(this).stop(true,true).fadeOut(0,function(){
                if($(this).text()==\'Полная форма\'){
                    $(this).text(\'Простая форма\');
                }else{
                    $(this).text(\'Полная форма\');
                }
                $(this).fadeIn(500);
            });        
            return false;
        });
        $(\'.no_quotes\').keypress(function (e){
            if(e.keyCode==39 || e.keyCode==34){
                alert(\'Кавычки запрещены в этом поле.\');
                event.returnValue=false;
            }
        });
    });
</script>
<form method="post">
<div>
    <table border="0" cellpadding="0" cellspacing="5" width="100%">
        ' . $select_parent . '
        <tr>
            <td>Название раздела</td>
            <td><input type="text" name="name" size="84" value="' . self::$_items['name'] . '"></td>
        </tr>
    </table>
</div>
<a href="#" id="allparam_q">Полная форма</a><br>
<div id="allparam">
		<table border="0" cellpadding="0" cellspacing="5" width="100%">
			<tr>
				<td>Шаблон дизайна</td>
				<td>' . $design . ' Видимый <input type="checkbox" name="visible" value="1"' . ((self::$_items['visible']) ? ' checked' : '') . '></td>
			</tr>
			<tr>
				<td>Титул страницы (авто)</td>
				<td><input type="text" name="title" size="84" value="' . self::$_items['title'] . '"></td>
			</tr>
			<tr>
				<td>Адрес страницы URL (авто)</td>
				<td><input type="text" name="directory" size="84" value="' . self::$_items['directory'] . '"> </td>
			</tr>
			<tr>
				<td>Описание для робота</td>
				<td><input type="text" name="description" size="84" value="' . self::$_items['description'] . '" class="no_quotes"></td>
			</tr>
			<tr>
				<td>Ключевые слова для робота</td>
				<td><input type="text" name="keywords" size="84" value="' . self::$_items['keywords'] . '" class="no_quotes"></td>
			</tr>
			<tr>
				<td>Переход на другой URL</td>
				<td><input type="text" name="url" size="84" value="' . self::$_items['url'] . '"></td>
			</tr>
                </table>
</div>
<br>
<textarea id="ta" name="content" style="width:100%" rows="24" cols="80">' . self::$_items['content'] . '
</textarea><p/>
<input type="submit" name="ok" value="Сохранить" />
</form>
';