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
	db.users.save( { 'login':login, 'hash':hash_sz } )

insert_user_data(1, 12345)


# Получить все документы
for user in db.users.find():
    print (user)
