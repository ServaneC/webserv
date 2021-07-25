from tests import *
from typing import Callable

import os
import sys

DEFAULT = "\033[39m"
GREEN = "\033[32m"
RED = "\033[31m"


def check_test(port: int, name_test: str, test: Callable) -> None:
	try:
		str_result = test(port)
	except:
		str_result = "couldn't connect"

	if (len(str_result) == 0):
		check = GREEN + "V" + DEFAULT
	else:
		check = RED + "X" + DEFAULT
	
	print("test : {:65} | result : {} {}".format(name_test, check, str_result))


def run(option: str, port: int) -> None:
	"""run tests"""
	
	print("All tests  are done requesting on http://localhost:port\n")

	if (option == "basic"):
		check_test(port, "GET / ", simple_get_index)
		check_test(port, "GET /auto autoindex", get_autoindex_subdir)
		check_test(port, "GET /forbidden (403) ", get_forbidden_dir)
		check_test(port, "GET non_existing_dir (404) ", get_404)
		check_test(port, "GET / ports 8080 and 8081", get_index_two_ports)
		check_test(port, "GET / 1 worker 50 times", fifty_get_root)
		check_test(port, "POST / method not authorized (405)  ", wrong_method)
		
		# check_test(port, "POST /post ", simple_post)
		# check_test(port, "POST /post content length = 0", post_size_0)
		# check_test(port, "POST /post request too big (413)", post_too_big)
		# check_test(port, "POST /post_upload upload in /upload", post_with_upload)
		
		check_test(port, "DELETE /delete_folder/index.html", delete)
		check_test(port, "DELETE /delete_folder/index.html (404)", delete_already_deleted)
		
		check_test(port, "GET http://webserv:port use of server_name ", server_name)
		# check_test(port, "GET /redirect_me/please redirect 301 in /redirection/index.html", redirect_get_ok)
		# check_test(port, "GET /redirect_me/wrong redirect 301 in /redirection/lol.html", redirect_get_non_existing)

		# check_test(port, "POST /post_upload upload in /upload", post_with_upload)

		# check_test(port, "POST /post_upload upload in /upload", post_with_upload)
		# check_test(port, "POST /post_upload upload in /upload", post_with_upload)

		check_test(port, "GET /use_location_root/", get_loc_root)
		check_test(port, "POST /use_location_root/", post_loc_root)
		
		print("\nCGI TESTS & CHUNKED REQUESTS:")
		check_test(port, "GET /cgi/file.tester ", cgi_tester_get)
		check_test(port, "POST /cgi/file.tester", cgi_tester_post)
		check_test(port, "GET /use_location_root_cgi/file.tester ", cgi_tester_get_loc_root)
		check_test(port, "POST /use_location_root_cgi/file.tester", cgi_tester_post_loc_root)

		check_test(port, "POST /cgi/file.tester 20 bits", chunked_post_no_upload)
		check_test(port, "POST /cgi/file.tester 500 bits", chunked_post_no_upload_size_500)

		check_test(port, "POST /post-upload/another_file.tester 20 bits", chunked_post_upload)
		check_test(port, "POST /post-upload/another_file.tester 500 bits", chunked_post_size_500)

	if (option == "stress"):
		print("STRESS TESTS:")
		check_test(port, "GET / 25 workers 100 times", stress_test1)
		check_test(port, "GET / 100 workers 25 times", stress_test1bis)
		check_test(port, "POST 100k-1M bits 10 workers 100 times", stress_test2)
		check_test(port, "POST 100k-1M bits 100 workers 10 times", stress_test2bis)
		check_test(port, "POST CGI 1M bits 5 workers 20 times", stress_test3)
		check_test(port, "POST CGI 1M bits 20 workers 5 times", stress_test3bis)
		check_test(port, "POST CGI w/ upload 1M bits 5 workers 10 times", stress_test4)
		check_test(port, "POST CGI w/ upload 1M bits 10 workers 5 times", stress_test4bis)
		check_test(port, "POST /post-upload/another_file.tester 100M bits", chunked_post_size_100M)

if (__name__ == "__main__"):
	if (len(sys.argv) != 2):
		print("Usage: python3 main.py tests.py [port]")
		exit(1)
	try:
		port = int(sys.argv[1])
	except:
		print("please input a valid port (int)")
		exit(1)

	os.chdir("tester_documents")
	os.system("rm -rf ./upload/*")
	os.system("echo \"Delete me please\" > ./delete_folder/index.html")

	option = ""
	while (option != 'basic' and option != 'stress'):
		print("Available tests options 'basic' and 'stress'")
		print('Please type the type of tests you want to do:')
		option = input()
		print()
	
	run(option, port)
	os.system("rm -rf ./upload/*")
