<?php

class main_page {

    public static $title = 'Административная панель', $content;

    public static function main() {
        self::$content = '
<p><b>Здравствуйте, ' . $_SESSION['name'] . '!</b><br />Вы вошли в систему управления сайтом!</p>
<table width="100%">
    <tr>
        <!-- Левый блок -->
        <td valign="top" width="60%">
            <div class="pblock">
                <p class="pbh">Краткая информация</p>
                <script language=JavaScript>
                    function show(obj) {
                    if (document.getElementById(obj).style.display == \'none\')
                            document.getElementById(obj).style.display = \'block\';
                            else document.getElementById(obj).style.display = \'none\';
                            }
                </script>
                <ul>
                    <li class="menu" onclick="show(\'sub1\')" style="cursor: pointer;">Разделы</li>
                    <span class="sub" id="sub1" style="display: none">
                        <b>Что бы добавить раздел:</b><br />
                        Перейдите в Древо разделов и нажмите на <img border="0" src="/admin/images/new.gif"/>.<br />
                        <b>Что бы отредактировать раздел:</b><br />
                        Перейдите в Древо разделов и нажмите на <img border="0" src="/admin/images/edit.gif"/>.<br />
                        <b>Что бы удалить раздел:</b><br />
                        Перейдите в Древо разделов и нажмите на <img border="0" src="/admin/images/delete.gif"/>.
                    </span>
                    <li class="menu" onclick="show(\'sub2\')" style="cursor: pointer;">Новости</li>
                    <span class="sub" id="sub2" style="display: none">
                        <b>Что бы добавить новость:</b><br />
                        Перейдите в Новости и нажмите добавить.<br />
                        <b>Что бы отредактировать новость:</b><br />
                        Перейдите в Новости и нажмите на название новости.<br />
                        <b>Что бы удалить новость:</b><br />
                        Перейдите в Новости, нажмите на название а потом удалить.
                    </span>
                    <li class="menu" onclick="show(\'sub3\')" style="cursor: pointer;">Метки<sup>1</sup></li>
                    <span class="sub" id="sub3" style="display: none">
                        <b>Для чего нужны метки.</b><br />
                        Метки нужны для того что бы вставить в определенный раздел новости, фотогалерею, акции или чтото другое.<br />
                        <b>Форматы меток</b><br />
                        Новости - &lt;!--{news[]}--&gt;<br />
                        Фотогалерея - &lt;!--{photo[]}--&gt;
                    </span>
                    <li class="menu" onclick="show(\'sub4\')" style="cursor: pointer;">Файлы</li>
                    <span class="sub" id="sub4" style="display: none">
                        <b>Как залить файл</b><br />
                        Если вам нужно загрузить на сайт файлы форматов типа: .doc .xls .rar и т.д. Вам нужно перейти в Загрузка в /files/, нажать обзор, выбрать нужный файл и назать загрузить.<br />
                        <b>Как вставить файл в раздел</b><br />
                        Вам нужно зайти в редактирование раздела написать текст ссылки на файл, выделить его и нажать на значок <img border="0" src="/admin/images/ed_link.png"/>  (Вставить/редактировать ссылку). В открывшемся окошке ввести /files/имя загруженоговами файла и нажать вставить.
                    </span>
                </ul>
                <p style="font-size: 9px;"><sup>1</sup> Будте осторожны!!!</p>
                Рекомендуемый браузер: <strong><a href="https://www.google.com/intl/ru/chrome/browser/" target="_blank">Google Chrome</a></strong>
            </div>
            <div class="pblock">
                <p class="pbh">Новые возможности</p>
                <p>Доступен упрощенный режим редактирования контента.
                       Пользователь, авторизованный в административной панели, может перейти к редактированию раздела непосредственно со страницы этого раздела.
                       Для этого необходимо осуществить двойной клик на контентной области страницы.
                       Функционал можно подключить/отключить в пункте <a href="/admin/?inc=opts&action=options">Настройки системы</a></p>
                 <p>Появилась возможность просматривать историю изменений шаблонов.  Для этого на странице редактирования шаблона надо перейти по ссылке "История". Количество сохранений в истории можно изменить в пункте <a href="/admin/?inc=opts&action=options">Настройки системы</a></p>
            </div>
        </td>
        <!-- /Левый блок -->
    </tr>
</table>
';
    }

}