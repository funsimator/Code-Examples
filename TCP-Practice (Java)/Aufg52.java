package smf;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.Socket;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextArea;
import javax.swing.JTextField;

// Simple Chat-Client using TCP 

public class Aufg52 extends JFrame{
	private static final long serialVersionUID = 1L;
	public static void main(String[] args) {
		Aufg52 foo = new Aufg52();
	}
	JTextField Textbox = new JTextField();
	JTextArea m_usernames = new JTextArea();
	JButton button = new JButton("send");
	JTextArea msgLog = new JTextArea();
	Socket socket;
	BufferedReader in;
	BufferedWriter out;
	
	Aufg52(){
		super("Toller Titel!");
		setDefaultCloseOperation(EXIT_ON_CLOSE);
		setLayout(new BorderLayout());
		add(msgLog, BorderLayout.WEST);
		msgLog.setPreferredSize(new Dimension(500,300));
		JPanel bottom = new JPanel();
		bottom.setLayout(new BorderLayout());
		bottom.add(Textbox, BorderLayout.CENTER);
		bottom.add(button,BorderLayout.EAST);
		add(m_usernames,BorderLayout.EAST);
		m_usernames.setPreferredSize(new Dimension(200,300));
		m_usernames.setBackground(Color.yellow);
		add(bottom,BorderLayout.SOUTH);
		pack();
		setVisible(true);
		
		String server = (String)(JOptionPane.showInputDialog(null,"Please enter your chat server","ChatClient", JOptionPane.QUESTION_MESSAGE,null,null,"localhost"));		
		if(server==null||server.length()==0)return;
		String user = (String)(JOptionPane.showInputDialog(null,"Please enter your name","ChatClient", JOptionPane.QUESTION_MESSAGE,null,null,"username"));
		if(user==null||user.length()==0)return;
		
		try {
			socket = new Socket(server,4711);
			in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
			out = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));
			out.write(user+"\n");
			out.flush();
			
			while((user=in.readLine()) != null ) {
				if(!user.equals("\t\t"))
						break;
				user = (String)(JOptionPane.showInputDialog(null,"Username was Empty or already Taken","ChatClient", JOptionPane.QUESTION_MESSAGE,null,null,"username"));
				out.write(user+"\n");
				out.flush();
			}
			
			//Add Button Event
			button.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e) {
					try {			
						String msg = Textbox.getText()+"\n";
						out.write(msg);
						out.flush();
						Textbox.setText(null);
					} catch (IOException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					}
				}
			});

			while(true) {
				ReceiveMsg();
			}
			
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		
	}	
	
	protected void finalize()  
	{  
	try {
		socket.close();
		System.out.println("Object is destroyed by the Garbage Collector");
	} catch (IOException e) {
		// TODO Auto-generated catch block
		e.printStackTrace();
	}  
	} 
	
	void ReceiveUsers(String first) {
		m_usernames.setText(null);
		m_usernames.append(first + "\n");
		String user;
		try {
			while (!(user = in.readLine()).isEmpty()) {
				m_usernames.append(user + "\n");
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}
	
	void ReceiveMsg() {
		String msg;
		try {
			if(in.ready()) {
				while ((msg = in.readLine()) != null) {
					if(msg.contains(":")||msg.contentEquals("Welcome"))
					msgLog.append(msg + "\n");
					else
						ReceiveUsers(msg);	
				}	
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}
	
}
