<?php

session_name('SID');
session_start();

if (isset($_SESSION['id'])) {
    $query = 'SELECT session FROM users WHERE id=' . $_SESSION['id'];
    $result = DB::query($query);
    $row = $result->fetch_array();
    if ($row['session'] != session_id()) {
        ?>
        <script>
            alert("В системе есть другой пользователь под вашим логином");
            document.location.href = "?inc=login"
        </script>
        <?php

        session_destroy();
        die;
    }
} else {
    include 'includes/login.inc';
    login::main();
    die;
}
