#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "encDec_funcs.h"

/* Defining the debug macro from the make file */
#ifdef DEBUG
#define DEBUG 1
#else
#define DEBUG 0
#endif

/* Malloc Function in the form of Steven-styled wrap */
/* in: bytes to allocate; return: ptr of type void to allocated memory */
void *Malloc(size_t numBytes) {                                             
    void *ptr = NULL;                                                 
    if ((ptr = malloc(numBytes)) == NULL) {                                
        fprintf(stderr, "Could not allocate space - %s", strerror(errno));   
        exit(errno);                                                            
    }
    return ptr; /* Functions of type void * call for a pointer to be returned */
}

/* Fopen Function in the form of Steven-styled wrap */
/* in: path/file, mode; return: file ptr */
FILE *Fopen(const char *path, const char *mode) {
    FILE *fp = NULL;
    if((fp = fopen(path, mode)) == NULL){
        fprintf(stderr, "fopen error (%d): %s\n", errno, strerror(errno)); /* Formatted print taken from program directions */
        exit(errno); 
    }
    if (*path) { /* If statement to see if file exists */
        free((char *)path); /* Free casting path to char to avoid warning */
    }
    return fp;
}

/* Fread Function in the form of Steven-styled wrap */
/* in: buffer ptr, buffer size, num buffers, input file ptr; return: bytes read */
size_t Fread(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t num_items = 0;
    if((num_items = fread(ptr, size, nmemb, stream)) != nmemb)  { /* If statement to check to see if the return value differs from the nmemb parameter, therefore causing an error or meaning the function has reached the end of file. */
        if(!feof(stream) && ferror(stream)) { /* If statement that if true means that fread returned an error */
            fprintf(stderr, "fread error (%d): %s\n", errno, strerror(errno)); /* Formatted print taken from program directions */
            exit(errno);
        }
    }
    return num_items; /* Returns the totla number of elements successfully read */
}

/* Fwrite Function in the form of Steven-styled wrap */
/* in: buffer ptr, buffer size, num buffers, output file ptr; return: bytes written */
size_t Fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t num_items;
    if((num_items = fwrite(ptr, size, nmemb, stream)) != nmemb)  { /* If statement to check to see if the return value differs from the nmemb parameter, therefore causing an error */
        fprintf(stderr, "fwrite error (%d): %s\n", errno, strerror(errno)); /* Formatted print taken from program directions */
        exit(errno);
    }
    return num_items; /* Returns the totla number of elements successfully returned as size_t object */
}           

/* Fclose Function in the form of Steven-styled wrap */
/* in: file ptr */
void  Fclose(FILE *stream) {
    if(fclose(stream)) { /* Checks to see if the stream returns EOF */ 
        fprintf(stderr, "fclose error (%d): %s\n", errno, strerror(errno)); /* Formatted print taken from program directions */
        exit(errno);
    }
}                                 

/* ParseArgs Function */
/* in: argc, argv; out: ifile, ofile, kfile */
void   parseArgs(int argc, char *argv[], char **ifile, char **ofile, char **kfile) {   
    if (((strcmp(argv[1], "-h")) == 0) || ((strcmp(argv[1], "--help")) == 0)) {
        help(); /* Help function call */
        exit(0); /* Exit with code 0, meaing no error */
    }
    
    for (int i = 1; i < argc; i+=2) { /* For loop to iterate over the words typed in the command line, int i = 1 so that we can skip over the entering of the file name, i increments by 2 because we are skipping over the arguments being passed. */
        if (((strcmp(argv[i], "-i")) == 0) || ((strcmp(argv[i], "--input")) == 0)) { /* Checking to see if "-i" was typed into the command line. */
            if(!(argv[i + 1] == NULL) && (argv[i+1][0] != '-')) { /* If statment to check to see that there is no missing argument. */
                *ifile = Malloc(strlen(argv[i]) + 1); /* Allocating memory for ifile. */
                strcpy(*ifile, argv[i+1]); /* Copies argv[i] to ifile's memory location */
            } else { /* Else statement that only runs if no argument was passed */
                fprintf(stderr, "command line error (203): no argument after '-i|--input'\n");
                exit(203); 
            }
        } else if (((strcmp(argv[i], "-o")) == 0) || ((strcmp(argv[i], "--output")) == 0)) { /* Checking to see if "-o" was typed into the command line. */
            if(!(argv[i + 1] == NULL) && (argv[i+1][0] != '-')) { /* If statment to check to see that there is no missing argument. */
                *ofile = Malloc(strlen(argv[i]) + 1); /* Allocating memory for ofile. */
                strcpy(*ofile, argv[i+1]); /* Copies argv[i] to ofile's memory location */
            } else { /* Else statement that only runs if no argument was passed */
                fprintf(stderr, "command line error (204): no argument after '-o|--output'\n");
                exit(204); 
            }
        } else if (((strcmp(argv[i], "-k")) == 0) || ((strcmp(argv[i], "--key")) == 0)) { /* Checking to see if "-o" was typed into the command line. */
            if(!(argv[i + 1] == NULL) && (argv[i+1][0] != '-')) { /* If statment to check to see that there is no missing argument. */
                *kfile = Malloc(strlen(argv[i]) + 1); /* Allocating memory for kfile. */
                strcpy(*kfile, argv[i+1]); /* Copies argv[i] to kfile's memory location */
            } else { /* Else statement that only runs if no argument was passed */
                fprintf(stderr, "command line error (205): no argument after '-k|--key'\n");
                exit(205); 
            }
        } else { /* For loop to check if just the file's name was typed into the command line */
            fprintf(stderr, "command line error (201): incorrect/unexpected argument entered\n"); /* Line give in instructions. */
            exit(201); /* Exiting using the corresponding error code */
        }
    }

    if (*ifile == NULL || *ofile == NULL || *kfile == NULL) { /* If statment that checks to see if any of the files are null */
        fprintf(stderr, "command line error (202): one or more required arguments not entered\n"); /* Line give in instructions. */
        exit(202); /* Exiting using the corresponding error code */
    } 
}

/* Help Function defined in function header */
void help() {
    /* Print statment that instructs the user on what to enter */
    fprintf(stdout, "Help: encDec performs an XOR-based encryption/decryption\non the specified input file using the specified key file to\nproduce the specified output file. Acceptable input looks like:\n\nencDec -i|--input <input file> -o|--output <output file> -k|--key <key file>\n");
    exit(0);
}



/* EncryptDecrypt function defined in function header */
/* in: buffer ptr, key ptr, buffer size, input file ptr, output file ptr */
void encryptDecrypt(unsigned char *block, unsigned char *key, size_t BLOCKSIZE, FILE *fip, FILE *fop) {    
    size_t items_read = 0;
    size_t items_written = 0;
    
    /* Key is passed but has already been read */
    
    while ((items_read = Fread(block, sizeof(char), BLOCKSIZE, fip)) > 0) {
        /* for loop to iterate of the string block */
        for (unsigned int i = 0; i < items_read; i++) {
            if (DEBUG) fprintf(stdout, "%c (%#04X) ^ %c (%#04X) = ", block[i], block[i], key[i], key[i]); /* Copied from xorCrypt.c */
            block[i] = block[i] ^ key[i % BLOCKSIZE]; /* Standard XOR encryption algorithm */
            if (DEBUG) fprintf(stdout, "%c (%#04X)\n", block[i], block[i]); /* Copied from xorCrypt.c */
        }
        items_written = Fwrite(block, sizeof(char), items_read, fop); /* Writes block to the output file */
        /* If statment that checks if the number of items read is greater than the number of items written */
        if(items_read > items_written) {
            fprintf(stderr, "encrypt/decrypt error (207): unable to write all items read (%zu vs. %zu)\n", items_written, items_read); /* Formatted print statment from the project descripiton */
            exit(207);
        }
    }
}

/* in: buffer ptr, key length, key file ptr */
void readKey(unsigned char *key, size_t BLOCKSIZE, FILE *fkp){
    /* Initializing block_size variable of type size_t to the BLOCKSIZE constant's value */
    size_t block_size = BLOCKSIZE;
    /* File is opened in the main function of encDec.c */
    size_t items_read = Fread(key, sizeof(char), BLOCKSIZE, fkp); /* Steven style wrapped fread: passing the key char array, size of a character, BLOCKSIZE, fkp) */
    /* If statement to check if error (206) applies: key is fewer bytes than expected */
    if (items_read < block_size) {
        fprintf(stderr, "readKey error (206): key is fewer bytes than expected (%zu vs. %zu)\n", items_read, block_size); /* Formatted print statment from the project descripiton */
        exit(206);
    }
}               