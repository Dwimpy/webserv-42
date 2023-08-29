<?php
session_start();

// Set session data
$_SESSION["username"] = "john_doe";
$_SESSION["password"] = 123;

?>
<!DOCTYPE html>
<html>
<head>
    <title>Register Page</title>
</head>
<body>
    <h2>Register</h2>
    <form action="register_process.php" method="post">
        <label for="username">Username:</label>
        <input type="text" id="username" name="username" required><br><br>
        <label for="password">Password:</label>
        <input type="password" id="password" name="password" required><br><br>
        <input type="submit" value="Register">
    </form>
</body>
</html>
