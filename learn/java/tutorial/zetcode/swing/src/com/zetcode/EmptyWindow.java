// http://zetcode.com/tutorials/javaswingtutorial/firstprograms/

package com.zetcode;

import java.awt.EventQueue;
import javax.swing.JFrame;

public class EmptyWindow extends JFrame {
  public EmptyWindow() {
    setTitle("Simple example");
    setSize(800, 600);
    setLocationRelativeTo(null);
    setDefaultCloseOperation(EXIT_ON_CLOSE);
  }

  public static void main(String[] args) {
    EventQueue.invokeLater(new Runnable() {
      @Override
      public void run() {
        EmptyWindow ew = new EmptyWindow();
        ew.setVisible(true);
      }
    });
  }
}
