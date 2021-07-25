import requests
import threading
import os

def simple_get_index(port: int) -> str:
	r = requests.get("http://localhost:" + str(port))
	if (r.status_code != 200):
		print(r.status_code)
		return "Bad status code"
	if (r.text != "Hello World !\n"):
		return "Bad Content"
	if (r.headers['Content-Length'] != "14"):
		return "Bad Content-Length"
	return ""

def get_index_two_ports(port: int) -> str:
	for i in range(10):
		r = requests.get("http://localhost:" + str(port))
		if (r.status_code != 200):
			print(r.status_code)
			return "Bad status code"
		if (r.text != "Hello World !\n"):
			return "Bad Content"
		if (r.headers['Content-Length'] != "14"):
			return "Bad Content-Length"
		r = requests.get("http://localhost:" + str(port + 1))
		if (r.status_code != 200):
			return "Bad status code"
		if (r.text != "Hello World !\n"):
			return "Bad Content"
		if (r.headers['Content-Length'] != "14"):
			return "Bad Content-Length"
	return ""


def get_forbidden_dir(port: int) -> str:
	r = requests.get("http://localhost:" + str(port) + "/forbidden")
	if (r.status_code != 403):
		print(r.status_code)
		return "Bad status code"
	return ""

def get_404(port: int) -> str:
	r = requests.get("http://localhost:" + str(port) + "/nonsense")
	if (r.status_code != 404):
		print(r.status_code)
		return "Bad status code"
	if (r.text.find("This is a 404 error page") == -1):
		return "Bad Content"
	return ""

def get_autoindex_subdir(port: int) -> str:
	r = requests.get("http://localhost:" + str(port) + "/auto")
	if (r.status_code != 200):
		print(r.status_code)
		return "Bad status code"
	if (r.text.find("Index of /auto") == -1):
		return "Bad Content"
	return ""

def fifty_get_root(port: int) -> str:
	for i in range(50):
		r = requests.get("http://localhost:" + str(port))
		if (r.status_code != 200):
			print(r.status_code)
			return "Bad status code"
		if (r.text != "Hello World !\n"):
			return "Bad Content"
		if (r.headers['Content-Length'] != "14"):
			return "Bad Content-Length"
	return ""

def wrong_method(port: int) -> str:
	r = requests.post("http://localhost:" + str(port))
	if (r.status_code != 405):
		print(r.status_code)
		return "Bad status code"
	return ""

def simple_post(port:int) -> str:
	payload = "a"
	r = requests.post("http://localhost:" + str(port) + "/post/", data=payload)
	if (r.status_code != 200):
		print(r.status_code)
		return "Bad status code."
	return ""

def post_size_0(port:int) -> str:
	payload = ""
	r = requests.post("http://localhost:" + str(port) + "/post/", data=payload)
	if (r.status_code != 200):
		print(r.status_code)
		return "Bad status code."
	return ""

def post_too_big(port:int) -> str:
	payload = "a" * 1025
	r = requests.post("http://localhost:" + str(port) + "/post/", data=payload)
	if (r.status_code != 413):
		print(r.status_code)
		return "Bad status code."
	return ""

def post_with_upload(port:int) -> str:
	payload = "fname=test_fname&lname=test_lname"
	r = requests.post("http://localhost:" + str(port) + "/post_upload/index.html", data=payload)
	if (r.status_code != 200 and r.status_code != 201):
		print(r.status_code)
		return "Bad status code."
	filename = "./upload/index.html"
	with open(filename) as f:
		content = f.readlines()
	if (content[0] != "fname=test_fname&lname=test_lname"):
		return "upload_file has wrong text"
	return ""

def delete(port: int) -> str:
	filename = "/delete_folder/index.html"
	if (os.path.isfile("." + filename)):
		print("File exists")
	r = requests.delete("http://localhost:" + str(port) + filename)
	if (r.status_code != 204):
		print(r.status_code)
		return "Bad status code for DELETE."
	if (os.path.isfile("." + filename)):
		return "File still exists"
	return ""

def delete_already_deleted(port: int) -> str:
	filename = "/delete_folder/index.html"
	if (os.path.isfile("." + filename) == False):
		print("File doesn't exist")
	r = requests.delete("http://localhost:" + str(port) + filename)
	if (r.status_code != 404):
		return "Bad status code for DELETE."
	return ""

def redirect_get_ok(port: int) -> str:
	r = requests.get("http://localhost:" + str(port) + "/redirect_me/please")
	if (r.status_code != 200):
		print(r.status_code)
		return "Bad status code"
	if (r.text != "redirection index :)"):
		print(r.text)
		return "Bad Content"
	return ""

def redirect_get_non_existing(port: int) -> str:
	r = requests.get("http://localhost:" + str(port) + "/redirect_me/wrong")
	if (r.status_code != 404):
		print(r.status_code)
		return "Bad status code"
	return ""

def server_name(port: int) -> str:
	r = requests.get("http://webserv:" + str(port))
	if (r.status_code != 200):
		return "Bad status code"
	if (r.text != "Hello World !\n"):
		return "Bad Content"
	if (r.headers['Content-Length'] != "14"):
		return "Bad Content-Length"
	return ""

def get_loc_root(port: int) -> str:
	r = requests.get("http://localhost:" + str(port) + "/use_location_root/")
	if (r.status_code != 200):
		print(r.status_code)
		return "Bad status code"
	if (r.text != "Hello World !\n"):
		return "Bad Content"
	if (r.headers['Content-Length'] != "14"):
		return "Bad Content-Length"
	return ""

def post_loc_root(port: int) -> str:
	payload = "args=oui&args2=non"
	r = requests.post("http://localhost:" + str(port) + "/use_location_root/", data=payload)
	if (r.status_code != 200):
		print(r.status_code)
		return "Bad status code"
	if (r.text != "Hello World !\n"):
		return "Bad Content"
	if (r.headers['Content-Length'] != "14"):
		return "Bad Content-Length"
	return ""

def cgi_tester_get(port: int) -> str:
	r = requests.get("http://localhost:" + str(port) + "/cgi/file.tester")
	if (r.status_code != 200):
		return "Bad status code"
	return ""

def cgi_tester_post(port: int) -> str:
	payload = "args=oui&args2=non"
	r = requests.post("http://localhost:" + str(port) + "/cgi/file.tester", data=payload)
	if (r.status_code != 200):
		return "Bad status code"
	if (r.text.find("ARGS=OUI&ARGS2=NON") == -1):
		return "Bad content"
	if (len(r.text) != 76):
		print(len(r.text))
		return "Bad content"
	return ""

def cgi_tester_get_loc_root(port: int) -> str:
	r = requests.get("http://localhost:" + str(port) + "/use_location_root_cgi/file.tester")
	if (r.status_code != 200):
		return "Bad status code"
	return ""

def cgi_tester_post_loc_root(port: int) -> str:
	payload = "args=oui&args2=non"
	r = requests.post("http://localhost:" + str(port) + "/use_location_root_cgi/file.tester", data=payload)
	if (r.status_code != 200):
		return "Bad status code"
	if (r.text.find("ARGS=OUI&ARGS2=NON") == -1):
		return "Bad content"
	if (len(r.text) != 76):
		print(len(r.text))
		return "Bad content"
	return ""

def cgi_tester_get(port: int) -> str:
	r = requests.get("http://localhost:" + str(port) + "/cgi/file.tester")
	if (r.status_code != 200):
		return "Bad status code"
	return ""

def cgi_tester_post(port: int) -> str:
	payload = "args=oui&args2=non"
	r = requests.post("http://localhost:" + str(port) + "/cgi/file.tester", data=payload)
	if (r.status_code != 200):
		return "Bad status code"
	if (r.text.find("ARGS=OUI&ARGS2=NON") == -1):
		return "Bad content"
	if (len(r.text) != 76):
		print(len(r.text))
		return "Bad content"
	return ""

def gen():
	var1 = "x=FOO&"
	var2 = "y=FOO123465789"
	x = var1.encode('utf8')
	y = var2.encode('utf8')
	yield x
	yield y

def chunked_post_no_upload(port: int) -> str:
	headers = {'Connection': 'keep-alive', 'Content-Type': 'application/x-www-form-urlencoded',
           'Transfer-Encoding': 'chunked'}
	r = requests.post("http://localhost:" + str(port) + "/cgi/file.tester", data=gen(), headers=headers)
	if (r.status_code != 200 and r.status_code != 201):
		return "Bad Status Code"
	if (len(r.text) != 78):
		print(len(r.text))
		return "Bad Length Response Body"
	return ""

def gen2():
	var1 = "a" * 250
	var2 = "b" * 250
	x = var1.encode('utf8')
	y = var2.encode('utf8')
	yield x
	yield y

def chunked_post_no_upload_size_500(port: int) -> str:
	headers = {'Connection': 'keep-alive', 'Content-Type': 'application/x-www-form-urlencoded',
           'Transfer-Encoding': 'chunked'}
	r = requests.post("http://localhost:" + str(port) + "/cgi/file.tester", data=gen2(), headers=headers)
	if (r.status_code != 200 and r.status_code != 201):
		return "Bad Status Code"
	if (len(r.text) != 558):
		print(len(r.text))
		return "Bad Length Response Body"
	return ""

def gen3():
	var1 = "a" * 100000
	var2 = "b" * 100000
	var3 = "c" * 100000
	var4 = "d" * 100000
	var5 = "e" * 100000
	v = var1.encode('utf8')
	w = var2.encode('utf8')
	x = var3.encode('utf8')
	y = var4.encode('utf8')
	z = var5.encode('utf8')
	yield v
	yield w
	yield x
	yield y
	yield z

def chunked_post_no_upload_size_500k(port: int) -> str:
	headers = {'Connection': 'keep-alive', 'Content-Type': 'application/x-www-form-urlencoded',
           'Transfer-Encoding': 'chunked'}
	r = requests.post("http://localhost:" + str(port) + "/cgi/file.tester", data=gen3(), headers=headers)
	if (r.status_code != 200 and r.status_code != 201):
		return "Bad Status Code"
	if (len(r.text) != 500058):
		print(len(r.text))
		return "Bad Length Response Body"
	return ""


def gen():
	var1 = "x=FOO&"
	var2 = "y=FOO123465789"
	x = var1.encode('utf8')
	y = var2.encode('utf8')
	yield x
	yield y

def chunked_post_upload(port: int) -> str:
	headers = {'Connection': 'keep-alive', 'Content-Type': 'application/x-www-form-urlencoded',
           'Transfer-Encoding': 'chunked'}
	r = requests.post("http://localhost:" + str(port) + "/post_upload/another_file.tester", data=gen(), headers=headers)
	if (r.status_code != 200 and r.status_code != 201):
		return "Bad Status Code"
	if (len(r.text) != 78):
		print(r.text)
		print(len(r.text))
		return "Bad Length Response Body"
	filename = "./upload/another_file.tester"
	with open(filename) as f:
		content = f.readlines()
	if (len(content[0]) != 20):
		print(len(content[0]))
		return "upload_file has wrong text"
	return ""

def gen2():
	var1 = "a" * 250
	var2 = "b" * 250
	x = var1.encode('utf8')
	y = var2.encode('utf8')
	yield x
	yield y

def chunked_post_size_500(port: int) -> str:
	headers = {'Connection': 'keep-alive', 'Content-Type': 'application/x-www-form-urlencoded',
           'Transfer-Encoding': 'chunked'}
	r = requests.post("http://localhost:" + str(port) + "/post_upload/another_file.tester", data=gen2(), headers=headers)
	if (r.status_code != 200 and r.status_code != 201):
		return "Bad Status Code"
	if (len(r.text) != 558):
		print(len(r.text))
		return "Bad Length Response Body"
	filename = "./upload/another_file.tester"
	with open(filename) as f:
		content = f.readlines()
	if (len(content[0]) != 500):
		print(len(content[0]))
		return "upload_file has wrong text"
	return ""

def gen3():
	var1 = "a" * 100000
	var2 = "b" * 100000
	var3 = "c" * 100000
	var4 = "d" * 100000
	var5 = "e" * 100000
	v = var1.encode('utf8')
	w = var2.encode('utf8')
	x = var3.encode('utf8')
	y = var4.encode('utf8')
	z = var5.encode('utf8')
	yield v
	yield w
	yield x
	yield y
	yield z

def chunked_post_size_500k(port: int) -> str:
	headers = {'Connection': 'keep-alive', 'Content-Type': 'application/x-www-form-urlencoded',
           'Transfer-Encoding': 'chunked'}
	r = requests.post("http://localhost:" + str(port) + "/post_upload/another_file.tester", data=gen3(), headers=headers)
	if (r.status_code != 200 and r.status_code != 201):
		return "Bad Status Code"
	if (len(r.text) != 500058):
		print(len(r.text))
		return "Bad Length Response Body"
	filename = "./upload/another_file.tester"
	with open(filename) as f:
		content = f.readlines()
	if (len(content[0]) != 500000):
		print(len(content[0]))
		return "upload_file has wrong text"
	return ""


""" BONUS METHODS  """

def simple_head_index(port: int) -> str:
	r = requests.head("http://localhost:" + str(port))
	if (r.status_code != 200):
		print(r.status_code)
		return "Bad status code"
	if (len(r.text) != 0):
		print(len(r.text))
		print(r.text)
		return "Bad Content"
	return ""

def simple_options_index(port: int) -> str:
	r = requests.options("http://localhost:" + str(port))
	if (r.status_code != 200):
		return "Bad status code"
	if (len(r.text) != 0):
		print(r.text)
		return "Bad Content"
	if (r.headers['Allow'] != "GET, HEAD, OPTIONS"):
		print(r.headers['Allow'])
		return "Bad Allow header"
	return ""


""" 20 clients doing 100 GET requests on /"""

def one_hundred_get_requests(port: int, nb: int) -> None:
	if nb % 2 == 0:
		port += 1
	for i in range(100):
		r = requests.get("http://localhost:" + str(port))
		if (r.status_code != 200):
			print("client" + i + ": Bad status code")
		if (r.text != "Hello World !\n"):
			print("client" + i + ": Bad Content")
		if (r.headers['Content-Length'] != "14"):
			print("client" + i + ": Bad Content-Length")
	print("client {} has finished all his tasks".format(str(nb)))

def stress_test1(port: int) -> str:
	print("Please wait a few seconds for the stress test to finish ...")
	threads = []

	for i in range(25):
		thread = threading.Thread(target=one_hundred_get_requests, args=(port, i))
		threads.append(thread)
		thread.start()
	for thread in threads:
		thread.join()
	return ""


""" 100 clients doing 20 GET requests on /"""

def one_hundred_get_requestsbis(port: int, nb: int) -> None:
	if nb % 2 == 0:
		port += 1
	for i in range(20):
		r = requests.get("http://localhost:" + str(port))
		if (r.status_code != 200):
			print("client" + i + ": Bad status code")
		if (r.text != "Hello World !\n"):
			print("client" + i + ": Bad Content")
		if (r.headers['Content-Length'] != "14"):
			print("client" + i + ": Bad Content-Length")
	print("client {} has finished all his tasks".format(str(nb)))

def stress_test1bis(port: int) -> str:
	print("Please wait a few seconds for the stress test to finish ...")
	threads = []

	for i in range(100):
		thread = threading.Thread(target=one_hundred_get_requestsbis, args=(port, i))
		threads.append(thread)
		thread.start()
	for thread in threads:
		thread.join()
	return ""

def one_hundred_post_requests(port: int, nb: int) -> None:
	for j in range(10):
		for i in range(100000, 1000001, 100000):
			payload = "a" * i
			r = requests.post("http://localhost:" + str(port) + "/post_upload/index.html", data=payload)
			if (r.status_code != 200 and r.status_code != 201):
				print("client " + str(nb) + ": Bad status code: " + str(r.status_code))
			filename = "./upload/index.html"
			with open(filename) as f:
				content = f.readlines()
			if (len(content[0]) != i):
				print("client " + str(nb) + ": Bad size")
	print("client {} has finished all his tasks".format(str(nb)))

def stress_test2(port: int) -> str:
	print("Please wait a few seconds for the stress test to finish ...")
	threads = []

	for i in range(20):
		thread = threading.Thread(target=one_hundred_post_requests, args=(port, i))
		threads.append(thread)
		thread.start()
	for thread in threads:
		thread.join()
	return ""

def one_hundred_post_requestsbis(port: int, nb: int) -> None:
	for j in range(2):
		for i in range(100000, 1000001, 100000):
			payload = "a" * i
			r = requests.post("http://localhost:" + str(port) + "/post_upload/index.html", data=payload)
			if (r.status_code != 200 and r.status_code != 201):
				print("client " + str(nb) + ": Bad status code: " + str(r.status_code))
			filename = "./upload/index.html"
			with open(filename) as f:
				content = f.readlines()
			if (len(content[0]) != i):
				print("client " + str(nb) + ": Bad size")
	print("client {} has finished all his tasks".format(str(nb)))

def stress_test2bis(port: int) -> str:
	print("Please wait a few seconds for the stress test to finish ...")
	threads = []

	for i in range(100):
		thread = threading.Thread(target=one_hundred_post_requestsbis, args=(port, i))
		threads.append(thread)
		thread.start()
	for thread in threads:
		thread.join()
	return ""

def twenty_cgi_requests(port: int, nb: int) -> None:
	for i in range(20):
		payload = "a" * 1000000
		r = requests.post("http://localhost:" + str(port) + "/cgi/file.tester", data=payload)
		if (r.status_code != 200):
			print("Bad status code {}".format(r.status_code))
		if (len(r.text) != 1000058):
			print("Bad content")
	print("client {} has finished all his tasks".format(str(nb)))

def stress_test3(port: int) -> str:
	print("Please wait a few seconds for the stress test to finish ...")
	threads = []

	for i in range(5):
		thread = threading.Thread(target=twenty_cgi_requests, args=(port, i))
		threads.append(thread)
		thread.start()
	for thread in threads:
		thread.join()
	return ""


def twenty_cgi_requestsbis(port: int, nb: int) -> None:
	for i in range(5):
		payload = "a" * 1000000
		r = requests.post("http://localhost:" + str(port) + "/cgi/file.tester", data=payload)
		if (r.status_code != 200):
			print("Bad status code {}".format(r.status_code))
		if (len(r.text) != 1000058):
			print("Bad content")
	print("client {} has finished all his tasks".format(str(nb)))

def stress_test3bis(port: int) -> str:
	print("Please wait a few seconds for the stress test to finish ...")
	threads = []

	for i in range(20):
		thread = threading.Thread(target=twenty_cgi_requestsbis, args=(port, i))
		threads.append(thread)
		thread.start()
	for thread in threads:
		thread.join()
	return ""

def gen4():
	for i in range(100):
		var1 = str(chr((ord("a") + i % 26))) * 10000
		x = var1.encode('utf8')
		yield x

def POST_CGI_upload_1M(port: int, nb: int) -> None:
	for i in range (10):
		headers = {'Connection': 'keep-alive', 'Content-Type': 'application/x-www-form-urlencoded',
			'Transfer-Encoding': 'chunked'}
		r = requests.post("http://localhost:" + str(port) + "/post_upload/" + "another_file.tester", data=gen4(), headers=headers)
		if (r.status_code != 200 and r.status_code != 201):
			print("Bad status code {}".format(r.status_code))
		if (len(r.text) != 1000058):
			print(len(r.text))
			print("Bad Length Response Body")
		filename = "./upload/another_file.tester"
		with open(filename) as f:
			content = f.readlines()
		if (len(content[0]) != 1000000):
			print(len(content[0]))
			print("upload_file has wrong text")
	print("client {} has finished all his tasks".format(str(nb)))

def stress_test4(port: int) -> str:
	print("Please wait a few seconds for the stress test to finish ...")
	threads = []

	for i in range(5):
		thread = threading.Thread(target=POST_CGI_upload_1M, args=(port, i))
		threads.append(thread)
		thread.start()
	for thread in threads:
		thread.join()
	return ""

def POST_CGI_upload_1Mbis(port: int, nb: int) -> None:
	for i in range (5):
		headers = {'Connection': 'keep-alive', 'Content-Type': 'application/x-www-form-urlencoded',
			'Transfer-Encoding': 'chunked'}
		r = requests.post("http://localhost:" + str(port) + "/post_upload/" + "another_file.tester", data=gen4(), headers=headers)
		if (r.status_code != 200 and r.status_code != 201):
			print("Bad status code {}".format(r.status_code))
		if (len(r.text) != 1000058):
			print(len(r.text))
			print("Bad Length Response Body")
		filename = "./upload/another_file.tester"
		with open(filename) as f:
			content = f.readlines()
		if (len(content[0]) != 1000000):
			print(len(content[0]))
			print("upload_file has wrong text")
	print("client {} has finished all his tasks".format(str(nb)))

def stress_test4bis(port: int) -> str:
	print("Please wait a few seconds for the stress test to finish ...")
	threads = []

	for i in range(10):
		thread = threading.Thread(target=POST_CGI_upload_1Mbis, args=(port, i))
		threads.append(thread)
		thread.start()
	for thread in threads:
		thread.join()
	return ""

def gen5():
	for i in range(2000):
		var1 = str(chr((ord("a") + i % 26))) * 50000
		x = var1.encode('utf8')
		yield x

def chunked_post_size_100M(port: int) -> str:
	print("Please wait a few seconds for the stress test to finish ...")
	headers = {'Connection': 'keep-alive', 'Content-Type': 'application/x-www-form-urlencoded',
		'Transfer-Encoding': 'chunked'}
	r = requests.post("http://localhost:" + str(port) + "/post_upload/" + "another_file.tester", data=gen5(), headers=headers)
	if (r.status_code != 200 and r.status_code != 201):
		return ("Bad status code {}".format(r.status_code))
	if (len(r.text) != 100000058):
		print(len(r.text))
		return("Bad Length Response Body")
	filename = "./upload/another_file.tester"
	with open(filename) as f:
		content = f.readlines()
	if (len(content[0]) != 100000000):
		print(len(content[0]))
		return ("upload_file has wrong text")
	print("client 0 has finished all his tasks")
	return ""
