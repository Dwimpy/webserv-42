extern crate askama;
extern crate r2d2;
extern crate r2d2_sqlite;
extern crate rusqlite;

use rusqlite::Error;
use askama::Template;
use std::time::Duration;
use std::thread::sleep;
use std::env;


mod utils;
use utils::utils::{add_cookie_to_database, check_cookie_in_database, check_user_credentials, create_db, fetch_users_from_db};

#[derive(Template)]
#[template(path = "tProfile.html")]
struct ProfileTemplate;

#[derive(Template)]
#[template(path = "tLogin.html")]
struct LoginTemplate;
fn main() -> Result<(), Error>{

    let cookie = env::var("Cookie").unwrap_or_else(|_| String::from(""));
    let username = env::var("USERNAME").unwrap_or_else(|_| String::from(""));
    let password = env::var("PWD").unwrap_or_else(|_| String::from(""));
    println!("cookie: {}<br>", cookie);
    println!("username: {}<br>", username);
    println!("pwd: {}<br>", password);

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
            let cookie_value = "cookieValue"; // TODO: Assign cookie value
            add_cookie_to_database(&username, &cookie_value).expect("Failed to add cookie to db");
            let template = ProfileTemplate {};
            println!("{}", template.render().unwrap());
            return Ok(());
        }
        Ok(false) => {
            println!("Credentials are incorrect. try again or register or fuck off");
            let template = LoginTemplate {};
            println!("{}", template.render().unwrap());
            return Ok(());
        }
        Err(err) => {
            eprintln!("Error checking credentials: {}", err);
            return Ok(());
        }
    }
}
