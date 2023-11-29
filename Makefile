# Makefile
#
# Authors: Adam Světlík, xsvetl07
#          <INSERT NAME>
#          <INSERT NAME>
#          <INSERT NAME>
#
# Date: 29.11.2023

CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

# Here add other object files
GRAPH_PARSER_OBJ = obj/graph_parser.o

# TODO: mby rename
BIN = hamiltonian_algorithm

all: $(BIN)

$(BIN): $(GRAPH_PARSER_OBJ) # Here add other object files
	$(CC) $(CFLAGS) -o $@ $^

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(BIN) $(GRAPH_PARSER_OBJ) # Here add other object files
