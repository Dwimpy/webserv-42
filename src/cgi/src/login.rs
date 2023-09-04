extern crate askama;
extern crate r2d2;
extern crate r2d2_sqlite;
extern crate rusqlite;

use rusqlite::Error;
use askama::Template;
use std::time::Duration;
use std::thread::sleep;
use std::env;

use std::io;
mod utils;
use utils::utils::{add_cookie_to_database, check_cookie_in_database, check_user_credentials, create_db, fetch_users_from_db, get_user_by_cookie};
use utils::utils::User;


#[derive(Template)]
#[template(path = "tProfile.html")]
struct ProfileTemplate {
    username: String,
    email: String,
    birthdate: String,
    cookie: String,
}

#[derive(Template)]
#[template(path = "tLogin.html")]
struct LoginTemplate;
fn main() -> Result<(), Error>{

    let cookie = env::var("Cookie").unwrap_or_else(|_| String::from(""));
    // println!("cookie: {}<br>", cookie);

    // let username = env::var("USERNAME").unwrap_or_else(|_| String::from(""));
    // let password = env::var("PWD").unwrap_or_else(|_| String::from(""));
    // println!("username: {}<br>", username);
    // println!("pwd: {}<br>", password);
    let mut input = String::new();
    io::stdin().read_line(&mut input).expect("Failed to read line");
    // println!("Input is: {}", input);

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


    match check_user_credentials(&username, &password) {
        /*
            Login:
            If the user enters their login credentials (username and password) and clicks "Login,"
                Then the server checks if the credentials match any in its database.
            If the credentials are correct,
                Then save the cookie in db"
                Then it attaches this session cookie to the user's browser.
                Then the user is considered "logged in."
            If the credentials are incorrect,
                Then the server rejects the login, and the user stays on the login page.
        */
        Ok(true) => {
            let cookie_value = cookie; // TODO: Assign cookie value
            add_cookie_to_database(&username, &cookie_value).expect("Failed to add cookie to db");

            match get_user_by_cookie(&cookie_value) {
                Ok(user) => {
                    let template = ProfileTemplate {
                        username: user.username,
                        email: user.email,
                        birthdate: user.birthdate,
                        cookie: user.cookie,
                    };
                    println!("{}", template.render().unwrap());
                    return Ok(());
                }
                Err(err) => {
                        eprintln!("Error: {:?}", err);
                    std::process::exit(1);
                    }
            }

        }
        Ok(false) => {
            println!("Please enter correct credentials.");
            let template = LoginTemplate {};
            println!("{}", template.render().unwrap());
            return Ok(());
        }
        Err(err) => {
            eprintln!("Error checking credentials: {}", err);
            let template = LoginTemplate {};
            println!("{}", template.render().unwrap());
            return Ok(());
        }
    }
}
