CC = gcc  # define o compilador


#define os arquivos a serem compilados
SRC = main.c 						\
      models/enemy.c 				\
      models/player.c 				\
	  models/Atack.c              	\
      data_structures/hashmap.c		\
	  data_structures/arraylist.c	\
	  
#define o nome do executavel

ifeq ($(OS),Windows_NT)	# WINDOWS
    OUT = game.exe
	DELETE = rm -f
else 					# LINUX
    OUT = game
	DELETE = del
endif


#junta tudo
all:
	$(CC) $(SRC) -o $(OUT)

run: ./$(OUT)

clean:
	$(DELETE) $(OUT)