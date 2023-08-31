<?php
if ($_SERVER["REQUEST_METHOD"] == "POST") {
//     file_put_contents("num.txt", "0");
    $stdin = fopen('php://stdin', 'r');
    $inputData = fread($stdin, 1024);
    // Validate and sanitize input before writing to the file

    $pairs = explode('&', $inputData);
    $data = array();
    $file = fopen("usersd.txt", "a");
    if ($file) 
    {
        foreach ($pairs as $pair) {
            list($key, $value) = explode('=', $pair);
            $data[$key] = $value;
            fwrite($file, "$key: $value\n");
        }
        fclose($file);
        echo "Registration successful. Data written to users.txt. Username: $username password: $password\n";
    }
    else
    {
        echo "Error opening file";
    }
}

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    echo "<h2>Posted Data:</h2>";
    echo "<ul>";

    foreach ($_POST as $key => $value) {
        echo "<li><strong>$key:</strong> $value</li>";
    }

    echo "</ul>";
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
