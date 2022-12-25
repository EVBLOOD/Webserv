<?php
    session_start();
    $_SESSION['log'] = $_POST['log'];
    echo "<h1>Session was set with log={$_POST['log']}</h1>";
?>