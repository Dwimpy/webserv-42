extern crate askama;
extern crate r2d2;



use std::collections::HashMap;
use std::fs::OpenOptions;
use std::io::prelude::*;
use askama::Template;
mod database;
use std::env;
use std::io;

#[derive(Template)]
#[template(path = "tRegisterLandingPage.html")]
struct TemplateData;

fn main() {
    let request_method = env::var("REQUEST_METHOD").unwrap_or_else(|_| String::from(""));
    if request_method == "POST" {
        let mut input_data = String::new();
        io::stdin().read_to_string(&mut input_data).expect("Failed to read input data");

        let pairs: Vec<&str> = input_data.trim().split('&').collect();
        let mut data = HashMap::new();

        let mut file = OpenOptions::new()
            .create(true)
            .append(true)
            .open("users.txt")
            .expect("Failed to open users.txt");

        for pair in pairs
        {
            let parts: Vec<&str> = pair.split('=').collect();
            if parts.len() == 2
            {
                let key = parts[0];
                let value = parts[1];
                data.insert(key.to_string(), value.to_string());
                let _ = writeln!(file, "{}: {}", key, value);
            }
            println!("Content-Type: text/html\n\n");
        }
        let template = TemplateData {};
        println!("{}", template.render().unwrap());
    } else {
        println!("Content-Type: text/html\n\n");
        let template = TemplateData {};
        println!("{}", template.render().unwrap());
    }
}

