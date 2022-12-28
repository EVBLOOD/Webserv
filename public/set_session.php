<?php
if (isset($_POST['log'])) {
    session_start();
    $_SESSION['log'] = $_POST['log'];
    echo "<h1>Session was set with log={$_POST['log']}</h1>";
} else {
    echo "<h1>pls set user in log_session.php</h1>";
}
?>