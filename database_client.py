import sqlite3
import os

if not os.path.exists('users.db'):
	con = sqlite3.connect('users.db')	
	cur = con.cursor()
	cur.execute("create table users (sender, reciever, data, message)")
else:
	con = sqlite3.connect('users.db')	
	cur = con.cursor()

def add_in_table(send, recv, data, message):
	cur.execute("insert into users values (?, ?, ?, ?)", (send, recv, data, message))
	cur.execute("select * from users where sender=:sender and reciever=:reciever", {"sender":send, "reciever":recv, "data":data, "message":message} )
	return cur
	

send = "Yeltsin"
recv = "Gorbachev"
data = "5-12-2015"
mes = "hi"

cur = add_in_table(send, recv, data, mes)
con.commit()

print (cur.fetchone())
con.close()
