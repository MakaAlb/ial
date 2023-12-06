# Makefile
# Date: 29.11.2023

CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

GRAPH_PARSER_OBJ = obj/graph_parser.o

BIN = hamiltonian_algorithm

all: $(BIN)

$(BIN): $(GRAPH_PARSER_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(BIN) $(GRAPH_PARSER_OBJ)
