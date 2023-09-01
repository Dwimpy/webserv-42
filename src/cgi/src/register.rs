extern crate askama;
extern crate r2d2;

extern crate r2d2_sqlite;
extern crate rusqlite;

use r2d2::Pool;
use r2d2_sqlite::SqliteConnectionManager;
use rusqlite::params;
use rusqlite::Connection;

use rusqlite::Error;


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




fn main() -> Result<(), Error> {
    // Specify the database file path (e.g., "mydb.sqlite")
    let db_path = "mydb.sqlite";

    // Open a connection to the database
    let conn = Connection::open(db_path)?;
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


    conn.execute(
        "INSERT INTO users (username, password) VALUES (?, ?)",
        [&"user2", &"password1"],
    )
        .expect("Failed to insert data");

    // Execute a SELECT query to retrieve all rows from the "users" table
    let query = "SELECT id, username, password FROM users";
    let mut stmt = conn.prepare(query)?;
    let rows = stmt.query_map([], |row| {
        Ok((
            row.get::<_, i64>(0)?,
            row.get::<_, String>(1)?,
            row.get::<_, String>(2)?,
        ))
    })?;

    // Iterate through the results and print all data
    for row in rows {
        let row = row?;
        let id: i64 = row.0;
        let username: String = row.1;
        let password: String = row.2;

        println!("User ID: {}, Username: {}, Password: {}", id, username, password);
        println!("<br>");
    }

    println!("Database and table created successfully.");
    let template = TemplateData {};
    println!("{}", template.render().unwrap());

    Ok(())
}
// fn main() {
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
//         let db_path = "mydb.sqlite";

        // Create a new database or open an existing one
        // let conn = Connection::open(db_path).expect("Failed to open database");

        // Create a table (e.g., "users")
        // conn.execute(
        //     "CREATE TABLE IF NOT EXISTS users (
        //         id INTEGER PRIMARY KEY,
        //         username TEXT NOT NULL,
        //         password TEXT NOT NULL
        //     )",
        //     [],
        // )
        // .expect("Failed to create table");
        //
        //
        // conn.execute(
        //     "INSERT INTO users (username, password) VALUES (?, ?)",
        //     [&"user2", &"password1"],
        // )
        // .expect("Failed to insert data");
        //
        // let mut stmt = conn
        //     .prepare("SELECT id, username, password FROM users WHERE id = ?")
        //     .expect("Failed to prepare query");
        //
        // let user_id_to_find = 4;
        // let mut rows = stmt
        //     .query(&[&user_id_to_find])
        //     .expect("Failed to execute query");
        //
        // while let Some(row) = rows.next().expect("Failed to retrieve row") {
        //     let id: i64 = row.get(0).expect("Failed to get ID");
        //     let username: String = row.get(1).expect("Failed to get username");
        //     let password: String = row.get(2).expect("Failed to get password");
        //
        //     println!("User ID: {}, Username: {}, password {}", id, username, password);
        // }
    // let mut stmt = conn.prepare("SELECT id, username, password FROM users");
    // let mut rows = stmt.expect("failed query").query([]);

    // Iterate through the results and print the data
    // for row in rows {
    //     let row = row;
    //     let id: i64 = row.get(0);
    //     let username: String = row.get(1);
    //     let password: String = row.get(2);

        // println!("ID: {}, Username: {}, Password: {}", id, username, password);
    // }

    // Close the database connection
    //     conn.close().expect("Failed to close database connection");

        // println!("Database and table created successfully.");
        // let template = TemplateData {};
        // println!("{}", template.render().unwrap());
//     }
// }

