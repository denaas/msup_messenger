import pymongo
from pymongo import Connection

connection = Connection()

connection.drop_database("server_database")


db = connection["server_database"]


db.drop_collection('users')


def insert_user_data(login, hash_sz):
	db.users.save( { 'username':login, 'hash':hash_sz } )

def delete_user_data(login):
	db.users.remove( {'username':login} )

def update_username(login, new_login):
	db.users.update( {'username':login}, {"$set" : {'username':new_login} } )
	
def update_hash(login, new_hash):
	db.users.update( {'username':login}, {"$set" : {'hash':new_hash} } )
	


def print_list(db):
	for user in db.users.find():
		print (user)



