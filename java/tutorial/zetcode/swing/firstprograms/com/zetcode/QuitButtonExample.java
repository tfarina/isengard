// http://zetcode.com/tutorials/javaswingtutorial/firstprograms/

package com.zetcode;

import java.awt.Container;
import java.awt.EventQueue;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.GroupLayout;
import javax.swing.JButton;
import javax.swing.JFrame;

public class QuitButtonExample extends JFrame {
  public QuitButtonExample() {
    Container pane = getContentPane();
    GroupLayout gl = new GroupLayout(pane);
    pane.setLayout(gl);

    JButton quitButton = new JButton("Quit");

    quitButton.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent event) {
        System.exit(0);
      }
    });

    gl.setAutoCreateContainerGaps(true);
    gl.setHorizontalGroup(gl.createSequentialGroup().addComponent(quitButton));
    gl.setVerticalGroup(gl.createSequentialGroup().addComponent(quitButton));

    pack();

    setTitle("Quit button example");
    setSize(800, 600);
    setLocationRelativeTo(null);
    setDefaultCloseOperation(EXIT_ON_CLOSE);
  }

  public static void main(String[] args) {
    EventQueue.invokeLater(new Runnable() {
      @Override
      public void run() {
        QuitButtonExample ex = new QuitButtonExample();
        ex.setVisible(true);
      }
    });
  }
}
