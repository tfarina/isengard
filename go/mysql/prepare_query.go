package main

import (
	"database/sql"
	"fmt"
	"log"

	_ "github.com/go-sql-driver/mysql"
)

//
// http://go-database-sql.org/retrieving.html
//
// Preparing Queries:
//
func main() {
	db, err := sql.Open("mysql", "nash:virginia@tcp(127.0.0.1:3306)/go_tutorial")
	if err != nil {
		log.Fatal(err)
	}
	defer db.Close()

	stmt, err := db.Prepare("SELECT id, title FROM todo WHERE id = ?")
	if err != nil {
		log.Fatal(err)
	}
	defer stmt.Close()

	rows, err := stmt.Query(1)
	if err != nil {
		log.Fatal(err)
	}
	defer rows.Close()

	for rows.Next() {
		var id int
		var title string
		err := rows.Scan(&id, &title)
		if err != nil {
			log.Fatal(err)
		}
		fmt.Println(id, title)
	}

	if err = rows.Err(); err != nil {
		log.Fatal(err)
	}
}
