// http://www.cs.cf.ac.uk/Dave/HCI/HCI_Handout_CALLER/node167.html

package com.zetcode;

import java.awt.*;
import java.awt.event.*;
import java.util.*;
import java.sql.*;
import javax.swing.*;
import javax.swing.table.DefaultTableModel;

public class SimpleTableWithSqlite3 extends JFrame {
  private JTable table;
  private JScrollPane scrollPane;
  private JPanel panel;

  public SimpleTableWithSqlite3() {
    setTitle("Simple example");
    setSize(800, 600);
    setLocationRelativeTo(null);
    setDefaultCloseOperation(EXIT_ON_CLOSE);

    // load the sqlite-JDBC driver using the current class loader
    try {
    Class.forName("org.sqlite.JDBC");
    } catch (Exception e) {
    }
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

      ResultSetMetaData metaData = rs.getMetaData();

      Vector<String> columnNames = new Vector<String>();
      int columnCount = metaData.getColumnCount();
      for (int i = 1; i <= columnCount; i++) {
         columnNames.add(metaData.getColumnName(i));
      }

      Vector<Vector<Object>> data = new Vector<Vector<Object>>();
      while (rs.next()) {
        Vector<Object> vector = new Vector<Object>();
        for (int i = 1; i <= columnCount; i++) {
          vector.add(rs.getObject(i));
        }
        data.add(vector);
      }

      panel = new JPanel();
      panel.setLayout(new BorderLayout());
      getContentPane().add(panel);

      table = new JTable(new DefaultTableModel(data, columnNames));

      scrollPane = new JScrollPane(table);
      panel.add(scrollPane, BorderLayout.CENTER);
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

  public static void main(String[] args) {
    try {
      // http://docs.oracle.com/javase/tutorial/uiswing/lookandfeel/plaf.html
      UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
    } catch (Exception e) {
    }

    EventQueue.invokeLater(new Runnable() {
      @Override
      public void run() {
        SimpleTableWithSqlite3 ex = new SimpleTableWithSqlite3();
        ex.setVisible(true);
      }
    });
  }
}
