<?php

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $filePath = "num.txt";

    if (file_exists($filePath)) {
        $fileContents = file_get_contents("num.txt");
        if ($fileContents !== false) {
            if(is_numeric($fileContents))
            {
//                echo "file content is $fileContents and it is numeric";
                $fileContents++;
            }
//            echo "file content is $fileContents";
            file_put_contents("num.txt", $fileContents);
        }
    } else {
        $fileContents = 1;
        file_put_contents("num.txt", $fileContents);

    }

//        echo "in the post";

}
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
        <form method="post">
                <input class="button" type="submit" name="increment" value=<?php echo $fileContents?>>
        </form>

</body>
</html>
