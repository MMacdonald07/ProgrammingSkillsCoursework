#include <stdio.h>
// Necessary import for rand() and srand() functions
#include <stdlib.h>

// Defines boolean enumeration for more elegant checking of whether points have been visited
typedef enum {
    false = 0,
    true = 1
} boolean;

// Declares structure which will store 3-dimensional points and a "boolean" value for whether the point has been traversed
typedef struct {
    int x;
    int y;
    int z;
    boolean visited;
} point;

// Declaration of a function that returns a random integer
// In the range 0...(max-1)
int randomInt(int max);

// Declaration of a function that returns either 0 or 1 dependent on if all points in an array have been visited
boolean arrayAllTrue(int num, point arr[(num * num * num)]);

int main() {
    // Initialise variables for incrementation and number of sites in one direction
    int i, j, k, m, n, loop_counter;
    // Initialise variable for maximum random generated number: 6 here as particle can move in 6 directions
    int nmax=6;
    int seed, rand_num, lower, upper, total;
    float average;
    FILE *fp;

    // File to write average number of iterations to traverse each site for each n
    char *filename = "iterations_Q2.csv";
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

    // Loops through n=2 to n=25 -> so sites = 8 to sites = 15625
    for (n=2;n<=25;n++) {
        // Creates a flat 1-D array consisting of n^3 3-D points
        point grid[n*n*n];
        // Resets value of total for each number of sites loop
        total = 0;

        // Loops through seed values from user input
        for (seed=lower;seed<=upper;seed++) {
            // Set up the random number generator
            srand(seed);
            // Resets value of m used in creation of grid and resets loop counter tracking how many loops for each
            // site to be traversed
            m = 0;
            loop_counter = 0;

            // Creates a 1-D array of type Point in which the Point increases in value in 3 dimensions
            // e.g. after three k loops, j will be incremented so that grid[3] has coords (1, 2, 1) if n=3
            // m is crucial in keeping track of index of grid
            for (i=0;i<n;i++) {
                for (j=0;j<n;j++) {
                    for (k=0; k<n; k++,m++) {
                        grid[m].x = i+1;
                        grid[m].y = j+1;
                        grid[m].z = k+1;
                        // Sets all points to be unvisited (boolean of 0)
                        grid[m].visited = false;
                    }
                }
            }

            // As particle starts at the first grid point, its visited value is set to true
            grid[0].visited = true;

            // Initialises particle pointer which points to the first grid point - coords of (1, 1, 1) for n=3
            point *particle = grid;

            do {
                // Generates random integer between 0 and 5
                rand_num = randomInt(nmax);

                // Uses switch-case statement on rand_num rather than 5 else if statements as it looks more elegant
                switch (rand_num){
                    // Particle will move backwards in z-direction
                    case 0:
                        // If particle is at grid point with z=1, needs to apply periodic conditions
                        if (particle->z == 1) {
                            // Want to make particle now point to grid point
                            // where x=particle.x, y=particle.y and z = n-1
                            // Can simply increment address by (n-1)
                            particle += (n-1);
                            break;
                        } else {
                            // Otherwise just decrement particle.z
                            particle--;
                            break;
                        }
                        // Particle moves forwards in z-direction
                    case 1:
                        // If particle is at maximum value of z, needs to move back to smallest z for periodic conditions
                        if (particle->z == n) {
                            // Do inverse of earlier condition to move back to z=1
                            particle -= (n-1);
                            break;
                        } else {
                            // Otherwise, simply increment particle.z
                            particle++;
                            break;
                        }
                        // Backwards in y-direction
                    case 2:
                        // Applies periodic conditions to lower y boundary
                        if (particle->y == 1) {
                            // Similar to earlier condition but multiplied by n as y-direction is higher level than
                            // z-direction with regard to grid point addresses
                            particle += n*(n-1);
                            break;
                        } else {
                            // If n=3, decrementing particle address by 3 will be akin to moving backwards in y
                            particle -= n;
                            break;
                        }
                        // Forwards in y-direction
                    case 3:
                        // Inverse of what is carried out for backwards in y
                        if (particle->y == n) {
                            particle -= n*(n-1);
                            break;
                        } else {
                            particle += n;
                            break;
                        }
                        // Backwards in x-direction
                    case 4:
                        // Similar to earlier condition but multiplied by n^2 as x-direction is higher level than
                        // z-direction and y-direction with regard to grid point addresses
                        if (particle->x == 1) {
                            particle += n*n*(n-1);
                            break;
                        } else {
                            // If n=3, decrementing particle address by 9 will be akin to moving backwards in x
                            particle -= (n*n);
                            break;
                        }
                        // Forwards in x-direction
                    case 5:
                        if (particle->x == n) {
                            particle -= n*n*(n-1);
                            break;
                        } else {
                            particle += (n*n);
                            break;
                        }
                    default:
                        break;
                }

                // If particle.visited for particle's new position is false, this needs to be changed to true for the whole grid
                if (particle->visited == false) {
                    // By doing particle - grid (defaults to grid[0]), program performs memory arithmetic to calculate
                    // the index of the grid which particle is currently pointing to
                    grid[(particle - grid)].visited = true;
                }

                loop_counter++;
                // Ends the loop when all grid points have been visited
            } while (arrayAllTrue(n, grid) != true);

            // Increment total by however many loops were required to visit every point for particular seed value
            total += loop_counter;
        }

        // Calculates a floating point average of how many loops were required to visit every point
        // Program takes average of seeds including both upper and lower bounds so add one to total number of measurements
        average = (float) total / (float) (upper + 1 - lower);
        // Saves this value and number of sites per grid direction to .csv file
        fprintf(fp, "%d, %f\n", n, average);
    }

    // Closes file
    fclose(fp);
    printf("Data saved to %s\n", filename);

    // Returns 0 to show program has executed without error
    return 0;
}

// Function definition
int randomInt(int max) {
// Note rand() is a built-in C function
// The % operator divides by max and takes the remainder (modulo operator)
    return (rand() % max);
}

// Function definition
boolean arrayAllTrue(int num, point arr[(num * num * num)]) {
    // For every point in grid array
    for (int i=0;i<(num*num*num);i++) {
        // If any point has not been visited, return false
        if (arr[i].visited == false) {
            return false;
        }
    }
    // Otherwise, return true
    return true;
}
