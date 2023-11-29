/**
 * @file graph_parser.c
 *
 * @brief Implementation of functions for parsing graph from file
 *
 * @date 29.11.2023
 */

#include "../include/graph_parser.h"



void error_exit(char *message) {
    fprintf(stderr, ANSI_COLOR_RED "ERROR: " ANSI_COLOR_RESET "%s\n", errno ? strerror(errno) : message);
    exit(EXIT_FAILURE);
}


void free_graph(graph_t *graph) {
    if (graph->nodes != NULL) {
        for (int i = 0; i < graph->nodes_count; i++) {
            if (graph->nodes[i].neighbours != NULL) {
                free(graph->nodes[i].neighbours);
            }
        }
        free(graph->nodes);
    }
    free(graph);
}

graph_t *parse_input(char *filename) {
    graph_t *graph = malloc(sizeof(graph_t));
    if (graph == NULL) {
        printf("Malloc failed.\n");
        exit(EXIT_FAILURE);
    }
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        if (errno == ENOENT) {
            free_graph(graph);
            error_exit("File does not exist");
        }
        else if (errno == EACCES) {
            free_graph(graph);
            error_exit("Permission denied");
        }
    }

    int matrix[MAX_NODES][MAX_NODES];
    int row = 0;
    int col = 0;
    int c;
    while ((c = fgetc(file)) != EOF) {
        if (c == '\n') {
            if (col != 0) {
                row++;
                col = 0;
            }
        }
        if (isdigit(c)) {
            matrix[row][col++] = c - '0';
        }

    }
    graph->nodes_count = row;
    graph->nodes = malloc(sizeof(node_t) * row);
    if (graph->nodes == NULL) {
        free_graph(graph);
        fclose(file);
        error_exit("Malloc failed");
    }
    for (int i = 0; i < row; i++) {
        graph->nodes[i].id = i;
        graph->nodes[i].degree = 0;
        graph->nodes[i].neighbours = malloc(sizeof(int) * row);
        if (graph->nodes[i].neighbours == NULL) {
            free_graph(graph);
            fclose(file);
            error_exit("Malloc failed");
        }
        for (int j = 0; j < row; j++) {
            if (matrix[i][j] == 1) {
                graph->nodes[i].neighbours[graph->nodes[i].degree++] = j;
            }
        }
    }
    fclose(file);
    return graph;
}


void print_graph_nodes(graph_t *graph) {
    printf("Nodes count: %d\n", graph->nodes_count);
    for (int i = 0; i < graph->nodes_count; i++) {
        printf("Node %d:\n", graph->nodes[i].id);
        printf("\tDegree: %d\n", graph->nodes[i].degree);
        printf("\tNeighbours: ");
        for (int j = 0; j < graph->nodes[i].degree; j++) {
            printf("%d ", graph->nodes[i].neighbours[j]);
        }
        printf("\n");
    }
}


char *parse_cmd(int argc, char **argv) {
    if (argc < 2) {
        error_exit("No arguments provided.");
    }
    if (argc > 2) {
        error_exit("Too many arguments provided.");
    }
    if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
        printf(HELP);
        exit(EXIT_SUCCESS);
    }
    return argv[1];
}


int main(int argc, char **argv) {
    // declarations
    char filename[MAX_FILENAME];
    graph_t *graph;

    // parse command line arguments
    strcpy(filename, parse_cmd(argc, argv));

    printf("File: %s\n", filename);

    // parse file
    graph = parse_input(filename);

    print_graph_nodes(graph);

    // free memory
    free_graph(graph);

    return EXIT_SUCCESS;
}
