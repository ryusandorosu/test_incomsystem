EXE = PostgreSQLProxyServer

all: rebuild
install:
	cd src && cmake CMakeLists.txt && make
	make clean
uninstall: clean
	rm -rf src/Makefile $(EXE)
clean:
	cd src && rm -rf cmake-build-debug CMakeFiles CMakeCache.txt *.cmake
	rm -rf test_client sql_queries.log
rebuild: uninstall install

test_install:
	g++ -Wall -Werror -Wextra src/test_client.cpp -o test_client