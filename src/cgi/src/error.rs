extern crate askama;
extern crate r2d2;
extern crate r2d2_sqlite;
extern crate rusqlite;

use rusqlite::Error;
use std::env;
use askama::Template;


#[derive(Template)]
#[template(path = "tError.html")]
struct ErrorTemplate {
    error_code: String,
    error_message: String,
}


fn main() -> Result<(), Error> {

    let error_msg = env::var("Error_msg").unwrap_or_else(|_| String::from(""));
    let error_code = env::var("Error_code").unwrap_or_else(|_| String::from(""));

    let template = ErrorTemplate {
        error_code : error_code.to_string(),
        error_message : error_msg.to_string(),
    };
    println!("{}", template.render().unwrap());

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
