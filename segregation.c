// Segregation Project
//
// Randomly arranges a supplied number of rich and poor units
// about a grid. The program then segregates the units based 
// on a supplied satisfaction value. The program runs in two
// modes, interactive and print. Print mode accepts an 
// additional command line argument and prints the specified
// number of steps and various info regardin those steps.
// Interactive mode allows to user to control the steps and
// change the variables used for the simulation.
//
// Author: pmh6003
//

#define _BSD_SOURCE

#include "display.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <ctype.h>
#include "check_input.h"

#define MAX_DIGITS 22

static size_t VERBOSE = 0;

static size_t size = 10;

void printInfo( int, char[20][20], float, int, double, int, int, int, int, int );
int relocate(int, char [20][20], double, float *);
void populate( int, char[20][20], int , int );

// Parses the arguments and runs in interactive or print mode
// based on the arguments.
int main(int argc, char * argv[]) {
			
	int opt;
	int tmpsize = 0;
	int *printmode = NULL;

	if( argc != 5 && argc != 6 ) {
		fprintf(stderr, "usage segregation [-pN] size satisfaction vacancy proportion\n" );
		exit(EXIT_FAILURE);
	}

	while( (opt = getopt( argc,argv, "vh:p:") ) != -1 ) {

		switch( opt ) {
			case 'v':
				VERBOSE = 1;
				break;
			case 'p':
				tmpsize = (int)strtol( optarg,NULL,10);
				printmode = &tmpsize;
				break;
			case 'h':
				tmpsize = (int)strtol( optarg,NULL,10);
				if( tmpsize>0) {
					size = (size_t)tmpsize;
					break;
				}
			default:
				fprintf(stderr, "usage segregation [-pN] size satisfaction vacancy proportion\n" );
				exit(EXIT_FAILURE);
		}
	}
		
	int arrsize = atoi(argv[optind]);
	double satisfaction = ( atoi(argv[optind+1]) / 100.0 );
	int vacancy = atoi(argv[optind+2]);
	int proportion = atoi(argv[optind+3]);	 	
	int pop = ((arrsize * arrsize) * ((100.0 - vacancy) / 100.0));

	//Invalid arguments
	if( arrsize < 5 || arrsize > 39 ) {
		fprintf(stderr, "size (%d) must be an integer in [5...39]\n", arrsize);
		fprintf(stderr, "usage segregation [-pN] size satisfaction vacancy proportion\n");
		exit(EXIT_FAILURE);
	}
	if(  (atoi(argv[optind+1])) < 1 || (atoi(argv[optind+1])) > 99 ) {
		fprintf(stderr, "satisfaction (%d) must be an integer in [1...99]\n", (atoi(argv[optind+1])));
		fprintf(stderr, "usage segregation [-pN] size satisfaction vacancy proportion\n" );
		exit(EXIT_FAILURE);
	}
	if( vacancy < 1 || vacancy > 99 ) {
		fprintf(stderr, "vacancy (%d) must be an integer in [1...99]\n", vacancy);
		fprintf(stderr, "usage segregation [-pN] size satisfaction vacancy proportion\n" );
		exit(EXIT_FAILURE);
	}
	if( proportion < 1 || proportion > 99 ) {
		fprintf(stderr, "proportion (%d) must be an integer in [1...99]\n", proportion);
		fprintf(stderr, "usage segregation [-pN] size satisfaction vacancy proportion\n" );
		exit(EXIT_FAILURE);
	}

	char Seg[arrsize][arrsize];

	populate( arrsize, Seg, vacancy, proportion );

	float* segMeasure;
	float segMeasureInterim = 0;
	segMeasure = &segMeasureInterim;
	int numMoves = 0; 

	int p = 0;	
	if( printmode != NULL ) {
		while( p <= *printmode ) {
			
			relocate( arrsize, Seg, 0, segMeasure);	//get segMeasure w/o actually relocating
			printInfo( arrsize, Seg, *segMeasure, pop, satisfaction, vacancy, proportion, p, numMoves, 0);
			numMoves = relocate( arrsize, Seg, satisfaction, segMeasure);	
			p++;
		}
		return 0;
	} 
		
	clear();
	set_cur_pos( 1, 0);
	relocate( arrsize, Seg, 0, segMeasure);	//get segMeasure w/o actually relocating
	printInfo( arrsize, Seg, *segMeasure, pop, satisfaction, vacancy, proportion, p, numMoves, 1);
	printf("\nstate: paused\n(/)stop/go, (.)step (r)eset, (s)at N, (v)ac N, (p)rop N, (q)uit, (i)nfo,\n(h)elp\n> ");
	fflush(stdout); 

	int state = 0; //Paused state = 0, running state = 1

	char buf[MAX_DIGITS];
	
	for( int j=0; ; ++j) {
					
		int check = check_input( stdin, buf, MAX_DIGITS, 4); // 4 seconds
	
		switch( check ) {
		case -1:
			if( feof( stdin ) ) {
				printf( "test_check received endof file\n");
				return 0;
			}
			if( ferror( stdin ) ) {
				printf( "test_check clearing error\n" );
				clearerr( stdin );
			}
			perror( "select" );
			errno = 0;
			break;
		case 0:
			if( state == 1 ) {
				
				clear();
				set_cur_pos( 1, 0);
				p++;
				numMoves = relocate( arrsize, Seg, satisfaction, segMeasure );
				relocate( arrsize, Seg, 0, segMeasure);	//get segMeasure w/o actually relocating
				printInfo( arrsize, Seg, *segMeasure, pop, satisfaction, vacancy, proportion, p, numMoves, 1);
				
				printf("\nstate: running\n(/)stop/go, (.)step (r)eset, (s)at N, (v)ac N, (p)rop N, (q)uit, (i)nfo,\n(h)elp\n> ");
				fflush(stdout);
			}
			break;
		default:
			if( buf[0] == 's' ) {

				char* str = buf;

				while(*str && !(isdigit(*str) || ((*str == '-' || *str == '+' ) && isdigit(*(str+1))))) {
					str++; }
					
				if( (strtod(str, NULL)) < 1 || (strtod(str, NULL)) > 99 ) {
					printf("> ");
					fflush(stdout);
				} else {
					satisfaction = ( strtod(str,NULL) / 100.0 );
					printf("> ");
					fflush(stdout);
				}
			}
			if( buf[0] == 'v' ) {
					
				char* str = buf;

				while(*str && !(isdigit(*str) || ((*str == '-' || *str == '+' ) && isdigit(*(str+1))))) {
					str++; }
				
				if( (strtod(str, NULL)) < 1 || (strtod(str, NULL)) > 99 ) {
					printf("> ");
					fflush(stdout);
				} else {
					vacancy = strtod(str, NULL);
					pop = ((arrsize * arrsize) * ((100.0 - vacancy) / 100.0));
	
					clear();
					set_cur_pos( 1, 0);
					state = 0;
					p = 0;
					numMoves = 0;
					populate( arrsize, Seg, vacancy, proportion);		
					relocate( arrsize, Seg, 0, segMeasure);	//get segMeasure w/o actually relocating
					printInfo( arrsize, Seg, *segMeasure, pop, satisfaction, vacancy, proportion, p, numMoves, 1);
					printf("\nstate: paused\n(/)stop/go, (.)step (r)eset, (s)at N, (v)ac N, (p)rop N, (q)uit, (i)nfo,\n(h)elp\n> ");
					fflush(stdout); 
				}
			}
			if( buf[0] == 'p' ) {

				char* str = buf;

				while(*str && !(isdigit(*str) || ((*str == '-' || *str == '+' ) && isdigit(*(str+1))))) {
					str++; }
			
				if( (strtod(str, NULL)) < 1 || (strtod(str, NULL)) > 99 ) {
					printf("> ");
					fflush(stdout);
				} else {
					proportion = strtod(str,NULL);
	
					clear();
					set_cur_pos( 1, 0);
					p = 0;
					numMoves = 0;
					state = 0;
					populate( arrsize, Seg, vacancy, proportion);
					relocate( arrsize, Seg, 0, segMeasure); //get segMeasure w/0 actually relocating
					printInfo( arrsize, Seg, *segMeasure, pop, satisfaction, vacancy, proportion, p, numMoves, 1);		
					printf("\nstate: paused\n(/)stop/go, (.)step (r)eset, (s)at N, (v)ac N, (p)rop N, (q)uit, (i)nfo,\n(h)elp\n> ");
					fflush(stdout);
				} 
			} 
			if( buf[0] == 'i' ) {
								
				printf("size %d, satisfaction %.2f, vacancy %.2f, proportion %.2f\n> ", arrsize, satisfaction, (vacancy/100.0), (proportion/100.0));
				fflush(stdout);
			}
			if( buf[0] == 'q' ) {
				return 0;
			}
			if( buf[0] == '/' ) {
				if( state == 0 ) {
					clear();
					set_cur_pos(1, 0);
					state = 1;
					printInfo( arrsize, Seg, *segMeasure, pop, satisfaction, vacancy, proportion, p, numMoves, 1);
					printf("\nstate: running\n(/)stop/go, (.)step (r)eset, (s)at N, (v)ac N, (p)rop N, (q)uit, (i)nfo,\n(h)elp\n> ");
					fflush(stdout);
				} else {
					clear();
					set_cur_pos(1, 0);
					state = 0;
					printInfo( arrsize, Seg, *segMeasure, pop, satisfaction, vacancy, proportion, p, numMoves, 1);
					printf("\nstate: paused\n(/)stop/go, (.)step (r)eset, (s)at N, (v)ac N, (p)rop N, (q)uit, (i)nfo,\n(h)elp\n> ");
					fflush(stdout);
				}
				//printf("> ");
				//fflush(stdout);
			}
			if( buf[0] == '.' ) {
				clear();
				set_cur_pos( 1, 0);
				state = 0;	
				p++;
				numMoves = relocate( arrsize, Seg, satisfaction, segMeasure );
				relocate( arrsize, Seg, 0, segMeasure);	//get segMeasure w/o actually relocating
				printInfo( arrsize, Seg, *segMeasure, pop, satisfaction, vacancy, proportion, p, numMoves, 1);
				printf("\nstate: paused\n(/)stop/go, (.)step (r)eset, (s)at N, (v)ac N, (p)rop N, (q)uit, (i)nfo,\n(h)elp\n> ");
				fflush(stdout); 
			}
			if( buf[0] == 'r' ) {
				clear();
				set_cur_pos( 1, 0);
				p = 0;
				state = 0;
				numMoves = 0;
				populate( arrsize, Seg, vacancy, proportion);
				relocate( arrsize, Seg, 0, segMeasure); //get segMeasure w/0 actually relocating
				printInfo( arrsize, Seg, *segMeasure, pop, satisfaction, vacancy, proportion, p, numMoves, 1);		
				printf("\nstate: paused\n(/)stop/go, (.)step (r)eset, (s)at N, (v)ac N, (p)rop N, (q)uit, (i)nfo,\n(h)elp\n> ");
				fflush(stdout); 
			}
			if( buf[0] == 'h' ) {
				printf("(/)stop/go, (.)step (r)eset, (s)at N, (v)ac N, (p)rop N, (q)uit, (i)nfo,\n(h)elp\n> ");
				fflush(stdout);
			}
		break;
		}
	}	
	return 0; 
}

// The move algorithm for the simulation. Moves units to the next available spot by marking the spot
// they move from with a 'p' for a poor unit and an 'r' for a rich unit and the spot they move to
// with a 'P' for a poor unit and a 'R' for a rich unit. Also helps determine the segregation measure
// and the number of units moved
//
// @param size		-the size of the grid
// @param Seg		-the grid to move units around
// @param satisfaction	-the satisfaction threshold to move units around
// @param segMeasure	-the value for segregation measure to return
// @return int		-the number of units moved
//
int relocate( int size, char Seg[size][size], double satisfaction, float * segMeasure ) {

	int row, col, row2, col2;
	double richNeighbors = 0;
	double poorNeighbors = 0;
	double currentSat;
	
	int numMoves = 0;
	*segMeasure = 0;

	for(row=0; row<size; row++) {
		for(col=0; col<size; col++) {
		
			poorNeighbors = 0;
			richNeighbors = 0;
			currentSat = 0;

			if( Seg[row][col] == '+' || Seg[row][col] == '-' ) {
			
				// Check for edge cases
				if( (row-1) >= 0 ) {
					if( (col-1) >= 0 ) {

						if( Seg[row-1][col-1] == '+' || Seg[row-1][col-1] == 'r') {						
							richNeighbors++; }
						if( Seg[row-1][col-1] == '-' || Seg[row-1][col-1] == 'p') {
							poorNeighbors++; }
					}	
					if( (col+1) <= size ) {

						if( Seg[row-1][col+1] == '+' || Seg[row-1][col+1] =='r') {
							richNeighbors++; }
						if( Seg[row-1][col+1] == '-' || Seg[row-1][col+1] == 'p') {
							poorNeighbors++; }
					}

					if( Seg[row-1][col] == '+' || Seg[row-1][col] == 'r') {
						richNeighbors++; }
					if( Seg[row-1][col] == '-' || Seg[row-1][col] == 'p') {
						poorNeighbors++; }
				}

				if( (row+1) <= size ) {
					if( (col-1) >= 0 ) {

						if( Seg[row+1][col-1] == '+' || Seg[row+1][col-1] == 'r') {
							richNeighbors++; }
						if( Seg[row+1][col-1] == '-' || Seg[row+1][col-1] == 'p') {
							poorNeighbors++; }
					}
					if( (col+1) <= size ) {
		
						if( Seg[row+1][col+1] == '+' || Seg[row+1][col+1] == 'r') {
							richNeighbors++; }
						if( Seg[row+1][col+1] == '-' || Seg[row+1][col+1] == 'p') {
							poorNeighbors++; }
					}

					if( Seg[row+1][col] == '+' || Seg[row+1][col] == 'r') {
						richNeighbors++; }
					if( Seg[row+1][col] == '-' || Seg[row+1][col] == 'p') {
						poorNeighbors++; }
				}
			
				if( (col+1) <= size ) {

					if( Seg[row][col+1] == '+' || Seg[row][col+1] == 'r') {
						richNeighbors++; }
					if( Seg[row][col+1] == '-' || Seg[row][col+1] == 'p') {
						poorNeighbors++; }
				}

				if( (col-1) >= 0 ) {
		
					if( Seg[row][col-1] == '+' || Seg[row][col-1] == 'r') {
						richNeighbors++; }
					if( Seg[row][col-1] == '-' || Seg[row][col-1] == 'p') {
						poorNeighbors++; }
				}	

				//Rich Neighbor
				if( Seg[row][col] == '+') {
					
					if( (richNeighbors + poorNeighbors) == 0 ) {
						currentSat = 1.0;
					} else {
						currentSat = (richNeighbors/(poorNeighbors + richNeighbors));	
					}
					*segMeasure += currentSat;					
	
					//Rich Neighbor needs to move
					if( currentSat < satisfaction ) {
						for(row2=0; row2<size; row2++) {
							for(col2=0; col2<size; col2++) {
								if( Seg[row2][col2] == '.') {
									Seg[row2][col2] = 'R'; //Rich neighbor will move IN here
									Seg[row][col] = 'r';   //Rich neighbor will move FROM here
									numMoves += 1;

									//break for loops
									col2 = size;
									row2 = size;
								}
							}
						}
					}
				}

				//Poor Neighbor
				if( Seg[row][col] == '-') {
						
					if( (richNeighbors + poorNeighbors) == 0 ) {
						currentSat = 1.0;
					} else {
						currentSat = (poorNeighbors/(poorNeighbors + richNeighbors));
					}
					*segMeasure += currentSat;					

					//Poor Neighbor needs to move
					if( currentSat < satisfaction ) {
						for(row2=0; row2<size; row2++) {
							for(col2=0; col2<size; col2++) {
								if( Seg[row2][col2] == '.') {
									Seg[row2][col2] = 'P'; //Poor neighbor will move IN here
									Seg[row][col] = 'p';   //Poor neighbor will move OUT here
									numMoves += 1;	
		
									//break for loops
									col2 = size;
									row2 = size;
								}
							}
						}
					}
				}
			}
		}
	}
	
	for(row=0; row<size; row++) {
		for(col=0; col<size; col++) {
	
			if( Seg[row][col] == 'r') { 
				Seg[row][col] = '.'; }
			if( Seg[row][col] == 'p') {
				Seg[row][col] = '.'; }

			if( Seg[row][col] == 'R') {
				Seg[row][col] = '+'; }
			if( Seg[row][col] == 'P') {
				Seg[row][col] = '-'; }
		}
	}
	return numMoves;
}

// Populates a supplied grid with poor and rich units using the supplied 
// specifications.
//
// @param arrsize		-the size of the grid
// @param Seg			-the grid to populate
// @param vacancy		-the ratio of empty spots in the grid
// @param proportion		-the proportion of rich to poor units
// @return			void
//
void populate( int arrsize, char Seg[arrsize][arrsize], int vacancy, int proportion ) {
	
	int pop = ((arrsize * arrsize) * ((100.0 - vacancy) / 100.0));
	int rich = ((proportion / 100.0) * pop);	

	

	for(int i=0; i<arrsize; i++) {
		for(int j=0; j<arrsize; j++) {
			Seg[i][j] = '.';
		}
	}
	
	srandom(41);		

	int row,col;	
	for(int i=1; i<=pop; i++) {
		row = rand();
		row %= arrsize;
		col = rand();
		col %= arrsize;

		if( Seg[row][col] == '+' || Seg[row][col] == '-') {
			i--;
		} else {
			if( i <= rich ) {
				Seg[row][col] = '+';
			} else {
				Seg[row][col] = '-';
			}
		}
	}

	for(int i=0; i<arrsize; i++) {
		for(int j=0; j<arrsize; j++) {
			if( Seg[i][j] != '+' && Seg[i][j] != '-' ) {
				Seg[i][j] = '.';
			}
		}
	}
}

// Prints the grid and various info about the grid and the parameters
// for the simulation
//
// @param size		-the size of the grid
// @param Seg		-the grid to print
// @param segMeasure	-the segregation measure based on the algorithm
// @param pop		-the population of the grid
// @param satisfaction	-the satisfaction threshold
// @param vacancy	-the ratio of empty spots in the grid
// @param proportion	-the ratio of rich to poor units
// @param cycle		-the number of steps in the simulation
// @param numMoves	-the number of units moved last step
// @param mode		-for print mode, 0, for interactive mode, 1
// @return		void
//
void printInfo( int size, char Seg[size][size], float segMeasure, int pop, double satisfaction, int vacancy, int proportion, int cycle, int numMoves, int mode) {


	for(int i=0; i<size; i++) {
		for(int j=0; j<size; j++) {
			printf("%c", Seg[i][j]);
		}
		puts(" ");
	}
	printf("cycle: %d\n", cycle);	
	printf("moves this cycle: %d\n", numMoves);
	printf("segregation measure: %f\n", (segMeasure/pop));
	if( mode == 0 ) {
		printf("size %d, satisfaction %.2f, vacancy %.2f, proportion %.2f\n", size, satisfaction, (vacancy/100.0), (proportion/100.0));
	}	
}
