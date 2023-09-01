// extern crate askama;
// extern crate r2d2;
// extern crate r2d2_sqlite;

// use std::io;
// use std::io::prelude::*;
// use r2d2::PooledConnection;
// use r2d2_sqlite::SqliteConnectionManager;
// // use r2d2_sqlite::rusqlite::types::Value;
// use r2d2_sqlite::rusqlite::params;
// use askama::Template;
// mod database;

// #[derive(Template)]
// #[template(path = "tProfile.html")]
// struct LoginPageTemplate;

// fn main() {
//     let request_method = std::env::var("REQUEST_METHOD").unwrap_or_else(|_| String::from(""));

//     let pool = database::establish_connection().expect("Failed to establish connection");

//     if request_method == "POST" {
//         let mut input_data = String::new();
//         io::stdin().read_to_string(&mut input_data).expect("Failed to read input data");

//         let pairs: Vec<&str> = input_data.trim().split('&').collect();
//         let mut data = std::collections::HashMap::new();
//         for pair in pairs {
//             let parts: Vec<&str> = pair.split('=').collect();
//             if parts.len() == 2 {
//                 let key = parts[0];
//                 let value = parts[1];
//                 data.insert(key.to_string(), value.to_string());
//             }
//         }

//         let conn = pool.get().expect("Failed to get connection from pool");

//         // Perform user authentication
//         let username = data.get("username").unwrap();
//         let password = data.get("password").unwrap();

//         let mut stmt = conn
//             .prepare("SELECT * FROM users WHERE username = ?1 AND password = ?2")
//             .expect("Failed to prepare statement");

//         let mut rows = stmt
//             .query([params![&username, &password]])
//             .expect("Failed to execute query");

//         let authenticated = rows.next().is_ok();

//         println!("Content-Type: text/html\n\n");
//         if authenticated {
//             // Successful login
//             println!("Login successful!");
//         } else {
//             // Failed login
//             println!("Login failed. Invalid username or password.");
//         }

//         let template = LoginPageTemplate {};
//         println!("{}", template.render().unwrap());
//     } else {
//         // Handle GET request (render the login form)
//         println!("Content-Type: text/html\n\n");
//         let template = LoginPageTemplate {};
//         println!("{}", template.render().unwrap());
//     }
// }


fn main() {
    println!("Hello, world!");
}