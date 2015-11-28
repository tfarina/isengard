package main

import (
	"database/sql"
	"log"

	_ "github.com/go-sql-driver/mysql"
)

const (
	// Default database driver.
	DEFAULT_DRIVER = "mysql"
)

//
// http://go-database-sql.org/accessing.html
//
func main() {
	db, err := sql.Open(DEFAULT_DRIVER, "nash:virginia@tcp(127.0.0.1:3306)/go_tutorial")
	if err != nil {
		log.Fatal(err)
	}
	defer db.Close()

	err = db.Ping()
	if err != nil {
		log.Fatal(err)
	}
}
