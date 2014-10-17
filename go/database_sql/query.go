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
func main() {
	db, err := sql.Open("mysql", "nash:virginia@tcp(127.0.0.1:3306)/go_tutorial")
	if err != nil {
		log.Fatal(err)
	}
	defer db.Close()

	rows, err := db.Query("SELECT id, title FROM todo")
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

	err = rows.Err()
	if err != nil {
		log.Fatal(err)
	}
}
