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
#include <stdbool.h>


// DEFINES

// color red for error messages
#define ANSI_COLOR_RED "\x1b[31m"

// color reset
#define ANSI_COLOR_RESET "\x1b[0m"

// max filename length
#define MAX_BUF 100

// max nodes in graph
#define MAX_NODES 100

// help message
#define HELP "\nUSAGE: ./hamiltonian_algo -f filename -s start_node_id -e end_node_id\n\n" \
                "\tfilename\tName of the file with the graph.\n"                           \
                "\tstart_node_id\tID of the starting node.\n"                              \
                "\tend_node_id\tID of the end node.\n\n"


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
    long start_node_id;
    long end_node_id;
} graph_t;

typedef struct pathset_t {
    int path_count;
  graph_t **paths;
} pathset_t;



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
char *parse_cmd(int argc, char **argv, graph_t *graph);

/**
 * @brief Parses the input file and creates a graph
 *
 * @param graph - graph structure to be filled
 * @param filename - name of the file with the graph
 */
void parse_input(graph_t *graph, char *filename);

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

void free_pathset(pathset_t *pathset);

void print_path(graph_t* path);

/**
 * @brief finds all Hamiltonian path from start node to end node
 *
 * @param graph - graph to be searched
 * @param start - staring node
 * @param end - end node
 *
 */

pathset_t *find_paths(graph_t *graph, long start, long end);

/**
 * @brief finds all Hamiltonian path from vertex start to end vertex
 *
 * @param graph - graph to be searched
 * @param start - starting node
 * @param end - end node
 * @param path - already searched path, NULL if first level of iteration
 * @param pathset - if path is found will contain all found paths
 *
 */
void find_paths_from_point(graph_t *graph, long start, long end,
                           graph_t *path);

#endif //IAL_GRAPH_PARSER_H
