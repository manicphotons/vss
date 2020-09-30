#include <stdio.h>
#include <stdlib.h>

#define IGNORE_INPUT( Arg ) ((void)Arg);

int
main( int ArgCount, char** ArgValues ){
	IGNORE_INPUT( ArgCount  );
	IGNORE_INPUT( ArgValues );

	printf( "%s\n", "Hello, World!" );

	return EXIT_SUCCESS;
}
