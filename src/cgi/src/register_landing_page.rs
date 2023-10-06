extern crate askama;
extern crate r2d2;

use askama::Template;

#[derive(Template)]
#[template(path = "tRegistration.html")]
struct TemplateData{}

fn main() {
        let template = TemplateData {};
        println!("{}", template.render().unwrap());
}

