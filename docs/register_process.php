<?php
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $stdin = fopen('php://stdin', 'r');
    $inputData = fread($stdin, 1024);
    // Validate and sanitize input before writing to the file

    $pairs = explode('&', $inputData);

    $data = array();
    foreach ($pairs as $pair) {
        list($key, $value) = explode('=', $pair);
        $data[$key] = $value;
    }

    $username = $data['username'];
    $password = $data['password'];


    // Open the file in append mode
    $file = fopen("users.txt", "a");

    if ($file) {
        // Write the user's information to the file
        fwrite($file, "Username: $username\n");
        fwrite($file, "Password: $password\n");
        fwrite($file, "-----------------\n");

        fclose($file);

        echo "Registration successful. Data written to users.txt. Username: $username $password\n";
    } else {
        echo "Error writing to file.";
    }
}
?>
<html lang="en">
<link rel="stylesheet" href="css/error.css">
<script src="js/stars.js"></script>
<div class="text">
    <div>Sucess</div>
    <h1>Login</h1>
    <hr>
    <div>Enjoy!</div>
</div>
<a href="../index.html" class="button">Go home</a>
<div class="astronaut">
    <img src="../assets/astronaut.png" alt="" class="src">
</div>
</html>
