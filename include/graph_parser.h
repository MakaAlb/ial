/**
 * @file graph_parser.h
 *
 * @brief Header file for graph_parser.c
 *
 * @date 29.11.2023
 */

#ifndef IAL_GRAPH_PARSER_H
#define IAL_GRAPH_PARSER_H



// INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>



// DEFINES

// color red for error messages
#define ANSI_COLOR_RED "\x1b[31m"

// color reset
#define ANSI_COLOR_RESET "\x1b[0m"

// max filename length
#define MAX_FILENAME 100

// max nodes in graph
#define MAX_NODES 100

// help message
#define HELP "\nUSAGE: ./hamiltonian_algo filename\n\n" \
                "\tfilename - name of the file with the graph\n\n"



// STRUCTURES

/**
 * @brief Structure representing a node in the graph
 *
 * @param id - id of the node
 * @param degree - degree of the node
 * @param neighbours - array of neighbours of the node
 */
typedef struct node_t {
    int id;
    int degree;
    int *neighbours;
} node_t;

/**
 * @brief Structure representing a graph
 *
 * @param nodes_count - number of nodes in the graph
 * @param nodes - array of nodes in the graph
 */
typedef struct graph_t {
    int nodes_count;
    node_t *nodes;
} graph_t;



// FUNCTIONS

/**
 * @brief Prints error message and exits the program
 *
 * @param message - error message, displayed only when errno is 0
 */
void error_exit(char *message);

/**
 * @brief Parses command line arguments
 *
 * @param argc - number of arguments
 * @param argv - array of arguments
 *
 * @return filename - name of the file with the graph
 */
char *parse_cmd(int argc, char **argv);

/**
 * @brief Parses the input file and creates a graph
 *
 * @param filename - name of the file with the graph
 *
 * @return graph - graph created from the input file
 */
graph_t *parse_input(char *filename);

/**
 * @brief Prints nodes of the graph with their information
 *
 * @param graph - graph to be printed
 */
void print_graph_nodes(graph_t *graph);

/**
 * @brief Frees the graph
 *
 * @param graph - graph to be freed
 */
void free_graph(graph_t *graph);


#endif //IAL_GRAPH_PARSER_H