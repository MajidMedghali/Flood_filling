ifndef GSL_PATH
GSL_PATH = .
endif

CFLAGS = -Wall -Wextra -std=c99 -g3 -lgsl -lgslcblas -I $(GSL_PATH)/include/ 

LDFLAGS = -L${GSL_PATH}/lib/ -lgsl -lgslcblas -lm


all: install

server: src/server.o src/server_functions.o src/graph.o src/initialise.o lib_player_genius.so lib_player_Three.so lib_player.so   src/graph_functions.o src/color.o 
	gcc $(CFLAGS) src/server.o src/server_functions.o  src/graph.o src/initialise.o src/graph_functions.o src/color.o -o server -ldl $(LDFLAGS)

server.o: src/server.c
	gcc -c --coverage $(CFLAGS) src/server.c 

server_functions.o: src/server_functions.c
	gcc -c $(CFLAGS) src/server_functions.c


graph_functions.o: src/graph_functions.c
	gcc -c --coverage $(CFLAGS) src/graph_functions.c -fpic

initialise.o: src/initialise.c
	gcc  -c --coverage $(CFLAGS) src/initialise.c

graph:graph.o 
	gcc $(CFLAGS) graph.o -o graph -ldl $(LDFLAGS)		
graph.o: src/graph.c
	gcc -c --coverage $(CFLAGS) src/graph.c



alltests: src/test/test.c color.o initialise.o graph.o genius_functions.o graph_functions.o server_functions.o 
	gcc $(CFLAGS)  src/test/test.c color.o graph.o genius_functions.o initialise.o graph_functions.o  server_functions.o  $(LDFLAGS) -lgcov  -o alltests -ldl $(LDFLAGS)


color.o: src/color.c
	gcc -c --coverage $(CFLAGS) src/color.c

test: alltests


install: server test
	install alltests install
	install server install
	install *.so install

player.o: src/player.c
	gcc -c $(CFLAGS) src/player.c -fPIC

lib_player.so: player.o server_functions.o
	gcc player.o server_functions.o -shared -o lib_player.so


lib_player_genius.so:player_genius.o src/genius_functions.o src/graph_functions.o
	gcc player_genius.o src/genius_functions.o src/graph_functions.o  -shared -o lib_player_genius.so

genius_functions.o:src/genius_functions.c src/graph_functions.o
	gcc -c  $(CFLAGS) src/genius_functions.c -fpic

player_genius.o:src/player_genius.c
	gcc -c $(CFLAGS) src/player_genius.c -fpic

player_genius:player_genius.o  genius_functions.o initialise.o graph.o graph_functions.o color.o server_functions.o
	gcc -g -O0$(CFLAGS) player_genius.o  genius_functions.o initialise.o graph.o graph_functions.o color.o -o genius $(LDFLAGS)


player_Three.o: src/player_Three.c
	gcc -c $(CFLAGS) src/player_Three.c -fpic


lib_player_Three.so: player_Three.o src/graph_functions.o server_functions.o src/color.o
	gcc player_Three.o src/graph_functions.o server_functions.o src/color.o -fPIC -shared -o lib_player_Three.so


clean:
	rm -f install/server install/alltests alltests	src/*.gcno src/*.gcda src/*.gcov *.gcno *.gcda *.gcov *.o src/*.o install/*.so *.so server
