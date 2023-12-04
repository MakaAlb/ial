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

void parse_input(graph_t *graph, char *filename) {

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

char *parse_cmd(int argc, char **argv, graph_t *graph) {

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
                graph->start_node_id = value;
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
                graph->end_node_id = value;
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
    printf("Start node ID: %ld\n", graph->start_node_id);
    printf("End node ID: %ld\n", graph->end_node_id);
    printf("File: %s\n", filename);
    return filename;
}


/*char *parse_cmd(int argc, char **argv) {
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
}*/

pathset_t *find_paths(graph_t *graph, long start, long end) {
    pathset_t *pathset = malloc(sizeof(pathset_t));
    if (pathset == NULL) {
        printf("Malloc failed.\n");
        exit(EXIT_FAILURE);
    }
    pathset->path_count = 0;
    pathset->paths = NULL;
    find_paths_from_point(graph, start, end, NULL);
    return pathset;
}

void find_paths_from_point(graph_t *graph, long start, long end,
                           graph_t *path) {
    //printf("%d\n",start);
    if (path == NULL) {
        path = malloc(sizeof(graph_t));
        if (path == NULL) {
            printf("Malloc failed.\n");
            exit(EXIT_FAILURE);
        }
        path->nodes = NULL;
        path->nodes_count = 0;
    }
    //printf("realloc\n");
    path->nodes = realloc(path->nodes, sizeof(node_t) * (path->nodes_count + 1));
    if (path->nodes == NULL) {
        printf("Malloc failed.\n");
        exit(EXIT_FAILURE);
    }
    //printf("add to path\n");
    path->nodes[path->nodes_count] = graph->nodes[start];
    path->nodes_count++;

    if (start == end) {
        print_path(path);
    } else {
        for (int i = 0; i < graph->nodes[start].degree; i++) {
            //printf("next: %d\n",graph->nodes[start].neighbours[i]);
            bool explore = true;
            for (int j = 0; j < path->nodes_count; j++) {
                //printf("path node %d: %d\n",j,path->nodes[j].id);
                if (graph->nodes[start].neighbours[i] == path->nodes[j].id) {
                    explore = false;
                    break;
                }
            }
            if (explore) {
                graph_t *path2 = malloc(sizeof(graph_t));
                if (path2 == NULL) {
                    printf("Malloc failed.\n");
                    exit(EXIT_FAILURE);
                }
                path2->nodes = malloc(sizeof(node_t) * path->nodes_count);
                if (path2->nodes == NULL) {
                    printf("Malloc failed.\n");
                    exit(EXIT_FAILURE);
                }
                memcpy(path2->nodes, path->nodes, sizeof(node_t) * path->nodes_count);
                path2->nodes_count = path->nodes_count;
                find_paths_from_point(graph, graph->nodes[start].neighbours[i], end, path2);
            }
        }
    }
}

int main(int argc, char **argv) {
    // declarations
    char filename[MAX_BUF];
    graph_t *graph = malloc(sizeof(graph_t));
    if (graph == NULL) {
        printf("Malloc failed.\n");
        exit(EXIT_FAILURE);
    }

    // parse command line arguments
    strcpy(filename, parse_cmd(argc, argv, graph));

    // parse file
    parse_input(graph, filename);

    print_graph_nodes(graph);

    pathset_t *ps = find_paths(graph, graph->start_node_id, graph->end_node_id);
    //for(int i =0;i<ps->path_count;i++)print_graph_nodes(ps->paths);

    free(ps);

    // free memory
    free_graph(graph);

    return EXIT_SUCCESS;
}
