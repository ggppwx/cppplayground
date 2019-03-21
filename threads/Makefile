all: reader-writer threadpool
	echo "Done"

reader-writer: reader-writer.o
	g++-8 -o $@ $<

threadpool: threadpool.o
	g++-8 -o $@ $<

%.o: %.cpp
	g++-8 -g -o $@ -c $<

threadpool.o: threadpool.hpp
