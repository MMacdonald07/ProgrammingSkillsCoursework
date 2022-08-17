#include <stdio.h>
// Necessary import for rand() and srand() functions
#include <stdlib.h>

// Defines boolean enumeration for more elegant checking of whether points have been visited
typedef enum {
    false = 0,
    true = 1
} boolean;

// Declares structure which will store 1-dimensional point and a "boolean" value for whether the point has been traversed
typedef struct {
    int x;
    boolean visited;
} point;

// Declaration of a function that returns a random integer
// In the range 0...(max-1)
int randomInt(int max);

// Declaration of a function that returns either true or false dependent on if all points in an array have been visited
boolean arrTrue(int num, point arr[]);

int main() {
    // Initialise variables for incrementation and number of sites
    int i, j, n, loop_counter;
    // Initialise variable for maximum random generated number: 2 here as particle can either move back or forwards
    int nmax=2;
    int rand_num, lower, upper, seed;
    float average;
    FILE *fp;

    // File to write average number of iterations to traverse each site for each n
    char *filename = "iterations_Q1.csv";
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

    // Loops through n=2 to n=400
    for (n=2;n<=400;n++) {
        // Creates 1-D array of n Points
        point grid[n];
        // Resets value of total for each number of sites loop
        int total = 0;

        // Loops through seed values from user input
        for (seed=lower;seed<=upper;seed++) {
            // Set up the random number generator
            srand(seed);

            // Resets loop counter for how many loops have been performed to traverse all sites
            loop_counter = 0;

            // Creates a grid of n Points where x increases linearly e.g. grid[3] will contain x=3
            for (i=0;i<n;i++) {
                grid[i].x = i + 1;
                // Sets all points to be unvisited
                grid[i].visited = false;
            }

            // As starting on very first grid point, sets its visited value to be true
            grid[0].visited = true;

            // Initialises particle pointer to first element in grid (x=1)
            point *particle = grid;

            do {
                //Generates random integer of 0 or 1 - 0 to move backwards and 1 to move forwards
                rand_num = randomInt(nmax);

                // Particle moves backwards
                if (rand_num == 0) {
                    // If particle at first point, must apply periodic boundary here
                    if (particle->x == 1) {
                        // Particle moves to final point in grid
                        particle += (n-1);
                    } else {
                        // Otherwise, address is decremented to point to previous index in grid
                        particle--;
                    }
                    // Particle moves forwards
                } else if (rand_num == 1) {
                    // If particle at final point in grid, must be moved to first point
                    if (particle->x == n) {
                        // Inverse of earlier periodic condition
                        particle -= (n-1);
                    } else {
                        // Else move forward by incrementing pointer address
                        particle++;
                    }
                    // If (for some unknown reason) rand_num is not 0 or 1, loop continues to next random number generation
                    // and loop_counter is not incremented
                } else {
                    continue;
                }

                // If particle.visited for particle's new position is false, this needs to be changed to true for grid array
                if (particle->visited == false) {
                    // By doing particle - grid (defaults to grid[0]), program performs memory arithmetic to calculate
                    // the index of the grid which particle is currently pointing to
                    grid[(particle - grid)].visited = true;
                }

                loop_counter++;
                // Ends the loop when all points have been visited
            } while (arrTrue(n, grid) != true);

            // Increments total by however many loops were required to visit every point in grid
            total += loop_counter;
        }

        // Calculates a floating point average of how many loops were required to visit every point
        // Cast integer values as floats to ensure correct average calculated
        // Program takes average of seeds including both upper and lower bounds so add one to total number of measurements
        average = (float) total / (float) (upper + 1 - lower);
        // Saves this value and number of sites to .csv file
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
boolean arrTrue(int num, point arr[]) {
    // For every point in grid array
    for (int i=0;i<num;i++) {
        // If a single point has not been visited, return false
        if (arr[i].visited == false) {
            return false;
        }
    }
    // Else return true
    return true;
};
