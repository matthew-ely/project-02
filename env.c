#include <stdio.h>
#include <unistd.h>
#include <string.h>

/**
 * Env class prints out all currently set environmental variables to standard
 * output.
 */
int main(int argc, char* argv[], char* envp[]) {
    // prints environmental variables
	for (int i = 1; i < argc; i++) printf("%s\n", argv[i]);
	for (int i = 1; argv[i] != NULL; i++) printf("%s\n", argv[i]);
	for (int i = 0; envp[i] != NULL; i++) printf("%s\n", envp[i]);

} // main
