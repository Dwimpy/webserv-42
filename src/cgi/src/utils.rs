pub mod utils {
    use rusqlite::Connection;
    use rusqlite::Error;


    pub struct User {
        pub username: String,
        pub cookie: String,
        pub email: String,
        pub birthdate: String,
    }


    pub fn create_db() -> Result<(), Error> {
        let db_path = "mydb.sqlite";
        let conn = Connection::open(db_path)?;

        conn.execute(
            "CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT NOT NULL UNIQUE,
            password TEXT NOT NULL,
            email TEXT,
            birthdate TEXT,
            pathtopicture TEXT,
            cookie TEXT
        )",
            [],
        )?;
        // conn.execute(
        //     "INSERT INTO users (username, password, cookie) VALUES (?, ?, ?)",
        //     [&"azer", &"azer123", &"azer=azer123"],
        // ).expect("Failed to insert data");
        // conn.execute(
        //     "INSERT INTO users (username, password, cookie) VALUES (?, ?, ?)",
        //     [&"danila", &"danila123", &"danila=danila123"],
        // ).expect("Failed to insert data");
        // conn.execute(
        //     "INSERT INTO users (username, password, cookie) VALUES (?, ?, ?)",
        //     [&"andrew", &"andrew123", &"andrew=andrew123"],
        // ).expect("Failed to insert data");

        Ok(())
    }

    pub fn get_user_by_cookie(cookie: &str) -> Result<User, rusqlite::Error> {
        let db_path = "mydb.sqlite";
        // Open a connection to your SQLite database file
        let conn = Connection::open(db_path)?;

        // Prepare an SQL query
        let mut stmt = conn.prepare("SELECT username, cookie, email, birthdate FROM users WHERE cookie = ?")?;

        // Execute the query, binding the username as a parameter
        let user_result = stmt.query_row(&[cookie], |row| {
            Ok(User {
                username: row.get(0)?,
                cookie: row.get(1)?,
                email: row.get(2)?,
                birthdate: row.get(3)?,
            })
        });

        match user_result {
            Ok(user) => Ok(user),
            Err(err) => Err(err),
        }
    }

    pub fn fetch_users_from_db() -> Result<(), Error> {
        let db_path = "mydb.sqlite";
        let conn = Connection::open(db_path)?;

        let query = "SELECT id, username, password, cookie FROM users";
        let mut stmt = conn.prepare(query)?;

        let rows = stmt.query_map([], |row| {
            Ok((
                row.get::<_, i64>(0)?,
                row.get::<_, String>(1)?,
                row.get::<_, String>(2)?,
                row.get::<_, String>(3)?,
            ))
        })?;

        // Iterate through the results and print all data
        for row in rows {
            let row = row?;
            let id: i64 = row.0;
            let username: String = row.1;
            let password: String = row.2;
            let cookie: String = row.3;

            println!("User ID: {}, Username: {}, Password: {}, Cookie: {}", id, username, password, cookie);
            println!("<br>");
        }

        Ok(())
    }

    pub fn check_user_credentials(username: &str, password: &str) -> Result<bool, Error> {
        let db_path = "mydb.sqlite";
        let conn = Connection::open(db_path)?;

        let mut stmt = conn.prepare("SELECT COUNT(*) FROM users WHERE username = ? AND password = ?")?;
        let count: i32 = stmt.query_row(&[username, password], |row| row.get(0))?;

        // If the count is 1, it means a matching user was found, and the credentials are correct

        // println!("count is {}, {}, {}", count, username, password);
        // Ok(count > 1)
        Ok(count == 1) // TODO: Uncomment this line and delete the line above

        // println!("<script> alert(\"{}, {}, {}\")</script>", count, username, password);
        // Ok(count > 1)
        // Ok(count == 1) // TODO: Uncomment this line and delete the line above

    }

    pub fn add_cookie_to_database(username: &str, cookie_value: &str) -> Result<(), Error> {
        let db_path = "mydb.sqlite";
        let conn = Connection::open(db_path)?;

        conn.execute(
            "UPDATE users SET cookie = ? WHERE username = ?",
            &[cookie_value, username],
        )?;

        Ok(())
    }

    pub fn check_cookie_in_database(cookie_value: &str) -> Result<bool, Error> {
        let db_path = "mydb.sqlite";
        let conn = Connection::open(db_path)?;

        let mut stmt = conn.prepare("SELECT COUNT(*) FROM users WHERE cookie = ?")?;
        let count: i32 = stmt.query_row(&[cookie_value], |row| row.get(0))?;

        // If the count is greater than 0, the cookie exists in the database
        Ok(count > 0)
    }

    pub fn user_exists(conn: &Connection, username: &str) -> Result<bool, Error> {
        let mut stmt = conn.prepare("SELECT COUNT(*) FROM users WHERE username = ?")?;
        let count: i64 = stmt.query_row(&[username], |row| row.get(0))?;
        Ok(count > 0)
    }

    pub fn add_user_to_db(username: &str, password: &str, birthdate: &str, email: &str) -> Result<i32, Error> {
        let db_path = "mydb.sqlite";
        let conn = Connection::open(db_path)?;

        if !user_exists(&conn, username)? {
            conn.execute(
                "INSERT INTO users (username, password, cookie, email, birthdate) VALUES (?, ?, ?, ?, ?)",
                &[username, password, "empty", email, birthdate],
            )?;
            println!("User '{}' added to the database.", username);
            Ok(1)
        } else {
            println!("User '{}' already exists in the database.", username);
            Ok(2)
        }

        // Ok(())
    }

}