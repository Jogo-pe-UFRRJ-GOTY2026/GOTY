CC = gcc  # define o compilador
CFLAGS = -Wall
LIBS = -fms-extensions -lncursesw


# necessario para printar unicode
export LANG=C.UTF-8
export LC_ALL=C.UTF-8

#define os arquivos a serem compilados
SRC = main.c 						\
      objects/Inimigo.c 			\
      objects/Inventario.c 			\
      objects/Player.c 				\
      system/Save.c 				\
      system/Combate.c 				\
	  utils/utils.c                 \
	  chapters/Prologo.c            \
	  chapters/Capitulo1.c          \
	  chapters/Capitulo2.c          \
	  chapters/Capitulo3.c          \
	  chapters/Capitulo4.c          \
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

run: all
	$(OUT)

clean:
	$(DELETE) $(OUT) 
	$(CLEAR)
