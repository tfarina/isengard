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
	// TODO(tfarina): Create go_tutorial database and a user/pass for it.
	db, err := sql.Open("mysql", "pyuser:yellow46@tcp(127.0.0.1:3306)/python_tutorial")
	if err != nil {
		log.Fatal(err)
	}
	defer db.Close()

	err = db.Ping()
	if err != nil {
		log.Fatal(err)
	}
}
