import pymongo
from pymongo import Connection

connection = Connection()

# Удалить БД, если она существует
connection.drop_database("server_database")

# Выбираем БД
db = connection["server_database"]

# Удалить коллекцию
db.drop_collection('users')

# Добавление документов в колекцию 'users'
def insert_user_data(login, hash_sz):
	db.users.save( { 'username':login, 'hash':hash_sz } )

def delete_user_data(login):
	db.users.remove( {'username':login} )

def update_username(login, new_login):
	db.users.update( {'username':login}, {"$set" : {'username':new_login} } )
	
def update_hash(login, new_hash):
	db.users.update( {'username':login}, {"$set" : {'hash':new_hash} } )
	

# Получить все документы
def print_list(db):
	for user in db.users.find():
		print (user)


#examples	
insert_user_data(1, 12345)
insert_user_data(2, 12345)
insert_user_data(3, 12345)

print_list(db)

delete_user_data(1)
print_list(db)

update_username(2, 5)
update_hash(3, 11111)
print_list(db)
