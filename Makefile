#!/usr/bin/make -f

# name := "$(shell pwd | sed -r "s/.*\/([[:alnum:] _-]*)/\1/")"
name := "own"
botan_inc := $(shell botan config cflags)
botan_link := $(shell botan config libs)
mysql_inc := -I/usr/include/mysql
mysql_link := -lmysqlpp -lmysqlclient

bin:
	g++ -std=c++17 -O3 -Wall -Wextra -Wpedantic ${botan_inc} ${mysql_inc} -o ${name} ${name}.cpp ${botan_link} ${mysql_link}
dbg:
	g++ -std=c++17 -ggdb3 -Og -Wall -Wextra -Wpedantic ${botan_inc} ${mysql_inc} -o ${name} ${name}.cpp ${botan_link} ${mysql_link}
prof:
	g++ -std=c++17 -pg -O3 -Wall -Wextra -Wpedantic ${botan_inc} ${mysql_inc} -o ${name} ${name}.cpp ${botan_link} ${mysql_link}
clean:
	rm -f ${name}
.PHONY:	bin	dbg	prof	clean
