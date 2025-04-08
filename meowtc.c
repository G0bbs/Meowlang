#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

enum Operation {
    INCR = '-', 
    DECR = '+', 
    LEFT = '<', 
    RIGHT = '>', 
    INPUT = ',', 
    OUTPUT = '.',
    LBRACK = '[',
    RBRACK = ']'
};

typedef struct {
    char last_read;
    int size;
    int offset;
    enum Operation m;
    enum Operation e;
    enum Operation o;
    enum Operation w;
} CharOperators;

void initOperators(CharOperators* ops);
bool parseFile(FILE* fileI, FILE* fileO, CharOperators* ops, enum Operation* ops_indexed, bool verbose);
void rotateOperators(CharOperators* ops, enum Operation* ops_indexed);
bool verifyMeow(char c, CharOperators* ops, bool verbose);

int main(int argc, char** argv) {
    if (argc <= 2) {
        printf("Use proper usage: meowtc <meow-file> <destination-file> [-v: verbose]\n");
        return 0;
    }
    bool verbose = false;
    if (argc > 2 && strcmp(argv[3], "-v") == 0) {
        verbose = true;
    }
    enum Operation ops_indexed[8] = {INCR, DECR, LEFT, RIGHT, INPUT, OUTPUT, LBRACK, RBRACK};
    CharOperators* ops = malloc(sizeof(CharOperators));
    if (ops == NULL) {
        printf("ERROR: malloc failed.\n");
        return 1;
    }

    initOperators(ops);

    char* meowFile = argv[1];
    char* outFile = argv[2];
    FILE* fileI = fopen(meowFile, "r");
    FILE* fileO = fopen(outFile, "w");
    if (fileI == NULL || fileO == NULL) {
        printf("ERROR: cannot open files. In: %s, Out %s.\n", meowFile, outFile);
        fclose(fileI);
        fclose(fileO);
        free(ops);
        return 1;
    }

    bool success = parseFile(fileI, fileO, ops, ops_indexed, verbose);
    if (!success) {
        printf("Unable to properly parse file! Rule broken!\n");
        fclose(fileO);
        FILE* fileO = fopen(outFile, "w"); // Clear file
    }
    
    fclose(fileI);
    fclose(fileO);
    free(ops);
    return 0;
}

// ops initializer
void initOperators(CharOperators* ops) {
    ops->size = 8;
    ops->offset = 0;
    ops->last_read = '\0';
    ops->m = INCR;
    ops->e = DECR;
    ops->o = LEFT;
    ops->w = RIGHT;
}

// Parse input and write to output. Return false if meow rule has been broken.
bool parseFile(FILE* fileI, FILE* fileO, CharOperators* ops, enum Operation* ops_indexed, bool verbose) {
    char c = fgetc(fileI);
    if (feof(fileI)) {
        printf("Input file is empty.\n");
        return true;
    }
    if (verbose) printf("In: {\n");
    while (c) {
        if (feof(fileI)) {
            break;
        }
        if (verbose) printf("%c", c);
        switch (c) {
            case 'm': {
                fprintf(fileO, "%c", (char)ops->m);
                if (!verifyMeow(c, ops, verbose)) {return false;}
                break;
            }
            case 'e': {
                fprintf(fileO, "%c", (char)ops->e);
                if (!verifyMeow(c, ops, verbose)) {return false;}
                break;
            }
            case 'o': {
                fprintf(fileO, "%c", (char)ops->o);
                if (!verifyMeow(c, ops, verbose)) {return false;}
                break;
            }
            case 'w': {
                fprintf(fileO, "%c", (char)ops->w);
                if (!verifyMeow(c, ops, verbose)) {return false;}
                break;
            }
            case '!': {
                rotateOperators(ops, ops_indexed);
                break;
            }
            case '#': {
                char line[2048];
                fgets(line, 2048, fileI);
                fprintf(fileO, "%s", line);
                break;
            }
            default: {
                fprintf(fileO, "%c", c);
                break;
            }
        }
        c = fgetc(fileI);
    }
    if (verbose) printf("}\n");
    return true;
}

// "Rotate" what each operator turns into
void rotateOperators(CharOperators* ops, enum Operation* ops_indexed) {
    ops->offset++;
    int mi = ops->offset % ops->size ;
    int ei = (ops->offset + 1) % ops->size;
    int oi = (ops->offset + 2) % ops->size;
    int wi = (ops->offset + 3) % ops->size;
    ops->m = ops_indexed[mi];
    ops->e = ops_indexed[ei];
    ops->o = ops_indexed[oi];
    ops->w = ops_indexed[wi];
}

// Verify that we only meows are being written
bool verifyMeow(char c, CharOperators* ops, bool verbose) {
    if (verbose) {
        printf("\nlast-read: %c, current in: %c.\n", ops->last_read, c);
    }
    // Allow repeats:
    // if (ops->last_read == c) {
    //     return true;
    // }
    if (ops->last_read == '\0' && c == 'm') {
        ops->last_read = 'm';
        return true;
    }
    if (ops->last_read == 'm' && c == 'e') {
        ops->last_read = 'e';
        return true;
    }
    if (ops->last_read == 'e' && c == 'o') {
        ops->last_read = 'o';
        return true;
    }
    if (ops->last_read == 'o' && c == 'w') {
        ops->last_read = 'w';
        return true;
    }
    if (ops->last_read == 'w' && c == 'm') {
        ops->last_read = 'm';
        return true;
    }
    printf("ERROR: All characters must be in the form: m!e!o!w!\n");
    return false;
}


