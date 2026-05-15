CC = gcc  # define o compilador
CFLAGS = -Wall
LIBS = -lncurses

#define os arquivos a serem compilados
SRC = main.c 						\
      models/enemy.c 				\
      models/player.c 				\
	  models/Atack.c              \
      data_structures/hashtable.c	\
	  data_structures/arraylist.c	\
	  
#define o nome do executavel

ifeq ($(OS),Windows_NT)	# WINDOWS
    OUT = C:/temp/game.exe
	DELETE = del
	CLEAR = ;cls
else 					# LINUX
    OUT = /tmp/game
	DELETE = rm -f
	CLEAR = && clear
endif


#junta tudo
all:
	$(CC) $(SRC) -o $(OUT) $(LIBS)

run: $(OUT)
	$(OUT)

clean:
	$(DELETE) $(OUT) $(CLEAR)
