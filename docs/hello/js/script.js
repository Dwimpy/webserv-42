document.getElementById("submitButton").addEventListener("click", function() {
    var userNameValue = document.getElementById("textField1").value;
    var passwordValue = document.getElementById("textField2").value;

    // Create an object to hold the data you want to send
    var data = {
        user: userNameValue,
        password: passwordValue
    };
    document.getElementById("textField1").value = 'login';
    document.getElementById("textField2").value = 'password';
    document.getElementById("textField2").type = 'text';
    // Make a POST request to the server using the Fetch API
    fetch("http://127.0.0.1:8080/", {
        method: "POST",
        headers: {
            "Content-Type": "application/json"
        },
        body: JSON.stringify(data)
    })
        .then(response => response.json())
        .then(responseData => {
            console.log("Server response:", responseData);
            // You can handle the response from the server here
        })
        .catch(error => {
            console.error("Error:", error);
            // Handle errors here
        });
});
document.getElementById("textField1").addEventListener("click", function()
{
    let textFieldValue = document.getElementById("textField1");
    if (textFieldValue.value === "login")
        textFieldValue.value='';
});
document.getElementById("textField2").addEventListener("click", function()
{
    let textFieldValue = document.getElementById("textField2");
    if (textFieldValue.value === "password")
    {
        textFieldValue.value='';
        textFieldValue.type="password";
    }
});