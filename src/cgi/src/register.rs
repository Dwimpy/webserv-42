extern crate askama;
extern crate r2d2;
extern crate r2d2_sqlite;
extern crate rusqlite;

use rusqlite::Connection;
use std::io::{self, Read};
use rusqlite::Error;
use std::env;
use askama::Template;

mod utils;
use utils::utils::{add_cookie_to_database, check_cookie_in_database, check_user_credentials, create_db, fetch_users_from_db, add_user_to_db, user_exists};


#[derive(Template)]
#[template(path = "tRegisterLandingPage.html")]
struct LandingPageTemplate;


fn main() -> Result<(), Error> {
    let cookie = env::var("Cookie").unwrap_or_else(|_| String::from(""));
    // let username = env::var("USERNAME").unwrap_or_else(|_| String::from(""));
    // let password = env::var("PWD").unwrap_or_else(|_| String::from(""));
    let mut input = String::new();
    io::stdin().read_line(&mut input).expect("Failed to read line");
    println!("Input is: {}", input);

    let pairs: Vec<(&str, &str)> = input
        .split('&')
        .map(|pair| {
            let mut split = pair.splitn(2, '=');
            let key = split.next().unwrap_or("");
            let value = split.next().unwrap_or("");
            (key, value)
        })
        .collect();

    // Initialize username and password variables
    let mut username = "";
    let mut password = "";

    // Iterate through the pairs and extract values
    for (key, value) in &pairs {
        match *key {
            "username" => username = *value,
            "password" => password = *value,
            _ => (),
        }
    }

    // Print the username and password
    // println!("Username: {}", username);
    // println!("Password: {}", password);

    return match add_user_to_db(&username, &password) {
        Ok(_) => {
            add_cookie_to_database(&username, &cookie).expect("Failed to add cookie to db");
            // println!("User added to db");
            let template = LandingPageTemplate {};
            println!("{}", template.render().unwrap());
            Ok(())
        }
        Err(err) => {
            eprintln!("Error adding user to db: {}", err);
            Ok(())
        }
    }
}


