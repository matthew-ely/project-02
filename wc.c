#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

#define BUFF 1048576 // defines buffer size

/**
 * WC class is a tool that returns lines/words/byte count of files as specified
 * by the input. Multiple file properties can be printed. Any combination of '-wlc'
 * will return words, lines, and/or byte count. If file is '-' or no file specified,
 * standard input is assumed and their counts are printed to standard output.
 */
int main(int argc, char* args[]) {
    // declare variables
    bool empty = false;
    bool l = false;
    bool w = false;
    bool b = false;
    bool op = false;
    int btot = 0;
    int ltot = 0;
    int wtot = 0;
    int j = 1;
    int dashes = 0;

    int o;
    // parses through the options listed
    while ((o = getopt(argc, args, "wlc")) != -1) {
        switch(o) {
        case 'w': // if words option
            w = true;
            op = true;

            break;
        case 'l': // if lines option
            l = true;
            op = true;

            break;
        case 'c': // if bytes option
            b = true;
            op = true;
            break;
        default: // if wrong argument
            setbuf(stdout, NULL);
            printf("ERROR: Invalid Argument:\n");
            return EXIT_FAILURE;
        } // switch
    } // while

    if (!op) { // if no options selected, sets all to true
        l = true;
        w = true;
        b = true;
    } // if

    if (argc == 1) { // if no file arguments nor options
        empty = true;
        argc++;
    } // if

    char* file;
    char buf[BUFF];
    for (int i = j; i < argc; i++) { // loops through files

        int bytes = 0;
        int lines = 0;
        int words = 0;
        int fd;

        if (!empty) { // if not empty set file
            file = args[i];
        } // if

        if ((empty)||(*file == '-')) { // if empty or '-' check stdin
            if (!empty&&!(strcmp(file, "-") == 0)) { // doesn't confuse -wlc for stdin
                dashes++;
                if (dashes + 1 == argc) {
                    argc++;
                    empty = true;
                } // if
                continue;
            } // if
            fd = creat("-", 0644);
            int rea;
            while ((rea = read(STDIN_FILENO, buf, BUFF)) > 0) { // reads stdin
                write(fd, buf, rea/*strlen(buf2)*/); // writes stdin to a file
            } // while
            file = "-";
        } // if
        fd = open(file, O_RDONLY); // opens file
        if (fd == -1) {
            perror(file);
            continue;
        } // if
        int n = read(fd, buf, BUFF); // reads file
        if (n == -1) {
            perror(file);
            continue;
        } // if
        off_t size = lseek(fd, 0, SEEK_END); // finds length of file in bytes
        if (size == -1) {
            perror(file);
            continue;
        } // if
        bytes = size;
        btot += bytes;
        char lastchar = '\n';
        for (int i = 0; i < size; i++) {
            if (buf[i] == ' ') { // finds words
                words++;
            } // if
            if (buf[i] == '\n') { // finds lines/words
                lines++;
                if (buf[i] != lastchar) {
                    words++;
                } // if
            } // if
            lastchar = buf[i]; // checks last character to not miscount words
        } // for
        ltot += lines;
        wtot += words;

        if (l) { // prints lines
            printf(" %d", lines);
        } // if
        if (w) { // prints words
            printf(" %d", words);
        } // if
        if (b) { // prints byte count
            printf(" %d", bytes);
        } // if
        if (!empty) { // prints file name
            printf(" %s\n", file);
        } else { // prints newline
            printf("\n");
        } // if
        int cr = close(fd); // closes file
        if (cr == -1) {
            perror(file);
            continue;
        } // if

    } // for
    if (argc - dashes - 1 > 1) { // if multiple files
        if (l) { // prints lines total
            printf(" %d", ltot);
        } // if
        if (w) { // prints words total
            printf(" %d", wtot);
        } // if
        if (b) { // prints total byte count
            printf(" %d", btot);
        } // if
        printf(" total\n");
    } // if
    remove("-"); // remove temporary file used
    return EXIT_SUCCESS;
} // main
