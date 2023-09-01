
extern crate r2d2;
extern crate r2d2_sqlite;

use r2d2::Pool;
use database::r2d2_sqlite::SqliteConnectionManager;

pub fn establish_connection() -> Result<Pool<SqliteConnectionManager>, r2d2::Error> {
    let manager = SqliteConnectionManager::file("users.db");
    Pool::new(manager)
}
