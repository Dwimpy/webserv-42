extern crate askama;
extern crate r2d2;
extern crate r2d2_sqlite;
extern crate rusqlite;

use rusqlite::Error;
use std::env;
use askama::Template;

mod utils;
use utils::utils::{check_cookie_in_database, create_db, fetch_users_from_db, get_user_by_cookie};

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
#[derive(Template)]
#[template(path = "tRegistration.html")]
struct RegistrationTemplate;


fn main() -> Result<(), Error> {

    let cookie = env::var("Cookie").unwrap_or_else(|_| String::from(""));
    // let username = env::var("USERNAME").unwrap_or_else(|_| String::from(""));
    // let password = env::var("PWD").unwrap_or_else(|_| String::from(""));
    // println!("cookie: {}<br>", cookie);
    // println!("username: {}<br>", username);
    // println!("pwd: {}<br>", password);

    create_db().expect("Failed to create db");
    println!("<br>------- DB --------- <br>");
    fetch_users_from_db().expect("Error fetching users: {}<br>");
    println!("------- END --------- <br>");

    match check_cookie_in_database(&cookie) {
        /*
            Has Cookies:
            If the user has a session cookie,
                Then the server recognizes the user and allows access to their profile.
                Then the server retrieves the user's data (like their name and date of birth) from the database using the session cookie.
                Then it displays this data on the user's profile page.
            If the user does not have a session cookie,
                Then the server prompts the user to log in or register to access their profile.
        */
        Ok(true) => {

            match get_user_by_cookie(&cookie) {
                Ok(user) => {
                    let template = ProfileTemplate {
                        username : user.username,
                        email : user.email,
                        birthdate : user.birthdate,
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
            // println!("No Cookies, redirecting to login page<br>");
            let template = LoginTemplate {};
            println!("{}", template.render().unwrap());
            return Ok(());
        }
        Err(err) => eprintln!("Error checking cookie: {}", err),
    }

/*
2. Registration:
    If the user clicks on "Register" and provides the required information (like username, password, name, and date of birth),
        Then the server checks if the provided information is valid and not already used by another user.
    If the information is valid and unique,
        Then the server creates a new user profile in its database.
        Then it generates a session cookie for the newly registered user.
        Then it attaches this session cookie to the user's browser.
        Then the user is now considered "registered and logged in."
    If the information is invalid or already used,
        Then the server rejects the registration attempt, and the user is asked to correct the information or choose a different username.
*/


    Ok(())
}
