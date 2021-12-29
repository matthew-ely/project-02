#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define BUFF 1048576 // define buffer size

/**
 * Head class prints the first n lines or c bytes of a file as selected by user input. Multiple
 * files can be printed. If '-' or no file argument, standard input is assumed and printed.
 */
int main(int argc, char* args[]) {
    // declare values
    int i = 1;
    int lines = 0;
    int bytes = 0;
    int byte = 0;
    bool l = true;
    bool noArg = false;
    char buf[BUFF];
    bool op = false;
    bool mult = false;

    if (argc > 2) { // checks if argument and number supplied
        if (strcmp(args[i], "-n") == 0) { // if argument is -n
            lines = atoi(args[i+1]); // set line count
            op = true;
            if (lines < 1) { // checks if line count is above 0
                setbuf(stdout, NULL);
                printf("ERROR: Line count must be a number greater than 0\n");
                return EXIT_FAILURE;
            } // if
        } else if (strcmp(args[i], "-c") == 0) { // if argument is -c
            bytes = atoi(args[i+1]); // sets byte count
            byte = bytes;
            op = true;
            if (bytes < 1) { // checks if byte count is above 0
                setbuf(stdout, NULL);
                printf("ERROR: Bytes count must be a number greater than 0\n");
                return EXIT_FAILURE;
            } // if

            l = false; // sets bool value to look for bytes not lines
        } // if
    } // if
    if (op) { // if option is selected skips index + 2
        i += 2;
    } else {
        lines = 10; // if no option, lines automatically set to 10
    } // if
    if (i == argc) { // if no argument and no file
        argc++;
        noArg = true;
    } // if
    if (argc - i > 1) { // checks if multiple files are supplied
        mult = true;
    } // if

    char* file;
    for (int j = i; j < argc; j++) { // loops through files

        if (!noArg) { // sets file if file argument supplied
            file = args[j];
        } // if
        if ((noArg)||(*file == '-')) { // chooses standard input
            //  char b[BUFF];
            if ((mult)&&(*file == '-')) { // print if file is '-'
                write(STDOUT_FILENO, "==> ", 4);
                write(STDOUT_FILENO, "standard input", 14);
                write(STDOUT_FILENO, " <==\n", 5);
            } // if
            if (l) { // if line option selected
                int n;
                int linesW = 0;
            out:
                while((linesW < lines)&&((n = read(STDIN_FILENO, buf, BUFF)) > 0)) {
                    if (n == -1) {
                        perror(file);
                        continue;
                    } // if
                    if (linesW < lines) {
                        int wr = write(STDOUT_FILENO, buf, n); // writes lines
                        if (wr == -1) {
                            perror(file);
                            continue;
                        } // if

                        linesW++; // increments line count
                    } else {
                        goto out;
                    } // if
                } // while

            } else { // if bytes option selected
                int n;
                int toWrite = bytes;
                while ((toWrite > 0)&&((n = read(STDIN_FILENO, buf, BUFF)) > 0)) { // reads lines
                    if (n == -1) {
                        perror(file);
                        continue;
                    } // if
                    if (n > toWrite) { // ensures not to try and write more bytes than byte count
                        n = toWrite;
                    } // if
                    int wr = write(STDOUT_FILENO, buf, n); // writes lines
                    if (wr == -1) {
                        perror(file);
                        continue;
                    } // if
                    toWrite -= n; // assigns how many more bytes should be written
                } // while
            } // if
            if (j != argc - 1) {
                write(STDOUT_FILENO, "\n", 1); // prints new line
            } // if
        } else {
            int fd = open(file, O_RDONLY); // opens file
            if (fd == -1) {
                perror(file);
                write(1, "\n", 1);
                continue;
            } // if
            int n = read(fd, buf, BUFF); // reads file
            if (n == -1) {
                perror(file);
                continue;
            } // if
            off_t size = lseek(fd, 0, SEEK_END); // gets size of file in bytes
            if (size == -1) {
                perror(file);
                continue;
            } // if
            if (bytes > size) { // ensures bytes selected dont exceed size of file
                bytes = size;
            } // if
            if (l) { // if lines selected
                for (int i = 0, j = 0; (i < size)&&(j < lines); i++) { // counts amount of newlines
                    if (buf[i] == '\n') {
                        j++;
                    } // if
                    bytes++; // counts bytes
                } // for
            } // if
            if (mult) { // if multiple files prints header
                setbuf(stdout, NULL);
                printf("==> %s <==\n", file);
            } // if
            int wr = write(STDOUT_FILENO, buf, bytes); // writes first n bytes
            if (wr == -1) {
                perror(file);
                continue;
            } // if
            if (j != argc - 1) {
                write(STDOUT_FILENO, "\n", 1); // prints newline
            } // if
            int cl = close(fd); // close file
            if (cl == -1) {
                perror(file);
                continue;
            } // if
            bytes = byte; // resets byte count for next file
        } // if
    } // for
} // main
