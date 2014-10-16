package main

import (
	"database/sql"
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

	stmt, err := db.Prepare("INSERT INTO todo (title) VALUES (?)")
	if err != nil {
		log.Fatal(err)
	}
	defer stmt.Close()

	_, err = stmt.Exec("go to the doctor")
	if err != nil {
		log.Fatal(err)
	}
}
