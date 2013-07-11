<?php

class feedback {

    private $html;

    function __construct() {
        if ($_SERVER['REQUEST_METHOD'] == 'POST') {
            $error_flag = 0;
            $vars = ['first_name' => 515, 'last_name' => 515, 'email' => 515, 'phone' => 515, 'comments' => 515, 'other' => 515];
            $vars = filter_input_array(0, $vars);
            if (isset($_POST['contact']) && $_POST['contact'] == 'Email') {
                $mail_check = ' checked="checked"';
                $phone_check = '';
            } else {
                $phone_check = ' checked="checked"';
                $mail_check = '';
            }
            if (!$vars['first_name']) {
                $vars['first_name'].='" style="border-color:red"';
                $error_flag = 1;
            }
            if (!$vars['email']) {
                $vars['email'].='" style="border-color:red"';
                $error_flag = 1;
            }
            if (!$vars['phone']) {
                $vars['phone'].='" style="border-color:red"';
                $error_flag = 1;
            }
            $vars['contact'] = '<input name="contact" type="radio" value="Телефон" id="phone_contact" ' . $phone_check .
                    '><label for="phone_contact"> Телефон</label> <input name="contact" type="radio" value="Email" id="email_contact"' .
                    $mail_check . '><label for="email_contact"> Email</label><br>';
            $refer = ['Buyer' => 'Брокер', 'Advertisment' => 'Наружная реклама', 'Newspaper' => 'Журнал/Газета', 'Magazine' => 'Интернет', 'Referal' => 'Рекомендации'];
            $vars['refer'] = '';
            foreach ($refer as $key => $val) {
                $checked = ($val == $_POST['refer']) ? ' checked="checked"' : '';
                $vars['refer'].='<input name="refer" type="radio" value="' . $val . '" id="' . $key . '"' . $checked . '> <label for="' . $key . '">' . $val . '</label><br>';
            }
            if (!$error_flag) {
                $msg = <<<MSG
Имя: {$_POST['first_name']}
Фамилия: {$_POST['first_name']}
Email: {$_POST['email']}
Телефон: {$_POST['phone']}
Связаться по: {$_POST['contact']}
Как вы узнали о нас: {$_POST['refer']}
Другое: {$_POST['other']}
Комментарии: 
{$_POST['comments']}
MSG;
                $headers = "From: admin@mail.ru\nBcc: zxy@zxy.ru\n";
                mail('user@mail.ru', 'Форма обратной связи', $msg, $headers) or die($msg);
                $vars['msg'] = '<span style="color:white">Ваше сообщение отправлено!</span>';
            } else {
                $vars['msg'] = '<span style="color:white">Заполните обязательные поля!</span>';
            }
        } else {
            $vars = ['first_name' => '', 'last_name' => '', 'email' => '', 'phone' => '', 'comments' => '', 'refer' => '', 'other' => '', 'msg' => ''];
            $vars['contact'] = '<input name="contact" type="radio" value="Телефон" id="phone_contact" checked="checked"><label for="phone_contact">
                Телефон</label> <input name="contact" type="radio" value="Email" id="email_contact"><label for="email_contact"> Email</label><br>';
            $vars['refer'] = '
                        <input name="refer" type="radio" value="Брокер" id="Buyer"> <label for="Buyer">Брокер</label><br>
                        <input name="refer" type="radio" value="Наружная реклама" id="Advertisment"> <label for="Advertisment">Наружная реклама</label><br>
                        <input name="refer" type="radio" value="Журнал/Газета" id="Newspaper"> <label for="Newspaper">Журнал/Газета</label><br>
                        <input name="refer" type="radio" value="Интернет" id="Magazine"> <label for="Magazine">Интернет</label><br>
                        <input name="refer" type="radio" value="Рекомендации" id="Referal"> <label for="Referal">Рекомендации</label>
                                    ';
        }
        $template = file_get_contents('modules/templates/feedback.html');
        $lbls = array_map(function($a) {
                    return '{' . $a . '}';
                }, array_keys($vars));
        $this->html = str_replace($lbls, $vars, $template);
    }

    function __toString() {
        return $this->html;
    }

}