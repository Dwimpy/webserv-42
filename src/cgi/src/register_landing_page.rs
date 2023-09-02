extern crate askama;
extern crate r2d2;

use std::collections::HashMap;
use std::fs::OpenOptions;
use std::io::prelude::*;
use askama::Template;
use std::env;
use std::io;

#[derive(Template)]
#[template(path = "tRegistration.html")]
struct TemplateData;

fn main() {
        println!("Content-Type: text/html\n\n");
        let template = TemplateData {};
        println!("{}", template.render().unwrap());
}

