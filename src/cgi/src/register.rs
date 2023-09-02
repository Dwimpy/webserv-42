extern crate askama;
extern crate r2d2;
extern crate r2d2_sqlite;
extern crate rusqlite;

use rusqlite::Connection;
use std::io::{self, Read};
use rusqlite::Error;
use std::env;
use askama::Template;


#[derive(Template)]
#[template(path = "tRegistration.html")]
struct TemplateData;
struct RegisterTemplate {
    file_contents: String,
}




fn main() -> Result<(), Error> {

        // Get the value of the "Cookie" environment variable
        // let cookie = env::var("Cookie").unwrap_or_else(|_| String::from(""));
    let cookie = match env::var("Cookie") {
        Ok(value) => value,
        Err(_) => String::from(""),
    };

    // Check if the cookie value matches "user=cookie"
    if cookie == "user:cookie" {
        println!("Cookie matches the expected value.{}.", cookie);
        //     access the cookie data and check if the user is logged in
        //     //that's curious! maybe have a cookie field in a db which will get a cookie connection after log in
        //     // needs to be cleared every time we end the session(optional) needs to be changed every log in (also check if this cookie value exists for someone else, throw an error then)
        //     //if he is then we retrieve and display his data
        //     //if not then we strongly recommend him to log in
        //     //he enters his credential and we compare them to db
        //     //if the user exists we retrieve and display his profile
        //     //if not we throw an error and suggest him to register
        //     //registration form
        //     //login password name date of birth
        //     //after that we save the data and redirect to login
        //     //process loops
    } else {
        println!("Cookie does not match the expected value.{}.", cookie);
    }

    println!("<br>");



    // Specify the database file path (e.g., "mydb.sqlite")
    let db_path = "mydb.sqlite";

    // Open a connection to the database
    let conn = Connection::open(db_path)?;
    // println!("HEHHEHHEHEHEE<br>");
    // Create a table (e.g., "users")
    conn.execute(
        "CREATE TABLE IF NOT EXISTS users (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                username TEXT NOT NULL,
                password TEXT NOT NULL,
                cookie TEXT
            )",
        [],
    )
        .expect("Failed to create table");

    // conn.execute(
    //     "INSERT INTO users (username, password, cookie) VALUES (?, ?, ?)",
    //     [&"user2", &"password1", &"cookieValue"],
    // )
    //     .expect("Failed to insert data");

    // Execute a SELECT query to retrieve all rows from the "users" table
    let query = "SELECT id, username, password, cookie FROM users";
    let mut stmt = conn.prepare(query)?;
    let rows = stmt.query_map([], |row| {
        Ok((
            row.get::<_, i64>(0)?,
            row.get::<_, String>(1)?,
            row.get::<_, String>(2)?,
            row.get::<_, String>(3)?,
        ))
    })?;

    // Iterate through the results and print all data
    for row in rows {
        let row = row?;
        let id: i64 = row.0;
        let username: String = row.1;
        let password: String = row.2;
        let cookie: String = row.3;

        println!("User ID: {}, Username: {}, Password: {}, Cookie: {}", id, username, password, cookie);
        println!("<br>");
    }

    let mut input_data = String::new();

    io::stdin().read_to_string(&mut input_data).expect("Failed to read input data");
    println!("{}", input_data);
    println!("<br>");

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

