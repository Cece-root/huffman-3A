#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitfile.h"
#include "compress.c"
#include <sys/stat.h>

int counter = 0;
// Rebuild our tree according to the given file and the number of unique characters.
void rebuildtree(tree head, bit_file_t *file, int unique_length){
    int c = BitFileGetBit(file);
    if(c != EOF && counter != unique_length){
        if(c == 0){
            tree newtree = (tree)malloc(sizeof(struct tree));
            head->left = newtree;
            rebuildtree(head->left, file, unique_length); 
            tree newertree = (tree)malloc(sizeof(struct tree));
            head->right = newertree;    
            rebuildtree(head->right, file, unique_length);        
        }
        else if(c == 1){
            head->left = NULL;
            head->right = NULL;
            head->data=BitFileGetChar(file);
            counter++;
        }
    }
}

// Decompresses the given file thanks to the tree created with the header in a new file.
void decompression(tree mytree, bit_file_t *file, FILE *decompressedfile, int total_length){
    int c;
    int count = 0;
    tree temp_tree = mytree;
    while((c = BitFileGetBit(file)) != EOF && count < total_length){
        if(c==0){
            temp_tree=temp_tree->left;
        }
        else if(c==1){
            temp_tree=temp_tree->right;
        }

        if(leafcheck(temp_tree)==1){
            fprintf(decompressedfile, "%c", temp_tree->data);
            count++;
            temp_tree = mytree;
        }    
    }
}


// Function that runs all the needed functions to decompress a given file.
void decomp(char *fileName){

    int length = 0;
    int uniqchar = 0;
    char *fileNameDecompress;
    bit_file_t *file;
    FILE *decompressedfile;
    tree mytree = (tree)malloc(sizeof(struct tree));

    file = BitFileOpen(fileName, BF_READ);  

    // We write a for loop that will delete the . and what comes after.
    for(int i = 0; i < strlen(fileName); i++){
        if(fileName[i] == '.'){
            fileName[i] = '\0';
        }
    }

    // We are going to create the name of the decompressed file according to the original name of the file, while making sure that we have no memory leak.
    fileNameDecompress = (char*)malloc( (strlen(fileName) + strlen(".dcp"))*sizeof(char) );
    strcpy(fileNameDecompress, fileName);
    strcat(fileNameDecompress, ".dcp");

    decompressedfile = fopen(fileNameDecompress, "w");

    BitFileGetBitsNum(file, &length, 4*8, sizeof(length));
    BitFileGetBitsNum(file, &uniqchar, 4*8, sizeof(uniqchar));

    rebuildtree(mytree, file, uniqchar);
    decompression(mytree, file, decompressedfile, length);

    BitFileClose(file);
    fclose(decompressedfile);
    
    struct stat st1;
    //change to have the complete name with extension
    char fName[200]="";
    strcpy(fName, fileName);
    char ptTxt[200]=".cmp";
    strcat(fName,ptTxt);
    //get size
    stat(fName, &st1);
    int size1 = st1.st_size;
    struct stat st2;
    //get size
    stat(fileNameDecompress, &st2);
    int size2 = st2.st_size;
    //print size
    if (size1 != -1)
        printf("Size of the original file is %d bytes \n", size1);
    if (size2 != -1)
        printf("Size of the compressed file is %d bytes \n", size2);

    
    printf("Your file has been decompressed under the filename : %s\n", fileNameDecompress);
}