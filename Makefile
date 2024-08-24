EXE = server.out

all: rebuild
install:
	cd src && cmake CMakeLists.txt && make
	make clean
uninstall: clean
	rm -rf src/Makefile $(EXE)
clean:
	cd src && rm -rf cmake-build-debug CMakeFiles CMakeCache.txt *.cmake
	rm -rf sql_queries.log
rebuild: uninstall install
