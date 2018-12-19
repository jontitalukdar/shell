getenv: getenv.cpp
	g++ -o getenv -ggdb3 -Wall -Werror -pedantic -std=gnu++98 getenv.cpp
test: test.cpp
	g++ -o test -ggdb3 -Wall -Werror -pedantic -std=gnu++98 test.cpp
test3: test3.cpp
	g++ -o test3 -ggdb3 -Wall -Werror -pedantic -std=gnu++98 test3.cpp -lreadline
test4: test4.c
	g++ -o test4 -ggdb3 -Wall -Werror -pedantic -std=gnu++98 test4.c
execve: execve.c
	g++ -o execve -ggdb3 -Wall -Werror -pedantic -std=gnu++98 execve.c
echo: echo.c
	g++ -o myecho -ggdb3 -Wall -Werror -pedantic -std=gnu++98 myecho.c
waitpid: waitpid.c
	g++ -o waitpid -ggdb3 -Wall -Werror -pedantic -std=gnu++98 waitpid.c
fork: fork.cpp
	g++ -o fork -ggdb3 -Wall -Werror -pedantic -std=gnu++98 fork.cpp
integrated: integrated.cpp
	g++ -o integrated -ggdb3 -Wall -Werror -pedantic -std=gnu++98 integrated.cpp

parser: parser.cpp
	g++ -o parser -ggdb3 -Wall -Werror -pedantic -std=gnu++98 parser.cpp

getcwd: getcwd.cpp
	g++ -o getcwd -ggdb3 -Wall -Werror -pedantic -std=gnu++98 getcwd.cpp

env_var: getcwd_var.cpp
	g++ -o env_var -ggdb3 -Wall -Werror -pedantic -std=gnu++11 getcwd_var.cpp


var: var.cpp
	g++ -o var -ggdb3 -Wall -Werror -pedantic -std=gnu++98 var.cpp


shell: shell.cpp
	g++ -o shell -ggdb3 -Wall -Werror -pedantic -std=gnu++98 shell.cpp 

myshell: myshell.cpp
	g++ -o myshell -ggdb3 -Wall -Werror -pedantic -std=gnu++11 myshell.cpp

redirect: redirect.c
	gcc -o shell -ggdb3 -Wall -Werror -pedantic -std=gnu98 redirect.c 


clean:
	rm  test test2 test3 *~
