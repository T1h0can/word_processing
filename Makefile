CC = g++ $(CFLAGS)
CFLAGS = -O0 -g -std=c++11

rusWordPair: rusWordPair.cpp utf8/utf8.h utf8/utf8.cpp
	$(CC) -o rusWordPair rusWordPair.cpp utf8/utf8.cpp

clean:
	rm -f rusWordPair
