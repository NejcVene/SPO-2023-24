#include <stdio.h>
#include <stdlib.h>

#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }

void createFile();
int bytesToRead(FILE *);

int main(int argc, char **argv) {

    // check for arguments
    if (argc < 2) {
        // create "stevila.bin" if there are none
        createFile();
        err("To few arguments. Created file", "stevila")
    }

    // open file specified by user
    FILE *f = fopen(argv[1], "rb");
    if (!f) {
        err("Could not open", argv[1])
    }

    // get first number that inicated how many numbers are there to read
    int toRead = bytesToRead(f);
    float buffer[toRead];

    // read all the numbers into buffer
    if (fread(buffer, sizeof(buffer), 1, f) != 1) {
        err("Could not read file", argv[1])
    }
    // print buffer content
    for (int i = 0; i<toRead; i++) {
        printf("%.2f\n", buffer[i]);
    }
    // close file
    if (fclose(f) == EOF) {
        err("Could not close file", argv[1])
    }
 
    return 0;

}

void createFile() {

    // create or rewrite "stevila.bin"
    FILE *f = fopen("stevila", "wb");
    if (!f) {
        err("Could not open file", "stevila")
    }

    // write content of this array into the file
    int lines[] = {5};
    if (fwrite(lines, sizeof(lines), 1, f) != 1) {
        err("Could not write to file", "stevila")
    }
    float content[] = {1.11, 2.22, 3.33, 4.44, 5.55};
    if (fwrite(content, sizeof(content), 1, f) != 1) {
        err("Could not write to file", "stevila")
    }
    if (fclose(f) == EOF) {
        err("Could not close file", "stevila")
    }

}

int bytesToRead(FILE *f) {

    // get the first number from the file, which is an integer
    int buf[1];
    if (fread(buf, sizeof(buf), 1, f) != 1) {
        err("Could not get first byte of file", "bytesToRead")
    }

    return buf[0];

}