//
// file: check_input.c
// Checks for availability of input on a file descriptor.
//
// code derived from http://developerweb.net/viewtopic.php?id=2933
// 2002 example by Hector Lasso, http://developerweb.net/profile.php?id=3756
//
// @author: bks, ben k steele
//

// need _POSIX_SOURCE for fileno function
#define _POSIX_SOURCE

#include <stdio.h>
#include <sys/select.h>
#include <string.h>
#include "check_input.h"

///
/// check input with a timeout and return 1 and a buffer of text, or
/// return 0 and -1 and an empty buffer '' on a timeout or an error.
///
int check_input( FILE * fp, char * buf, int len, int timeout ) {

	// Initialize the set
	int fd = fileno( fp );
	fd_set selected_set;
	FD_ZERO( &selected_set);
	FD_SET( fd, &selected_set);

	// Initialize timeout struct
	struct timeval tv;
	tv.tv_sec = timeout;
	tv.tv_usec = 0;
	
	buf[0] = '\0'; // init to return empty string (NUL) in case of no input

	int result = select( 1, &selected_set, NULL, NULL, &tv);
	
	if( result < 0 ) {
		perror( "select" );
		return -1;
	} else if( result > 0 ) {

		// get the input that is ready for consumption
		if( NULL == fgets( buf, len, fp ) ) {
			
			if(feof( fp ) ) {
			
				return -1;
			}
			if( ferror( fp ) ) {
				perror("error getting input with fgets");
			}
			return 0;
		}
	}
	if( buf[0] == '\n' ) {
		// parse the buffer using strtok
		char sep[] = " \t\n";
		char * sptr = strtok( buf, sep );
		for( int j = 1; sptr != NULL; ++j ) {
			printf( "%d: %s\n", j, sptr );
			sptr = strtok( NULL, sep );
		}

	}
	return result;
}
