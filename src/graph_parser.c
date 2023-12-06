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
            error_exit("parse_input(): File does not exist");
        } else if (errno == EACCES) {
            free_graph(graph);
            error_exit("parse_input(): Permission denied");
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
        error_exit("parse_input(): Malloc failed");
    }
    for (int i = 0; i < row; i++) {
        graph->nodes[i].id = i;
        graph->nodes[i].degree = 0;
        graph->nodes[i].neighbours = malloc(sizeof(int) * row);
        if (graph->nodes[i].neighbours == NULL) {
            free_graph(graph);
            fclose(file);
            error_exit("parse_input(): Malloc failed");
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

void print_path(graph_t *path) {
    printf("Nodes count: %d\n", path->nodes_count);
    printf("\t");
    for (int i = 0; i < path->nodes_count; i++) {
        printf("%d ", path->nodes[i].id);
    }
    printf("\n");
}

char arg_value[MAX_BUF];

int get_arg(int argc, char *argv[], char *flags) {
    static int arg_id = 1;
    if (arg_id >= argc) {
        return -1;
    }

    if (argv[arg_id][0] == '-') {
        for (size_t i = 0; i < strlen(flags); i++) {
            if (argv[arg_id][1] == flags[i] && argv[arg_id][2] == '\0') {
                memset(arg_value, 0, MAX_BUF);
                strcpy(arg_value, argv[arg_id + 1]);
                arg_id += 2;
                return argv[arg_id - 2][1];
            }
        }
        error_exit("get_arg(): Wrong argument");
    } else {
        error_exit("get_arg(): Wrong argument");
    }
    return -1;
}

char *parse_cmd(int argc, char **argv, long *start_node_id, long *end_node_id) {

    // VARIABLES

    // flags for checking if arguments are set
    bool start_node_set = false,
            end_node_set = false,
            filename_set = false;

    // for get_arg function, contains the option character
    int opt;

    // for strtol string part
    char *end_ptr = NULL;

    // value of the flag
    long value;

    // name of the file with the graph
    char *filename = NULL;

    if (argc == 1) {
        fprintf(stdout, HELP);
        exit(EXIT_SUCCESS);
    }

    if (argc != 7) {
        error_exit("parse_cmd(): Wrong number of arguments");
    }

    while ((opt = get_arg(argc, argv, "sef")) != -1) {
        switch (opt) {
            case 's':
                if (start_node_set) {
                    error_exit("parse_cmd(): Start node ID already set");
                }
                value = strtol(arg_value, &end_ptr, 10);
                if (*end_ptr != '\0') {
                    error_exit("parse_cmd(): Start node ID must be a number");
                }
                if (value < 0) {
                    error_exit("parse_cmd(): Start node ID must be positive");
                }
                *start_node_id = value;
                start_node_set = true;
                break;
            case 'e':
                if (end_node_set) {
                    error_exit("parse_cmd(): End node ID already set");
                }
                value = strtol(arg_value, &end_ptr, 10);
                if (*end_ptr != '\0') {
                    error_exit("parse_cmd(): End node ID must be a number");
                }
                if (value < 0) {
                    error_exit("parse_cmd(): End node ID must be positive");
                }
                *end_node_id = value;
                end_node_set = true;
                break;
            case 'f':
                if (filename_set) {
                    error_exit("parse_cmd(): Filepath already set");
                }
                filename = malloc(sizeof(char) * (strlen(arg_value) + 1));
                if (filename == NULL) {
                    error_exit("parse_cmd(): Malloc failed");
                }
                strcpy(filename, arg_value);
                strcat(filename, "\0");
                filename_set = true;
                break;
            default: /* '?' */
                error_exit("parse_cmd(): Wrong argument");
        }
    }
    printf("Start node ID: %ld\n", *start_node_id);
    printf("End node ID: %ld\n", *end_node_id);
    printf("File: %s\n", filename);
    return filename;
}

path_set_t *find_paths(graph_t *graph, long start, long end) {
    path_set_t *path_set = malloc(sizeof(path_set_t));
    if (path_set == NULL) {
        error_exit("find_paths(): Malloc failed");
    }
    path_set->path_count = 0;
    path_set->paths = NULL;
    find_paths_from_point(graph, start, end, NULL);
    return path_set;
}

void find_paths_from_point(graph_t *graph, long start, long end,
                           graph_t *path) {
    node_t *tmp_ptr;
    bool explore;

    //printf("%d\n",start);
    if (path == NULL) {
        path = malloc(sizeof(graph_t));
        if (path == NULL) {
            error_exit("find_paths_from_point(): Malloc failed");
        }
        path->nodes = NULL;
        path->nodes_count = 0;
    }
    //printf("realloc\n");
    tmp_ptr = realloc(path->nodes, sizeof(node_t) * (path->nodes_count + 1));
    if (tmp_ptr == NULL) {
        error_exit("find_paths_from_point(): Realloc failed");
    }
    path->nodes = tmp_ptr;
    //printf("add to path\n");
    path->nodes[path->nodes_count] = graph->nodes[start];
    path->nodes_count++;

    if (start == end) {
        if (path->nodes_count == graph->nodes_count + (path->nodes[0].id == end ? 1 : 0)) {
            print_path(path);
            free(path->nodes);
            free(path);
            return;
        }
        if (path->nodes_count > 1) {
            free(path->nodes);
            free(path);
            return;
        } //visited start prematurly
    }

    for (int i = 0; i < graph->nodes[start].degree; i++) {
        //printf("next: %d\n",graph->nodes[start].neighbours[i]);
        explore = true;
        for (int j = 0; j < path->nodes_count; j++) {
            //printf("path node %d: %d\n",j,path->nodes[j].id);
            if (graph->nodes[start].neighbours[i] == path->nodes[j].id
                && graph->nodes[start].neighbours[i] != end) {
                explore = false;
                break;
            }
        }
        if (explore) {
            graph_t *path2 = malloc(sizeof(graph_t));
            if (path2 == NULL) {
                error_exit("find_paths_from_point(): Malloc failed");
            }
            path2->nodes = malloc(sizeof(node_t) * path->nodes_count);
            if (path2->nodes == NULL) {
                error_exit("find_paths_from_point(): Malloc failed");
            }
            memcpy(path2->nodes, path->nodes, sizeof(node_t) * path->nodes_count);
            path2->nodes_count = path->nodes_count;
            find_paths_from_point(graph, graph->nodes[start].neighbours[i], end, path2);
        }
    }
    free(path->nodes);
    free(path);
}

int main(int argc, char **argv) {
    // declarations
    char *filename;
    long start_node_id;
    long end_node_id;
    graph_t *graph;


    // parse command line arguments
    filename = parse_cmd(argc, argv, &start_node_id, &end_node_id);

    // parse file
    graph = parse_input(filename);

    path_set_t *ps = find_paths(graph, start_node_id, end_node_id);


    // free memory
    free(ps);
    free_graph(graph);
    free(filename);

    return EXIT_SUCCESS;
}
