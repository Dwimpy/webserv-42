extern crate askama;
extern crate r2d2;

extern crate r2d2_sqlite;
extern crate rusqlite;

use r2d2::Pool;
use r2d2_sqlite::SqliteConnectionManager;
use rusqlite::params;
use rusqlite::Connection;



use std::collections::HashMap;
use std::io;
use std::io::prelude::*;
use std::env;
use askama::Template;
mod database;
use database::establish_connection;


#[derive(Template)]
#[template(path = "tRegisterPage.html")]
struct TemplateData;
struct RegisterTemplate {
    file_contents: String,
}

fn main() {
//     let request_method = env::var("REQUEST_METHOD").unwrap_or_else(|_| String::from(""));
//
//     let pool = establish_connection().expect("Failed to establish connection");
//
//     if request_method == "POST" {
//         let mut input_data = String::new();
//         io::stdin().read_to_string(&mut input_data).expect("Failed to read input data");
//
//         let pairs: Vec<&str> = input_data.trim().split('&').collect();
//         let mut data = HashMap::new();
//
//         // Form data parsing
//         for pair in pairs {
//             let parts: Vec<&str> = pair.split('=').collect();
//             if parts.len() == 2 {
//                 let key = parts[0];
//                 let value = parts[1];
//                 data.insert(key.to_string(), value.to_string());
//             }
//         }
//
//         // Insert user data into the database
//         let conn = pool.get().expect("Failed to get connection from pool");
//         conn.execute("INSERT INTO users (username, password) VALUES (?, ?)",
//                      &[data.get("username").unwrap(), data.get("password").unwrap()])
//             .expect("Failed to insert data");
//
//         println!("Content-Type: text/html\n\n");
//         let template = TemplateData {};
//         println!("{}", template.render().unwrap());
//     } else {
//         println!("Content-Type: text/html\n\n");
        let db_path = "mydb.sqlite";

        // Create a new database or open an existing one
        let conn = Connection::open(db_path).expect("Failed to open database");

        // Create a table (e.g., "users")
        conn.execute(
            "CREATE TABLE IF NOT EXISTS users (
                id INTEGER PRIMARY KEY,
                username TEXT NOT NULL,
                password TEXT NOT NULL
            )",
            [],
        )
        .expect("Failed to create table");

        println!("Database and table created successfully.");
        let template = TemplateData {};
        println!("{}", template.render().unwrap());
//     }
}

