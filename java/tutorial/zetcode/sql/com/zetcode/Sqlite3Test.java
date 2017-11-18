// https://bitbucket.org/xerial/sqlite-jdbc

package com.zetcode;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

public class Sqlite3Test {
  public static void main(String[] args) throws ClassNotFoundException {
    // load the sqlite-JDBC driver using the current class loader
    Class.forName("org.sqlite.JDBC");
    Connection connection = null;
    try {
      connection = DriverManager.getConnection("jdbc:sqlite:sample.db");
      Statement statement = connection.createStatement();
      statement.setQueryTimeout(30);

      statement.executeUpdate("DROP TABLE IF EXISTS person");
      statement.executeUpdate("CREATE TABLE person (id integer, name string)");
      statement.executeUpdate("INSERT INTO person VALUES(1, 'leo')");
      statement.executeUpdate("INSERT INTO person VALUES(2, 'yui')");
      ResultSet rs = statement.executeQuery("SELECT * FROM person");

      while (rs.next()) {
        System.out.println("name = " + rs.getString("name"));
        System.out.println("id = " + rs.getInt("id"));
      }
    }
    catch (SQLException e) {
      System.err.println(e.getMessage());
    }
    finally {
      try {
        if (connection != null)
          connection.close();
      }
      catch(SQLException e) {
        System.err.println(e);
      }
    }
  }
}
