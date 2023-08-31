use std::collections::HashMap;
use std::fs::OpenOptions;
use std::io::{self, Read};
use std::io::prelude::*;
use std::env;

fn main() {
    let request_method = env::var("REQUEST_METHOD").unwrap_or_else(|_| String::from(""));
    
    if request_method == "POST" {
        let mut input_data = String::new();
        io::stdin().read_to_string(&mut input_data).expect("Failed to read input data");
        
        let pairs: Vec<&str> = input_data.trim().split('&').collect();
        let mut data = HashMap::new();
        
        let mut file = OpenOptions::new()
            .create(true)
            .append(true)
            .open("users.txt")
            .expect("Failed to open usersd.txt");
        
        for pair in pairs {
            let parts: Vec<&str> = pair.split('=').collect();
            if parts.len() == 2 {
                let key = parts[0];
                let value = parts[1];
                data.insert(key.to_string(), value.to_string());
                let _ = writeln!(file, "{}: {}", key, value);
            }
        }
        
        println!("Content-Type: text/html\n\n");
        println!("Registration successful. Data written to users.txt.");
        for (key, value) in &data {
            println!("{}: {}", key, value);
        }
    }
    
    println!("Content-Type: text/html\n\n");
    println!("<html lang=\"en\">");
    println!("<link rel=\"stylesheet\" href=\"css/error.css\">");
    println!("<script src=\"js/stars.js\"></script>");
    println!("<div class=\"text\">");
    println!("<div>Sucess</div>");
    println!("<h1>Login</h1>");
    println!("<hr>");
    println!("<div>Enjoy!</div>");
    println!("</div>");
    println!("<a href=\"../index.html\" class=\"button\">Go home</a>");
    println!("<div class=\"astronaut\">");
    println!("<img src=\"../assets/astronaut.png\" alt=\"\" class=\"src\">");
    println!("</div>");
    println!("</html>");
}
