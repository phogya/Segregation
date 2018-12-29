# Segregation

A C program written for Mechanics of Programming. Simulates the segregation of two different groups based on their preference to be either surrounded by those that are similar or surrounded by no one. The program will display an ASCII representation of a collection of units and empty spaces. The two different groups of units are denoted by the plus, '+', symbol and the minus, '-', symbol. Empty spaces are denoted by the period, '.', symbol. The program will also display an interactive prompt to step through the segregation process and change variables. Stepping through the segregation process will cause units to move based on their satisfaction. Satisfaction is given by the number of neighbors of the same group divided by the total number of neighbors. In the case that there are no neighbors satisfaction is 1. A unit will move if their satisfaction value is below the given satisfaction argument.

### Pre-Segregation Prompt:
![Unsegregated Prompt](images/unsegregatedprompt.PNG)

### Post-Segregation Prompt:
![Segregated Prompt](images/segregatedprompt.PNG)

## Usage

For help compiling and running C programs on a windows machine see instructions [here](https://docs.microsoft.com/en-us/cpp/build/walkthrough-compile-a-c-program-on-the-command-line?view=vs-2017).

For help compiling and running C programs on a linux machine see instructions [here](http://webhotel4.ruc.dk/~keld/teaching/CAN_e14/Readings/How%20to%20Compile%20and%20Run%20a%20C%20Program%20on%20Ubuntu%20Linux.pdf)

Depending on your environment you can run the program with something like this:

    ./segregtion <size> <satisfaction> <vacancy> <proportion>
  
 where size is the size of the dimensions of the grid, from 5 to 39, satisfaction is the minimum satisfaction for a unit to not move, from 1 to 99, vacancy is the precentage of the grid that is occupied by empty units, from 1 to 99, and proportion is the percentage of the grid that is occupied by '+' units, from 1 to 99.
