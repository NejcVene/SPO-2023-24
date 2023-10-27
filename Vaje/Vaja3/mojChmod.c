void callChmod(char *, mode_t);
void getFileStat(char *, struct stat*);
    // check for arguments
    mode_t mode = strtol(argv[1], &remainder, 8);
    // if mode is 0 then strtol() failed to convert to base 8, meaning (probably) that argv[1] is not a number
        // get file stat
        getFileStat(argv[2], &statBuf);
        // check for correct argument
        if (strcmp(argv[1], "u+s") == 0) {
            callChmod(argv[2], statBuf.st_mode | S_ISUID);
        } else if (strcmp(argv[1], "u-s") == 0) {
            callChmod(argv[2], statBuf.st_mode & ~S_ISUID);
        } else if (strcmp(argv[1], "g+s") == 0) {
            callChmod(argv[2], statBuf.st_mode | S_ISGID);
        } else if (strcmp(argv[1], "g-s") == 0) {
            callChmod(argv[2], statBuf.st_mode & ~S_ISGID);
        } else {
            err("Unknown argument", argv[1])
        // get new file stat and print it out
        getFileStat(argv[2], &statBuf);
        printMode(statBuf.st_mode);
        return 0;
    // if we are here, then strtol() converted a number to base 8
    // apply mode
    // get file stat
    getFileStat(argv[2], &statBuf);
    // print new file mode
        fileInfo[19] = 's';
        fileInfo[22] = 'S';
        fileInfo[25] = 'T';
void callChmod(char *file, mode_t mode) {
    // try to run chmod on file
}

void getFileStat(char *file, struct stat *statBuf) {

    // try to get stat for file
    if (stat(file, statBuf) < 0) {
        err("Could not stat file", file)
    }

}

/*
switch (argv[1][0]) {
            case 'u':
                callChmod(argv[2], (argv[1][1] == '+' && argv[1][2] == 's') ? (statBuf.st_mode | S_ISUID) : (statBuf.st_mode & ~S_ISUID));
                break;
            case 'g':
                callChmod(argv[2], (argv[1][1] == '+' && argv[1][2] == 's') ? (statBuf.st_mode | S_ISGID) : (statBuf.st_mode & ~S_ISGID));
                break;
            default:
                err("Unknown argument", argv[1])
                break;
        }
*/