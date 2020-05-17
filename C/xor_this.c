/**
 * Encrypts the string provided by argv[1] using a xor incremental encryption
 * The key is provided by argv[2] and it needs to be an integer.
 * 
 *	EXAMPLE:
 *
 *	./xor_this hello 1
 *
 * 		h e l l o
 * XOR  1 2 3 4 5
 *      ---------
 *      i d m m n
 *
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char **argv){
	if(argc < 3){
		printf("USAGE: %s <clear_string> <initial_key>\n", argv[0]);
		return 1;
	}

	char *s = argv[1];
	char *tmp = s;
	int initial = atoi(argv[2]);

	for(; *s != '\0'; s++){
		*s = *s ^ initial;
	}

	printf("%s\n", tmp);

	return 0;
}
