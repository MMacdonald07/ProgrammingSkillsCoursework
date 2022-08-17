#include <stdio.h>
// Necessary import for rand() and srand() functions as well as any memory allocation functionality
#include <stdlib.h>

// Defines boolean enumeration for more elegant checking of whether points have been visited
typedef enum {
    false = 0,
    true = 1
} boolean;

// A structure to represent a node in the network
struct Node {
    int destination;
    boolean visited;
    struct Node *next;
};

// A structure to represent a graph - an array of adjacency lists
// The size of the adjacency list is made to be 400 as the maximum graph size tested will be 20 clusters with 20 sites
// Would have been more efficient if *head[] could have been dynamically allocated memory
struct Graph {
    int clusters;
    int sites;
    int vertices;
    struct Node *head[400];
};

// Declaration of a function that adds a new edge to the graph
void addEdge(struct Graph *graph, int src, int destination);

// Declaration of a function that creates a graph of V vertices with edges where they should be based on cluster and site count
struct Graph *createGraph(int clusters, int sites, int V);

// Declaration of a function that frees up memory used by a graph struct
void deleteGraph(struct Graph *graph);

// Declaration of a function that returns the number of connections
// and an array containing each connection destination for a given vertex
// These values can then be used in conjunction to decide where the particle will move to next
void getNodeAdjacencyList(struct Graph *graph, int vertex, int *connections, int *num_connections);

// Declaration of a function that returns a random integer in the range 0...(max-1)
int randomInt(int max);

// Declaration of a function that returns either 0 or 1 depending on if all points in a network have been visited
boolean isAllGraphVisited(struct Graph *graph);

int main() {
    // Initialise variables for incrementation, number of clusters/sites
    int clusters, sites, vertex, num_connections, loop_counter;
    int seed, rand_num, lower, upper, total;
    float average;
    FILE *fp;

    // File to write average number of iterations to traverse each site for each cluster and site value used
    char *filename = "iterations_Q3.csv";
    fp = fopen(filename, "w+");

    // Asks user to input lower and upper seed values - will use these to take average number of iterations
    printf("The program will use a range of seeds to calculate an average for each number of sites.\n");
    printf("Note that any inputted floats will be rounded down here.\n");
    printf("Enter the starting seed: \n");
    // Will loop the program until a valid starting seed is inputted by the user
    while (1) {
        if (scanf("%d", &lower)) {
            // If input is valid, breaks out of loop and clears input buffer in case floating point typed in
            while (getchar() != '\n');
            break;
        } else {
            printf("Please enter an integer\n");
            // Clears input buffer for user to input their desired precision again
            while (getchar() != '\n');
            continue;
        }
    }

    printf("Enter the final seed: \n");
    // Will loop the program until a valid final seed value is inputted by the user
    while (1) {
        if (scanf("%d", &upper) && upper > lower) {
            // If input is valid, breaks out of loop and clears input buffer in case floating point typed in
            while (getchar() != '\n');
            break;
        } else {
            printf("Please enter a valid upper seed value\n");
            // Clears input buffer for user to input their desired precision again
            while (getchar() != '\n');
            continue;
        }
    }

    // Loops for clusters of size 2 up to and including size 20
    for (clusters = 2; clusters <= 20; clusters++) {
        // Loops to obtain data for sites of size 2 up to and including size 20
        for (sites = 2; sites <= 20; sites++) {
            // Calculate number of nodes based on clusters and sites
            int vertices = clusters * sites;
            // Reset total for each loop of varying sites/clusters
            total = 0;

            // Loops through seed values given by user
            for (seed = lower; seed <= upper; seed++) {
                // Creates graph of A clusters and B sites including connections
                struct Graph *graph = createGraph(clusters, sites, vertices);
                // Sets up random number generator
                srand(seed);

                // Initiates particle pointer at first node in graph adjacency list and sets its visited value to True
                graph->head[0]->visited = true;
                struct Node *particle;

                // Sets vertex number to 0 as starting at first node and num_connections incrementation counter to 0
                vertex = 0;
                num_connections = 0;
                loop_counter = 0;

                do {
                    // Prepares connections array which will contain a maximum number of edges equal to number of sites
                    int connections[sites];
                    // Produces a list of all accessible sites from current node and gets the number of connections
                    getNodeAdjacencyList(graph, vertex, connections, &num_connections);
                    // Returns random number between 0 and (num_connections-1) - this will relate directly to
                    // the connections array
                    rand_num = randomInt(num_connections);
                    // Points the particle at the new node and updates the vertex value
                    particle = graph->head[connections[rand_num]];
                    vertex = connections[rand_num];

                    // If the particle's visited value is False, update this to True
                    if (particle->visited == false) {
                        graph->head[connections[rand_num]]->visited = true;
                    }
                    // Reset num_connections each loop, so it does not continue incrementing
                    // from current vertex num_connections
                    num_connections = 0;
                    loop_counter++;
                    // Stop looping when each node in the network has been visited
                } while (isAllGraphVisited(graph) != true);

                // Add the number of loops required to traverse whole graph to the total counter
                total += loop_counter;
                // Free graph from memory before creating a new one
                deleteGraph(graph);
            }

            // Program takes average of seeds including both upper and lower bounds so add one to total number of measurements
            average = (float) total / (float) (upper + 1 - lower);
            // Saves number of clusters, sites, and calculated average value over seed range to .csv file
            fprintf(fp, "%d, %d, %f\n", clusters, sites, average);
        }
    }

    // Closes file
    fclose(fp);
    printf("Data saved to %s\n", filename);

    // Shows program has executed without errors
    return 0;
}

// Function Definition
void addEdge(struct Graph *graph, int src, int destination) {
    // Allocate memory to new adjacency list node from src to destination
    struct Node *newNode = (struct Node *) malloc(sizeof(struct Node));
    newNode->destination = destination;
    // Initialise visited value as False
    newNode->visited = false;
    // Point new node to current head
    newNode->next = graph->head[src];
    // Point head to new node
    graph->head[src] = newNode;
    // Allocate memory to new adjacency list node from destination to src as graph is undirected
    // so need to add a corresponding edge going the other way
    newNode = (struct Node *) malloc(sizeof(struct Node));
    newNode->destination = src;
    // Set visited value as False
    newNode->visited = false;
    // Point new node to current head
    newNode->next = graph->head[destination];
    // Point head to new node
    graph->head[destination] = newNode;
}

// Function Definition
struct Graph *createGraph(int clusters, int sites, int V) {
    // Allocate memory to graph pointer
    struct Graph *graph = (struct Graph *) malloc(sizeof(struct Graph));

    graph->vertices = V;
    graph->clusters = clusters;
    graph->sites = sites;

    // Initialise each adjacency list element as empty by making its head NULL
    int i, j;
    for (i = 0; i < V; i++) {
        graph->head[i] = NULL;
    }

    for (i = 0; i < V; i++) {
        // If vertex is an inner node connecting to previous cluster
        if (i % sites == 0) {
            // If vertex is first one connecting to previous cluster, add edge to final vertex to implement
            // periodic boundary conditions in network
            if (i == 0) {
                addEdge(graph, ((clusters * sites) - (sites - 1)), i);
            }
            for (j = i; j <= (i + sites - 1); j++) {
                // A vertex should not be connected to itself
                if (j != i) {
                    addEdge(graph, j, i);
                }
            }
        // If vertex is an inner node connecting to next cluster
        } else if ((i - 1) % sites == 0) {
            // If vertex is final one connecting to new cluster, this edge has already been added, so it can be skipped
            if (i != ((clusters * sites) - (sites - 1))) {
                // Otherwise, add an edge connecting inner node to the next cluster
                addEdge(graph, (i + sites - 1), i);
            }
            for (j = i; j < (i + sites - 1); j++) {
                if (j != i) {
                    addEdge(graph, j, i);
                }
            }
        // Otherwise, vertex is an outer node with (sites-1) edges - of which 2 of these edges have already been added
        } else {
            // Add edges between outer node and other outer nodes
            for (j = i; j <= (i + (sites - 2)); j++) {
                // If j is vertex of node in next cluster, an outer node should not be pointing to it
                // so this node's edges are complete
                if (j % sites == 0) {
                    break;
                }
                if (j != i) {
                    addEdge(graph, j, i);
                }
            }
        }
    }

    return graph;
}

void deleteGraph(struct Graph *graph) {
    int i;
    for (i = 0; i < graph->vertices; i++) {
        // For each vertex in graph, adjListNode is freed up from memory
        free(graph->head[i]);
    }
    // Then graph itself is freed from memory
    free(graph);
}

// Function Definition
void getNodeAdjacencyList(struct Graph *graph, int vertex, int *connections, int *num_connections) {
    // Creates a new node starting at the given vertex in the adjacency list
    struct Node *startNode = graph->head[vertex];
    // Continue looping while startNode != NULL
    while (startNode) {
        // Adds the edge destination to the connections array e.g. for num_connections = 1, the second connection
        // for the vertex will be appended to the array
        connections[*num_connections] = startNode->destination;
        // Moves startNode to the next node in its adjacency list
        startNode = startNode->next;
        // Increments value of num_connections
        (*num_connections)++;
    }
}

// Function Definition
int randomInt(int max) {
// Note rand() is a built-in C function
// The % operator divides by max and takes the remainder (modulo operator)
    return (rand() % max);
}

// Function Definition
boolean isAllGraphVisited(struct Graph *graph) {
    int v;
    // For every node in graph's adjacency list
    for (v = 0; v < graph->vertices; v++) {
        // If node has not been visited then return false
        if (graph->head[v]->visited == false) {
            return false;
        }
    }
    // Otherwise, return true
    return true;
}
