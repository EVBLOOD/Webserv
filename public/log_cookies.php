<?php 
    if (isset($_COOKIE['user'])) {
        $user = $_COOKIE['user'];
        if ($user != null) {
            echo "<h1>hello $user</h1>";
        }
    }
    else {
        echo "
        <html>
            <form method='POST' action='set_cookie.php'>
                <input type='text' name='user'>

                <button type='submit'> submit </button>
            </form>
        </html>
        ";
    }
?>
