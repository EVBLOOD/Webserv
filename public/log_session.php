<?php 
    session_start();
    if (isset($_SESSION['log'])) {
        $log = $_SESSION['log'];
        if ($log != null) {
            echo "<h1>hello $log</h1>";
        }
    }
    else {
        echo "
        <html>
            <form method='POST' action='set_session.php'>
                <input type='text' name='log'>

                <button type='submit'> submit </button>
            </form>
        </html>
        ";
    }
?>
