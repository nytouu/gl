include config.mk

SRC = main.cpp
OBJ = ${SRC:.cpp=.o}

all: clean app

app: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	rm -f ${OBJ} app

run: app
	./app

.PHONY: all app clean run
