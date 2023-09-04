#!/usr/local/bin/python3

import cgi
import os
import sqlite3
from jinja2 import Environment, FileSystemLoader

script_dir = os.path.dirname(os.path.abspath(__file__))
os.chdir(script_dir)


def save_image_to_db(username, image_filename):
    conn = sqlite3.connect("../users.db")
    cursor = conn.cursor()

    cursor.execute("UPDATE users SET pathtopicture = ? WHERE username = ?", (image_filename, username))
    conn.commit()
    conn.close()


def render_image_upload_form():
    env = Environment(loader=FileSystemLoader('../templates'))
    template = env.get_template('upload_form.html')

    print("Content-type: text/html\n")
    print(template.render())


def handle_image_upload():
    form = cgi.FieldStorage()

    username = "azer"

    image = form['image']

    if image.filename:
        user_image_directory = os.path.join("user_images", username)
        os.makedirs(user_image_directory, exist_ok=True)
        image_filename = os.path.join(user_image_directory, image.filename)

        with open(image_filename, 'wb') as image_file:
            image_file.write(image.file.read())

        save_image_to_db(username, image_filename)

        print("Content-type: text/html\n")
        print("<html><body>")
        print("<h1>Image Uploaded Successfully</h1>")
        print("<p><a href='profile.cgi'>Back to Profile</a></p>")
        print("</body></html>")
    else:
        print("Content-type: text/html\n")
        print("<html><body>")
        print("<h1>Error: No file uploaded</h1>")
        print("<p><a href='profile.cgi'>Back to Profile</a></p>")
        print("</body></html>")


if __name__ == "__main__":
    form = cgi.FieldStorage()
    print(form)

    if "image" in form:
        handle_image_upload()
    else:
        render_image_upload_form()
