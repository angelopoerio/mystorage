from driver import *
import pickle

TEST_KEY = "I_AM_A_TEST_KEY"
TEST_KEY2 = "I_AM_A_TEST_KEY2"

with MyStorage('localhost') as obj:  # use context
	for key in [TEST_KEY, TEST_KEY2]:
		obj.put(key, 15)
		print "written {0}:{1}".format(key, obj.get(key))

	print obj.list()

con = MyStorage('localhost').get_connection() # flat way
con.connect()
con.put('2_' + TEST_KEY, 20)
con.put('2_' + TEST_KEY2, 20)
print con.list()

obj = {'key_1': 1, 'key2': [7, 54, 99]}
con.put('object', pickle.dumps(obj).encode('base64'))
obj_str = con.get('object')
print pickle.loads(obj_str.decode('base64'))
con.quit()
