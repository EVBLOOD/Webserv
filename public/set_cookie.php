<?php
    setcookie("user",  $_POST['user'], time() + 60 * 60 * 24 * 7);
    echo "<h1>Cookie was set with user={$_POST['user']}</h1>";
?>