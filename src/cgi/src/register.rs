// extern crate askama;

// use std::fs;
// use std::env;
// use askama::Template;

// #[derive(Template)]
// #[template(path = "tRegisterPage.html")]
// struct RegisterTemplate {
//     file_contents: String,
// }

// fn main() {
//     let request_method = env::var("REQUEST_METHOD").unwrap_or_else(|_| String::from(""));

//     let mut file_contents = String::new();

//     if request_method == "POST" {
//         let file_path = "num.txt";
        
//         if let Ok(file_content) = fs::read_to_string(file_path) {
//             if let Ok(parsed_value) = file_content.parse::<i32>() {
//                 let incremented_value = parsed_value + 1;
//                 file_contents = incremented_value.to_string();
//                 fs::write(file_path, incremented_value.to_string()).expect("Failed to write to file");
//             }
//         } else {
//             fs::write(file_path, "1").expect("Failed to write to file");
//             file_contents = String::from("1");
//         }
//     }

//     println!("Content-Type: text/html\n\n");
//     let template = RegisterTemplate { file_contents };
//     println!("{}", template.render().unwrap());
// }

extern crate askama;
extern crate r2d2;

use std::collections::HashMap;
use std::io;
use std::io::prelude::*;
use std::env;
use askama::Template;
mod database;
use database::establish_connection;


#[derive(Template)]
#[template(path = "tRegisterLandingPage.html")]
struct TemplateData;
struct RegisterTemplate {
    file_contents: String,
}

fn main() {
    let request_method = env::var("REQUEST_METHOD").unwrap_or_else(|_| String::from(""));

    let pool = establish_connection().expect("Failed to establish connection");

    if request_method == "POST" {
        let mut input_data = String::new();
        io::stdin().read_to_string(&mut input_data).expect("Failed to read input data");

        let pairs: Vec<&str> = input_data.trim().split('&').collect();
        let mut data = HashMap::new();

        // Form data parsing
        for pair in pairs {
            let parts: Vec<&str> = pair.split('=').collect();
            if parts.len() == 2 {
                let key = parts[0];
                let value = parts[1];
                data.insert(key.to_string(), value.to_string());
            }
        }

        // Insert user data into the database
        let conn = pool.get().expect("Failed to get connection from pool");
        conn.execute("INSERT INTO users (username, password) VALUES (?, ?)",
                     &[data.get("username").unwrap(), data.get("password").unwrap()])
            .expect("Failed to insert data");

        println!("Content-Type: text/html\n\n");
        let template = TemplateData {};
        println!("{}", template.render().unwrap());
    } else {
        println!("Content-Type: text/html\n\n");
        let template = TemplateData {};
        println!("{}", template.render().unwrap());
    }
}

