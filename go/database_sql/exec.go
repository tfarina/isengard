package main

import (
	"database/sql"
	"fmt"
	"log"

	_ "github.com/go-sql-driver/mysql"
)

//
// http://go-database-sql.org/accessing.html
//
func main() {
	db, err := sql.Open("mysql", "nash:virginia@tcp(127.0.0.1:3306)/go_tutorial")
	if err != nil {
		log.Fatal(err)
	}
	defer db.Close()

	err = db.Ping()
	if err != nil {
		log.Fatal(err)
	}

	_, err = db.Exec(
		"INSERT INTO todo (title) VALUES (?)", "wash my car")
	if err != nil {
		fmt.Errorf("Failed to write to database: %s", err)
	}
}
