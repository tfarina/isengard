// http://www.cs.cf.ac.uk/Dave/HCI/HCI_Handout_CALLER/node167.html

package com.zetcode;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class SimpleTableExample extends JFrame {
  private JTable table;
  private JScrollPane scrollPane;
  private JPanel panel;

  public SimpleTableExample() {
    setTitle("Simple example");
    setSize(800, 600);
    setLocationRelativeTo(null);
    setDefaultCloseOperation(EXIT_ON_CLOSE);

    String columnNames[] = { "Column 1", "Column 2", "Column 3" };

    String dataValues[][] = {
            { "12", "13", "14" },
            { "21", "22", "23" },
            { "33", "34", "35" },
    };

    panel = new JPanel();
    panel.setLayout(new BorderLayout());
    getContentPane().add(panel);

    table = new JTable(dataValues, columnNames);

    scrollPane = new JScrollPane(table);
    panel.add(scrollPane, BorderLayout.CENTER);
  }

  public static void main(String[] args) {
    try {
      UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
    } catch (Exception e) {
    }

    EventQueue.invokeLater(new Runnable() {
      @Override
      public void run() {
        SimpleTableExample ex = new SimpleTableExample();
        ex.setVisible(true);
      }
    });
  }
}
