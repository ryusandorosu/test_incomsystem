EXE = PostgreSQLProxy

all: rebuild run
install:
	cd src && cmake CMakeLists.txt && make
	make clean
uninstall: clean
	rm -rf src/Makefile $(EXE)
clean:
	cd src && rm -rf cmake-build-debug CMakeFiles CMakeCache.txt *.cmake
rebuild: uninstall install
run:
	./$(EXE)
