<?php
session_start();

// Set session data
$_SESSION["username"] = "john_doe";
$_SESSION["password"] = 123;

?>
<!DOCTYPE html>
<html>
<link rel="shortcut icon" href="assets/planet.png" />
    <link rel="stylesheet" href="css/style.css">
    <link href="https://fonts.googleapis.com/css2?family=Kaushan+Script&family=Montserrat:wght@400;700&display=swap" rel="stylesheet">
<head>
    <title>Register Page</title>
</head>
<body>
    <h2>Register</h2>
    <form action="register_process.php" method="post">
        <label for="username">Username:</label>
        <input class="input-group__input" type="text" id="username" name="username" required><br><br>
        <label for="password">Password:</label>
        <input class="input-group__input" type="password" id="password" name="password" required><br><br>
        <input class="button" type="submit" value="Register">
    </form>
</body>
</html>
