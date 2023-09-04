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
struct LandingPageTemplate{
    message: String,
}


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
    let mut email = "";
    let mut birthdate = "";

    // Iterate through the pairs and extract values
    for (key, value) in &pairs {
        match *key {
            "username" => username = *value,
            "password" => password = *value,
            "email" =>  email = *value,
            "birthdate" =>  birthdate = *value,
            _ => (),
        }
    }

    // Print the username and password
    println!("Username: {}", username);
    println!("email: {}", email);
    println!("birthdate: {}", birthdate);
    println!("Password: {}", password);

    return match add_user_to_db(&username, &password, &birthdate, &email) {
        Ok(1) => {
            add_cookie_to_database(&username, &cookie).expect("Failed to add cookie to db");
            // println!("User added to db");
            let template = LandingPageTemplate {
                message: String::from("Alles gut! Enjoy!"),
            };
            println!("{}", template.render().unwrap());
            Ok(())
        }
        Ok(2) => {
            // add_cookie_to_database(&username, &cookie).expect("Failed to add cookie to db");
            // println!("User added to db");
            let template = LandingPageTemplate {
                message: String::from("FAIL! User already exists"),
            };
            println!("{}", template.render().unwrap());
            Ok(())
        }
        Ok(_) =>{
            let template = LandingPageTemplate {
                message: String::from("FAIL! User already exists"),
            };
            println!("{}", template.render().unwrap());
            Ok(())
        }
        Err(err) => {
            // let template = LandingPageTemplate {};
            // println!("{}", template.render().unwrap());
            eprintln!("Error adding user to db: {}", err);
            Ok(())
        }
    }
}


