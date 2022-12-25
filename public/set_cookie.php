<?php
    setcookie("user",  $_POST['user'], time() + 3600);
    echo "<h1>Cookie was set with user={$_POST['user']}</h1>";
?>