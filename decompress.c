#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compress.c"

// Rebuild our tree according to the given file and the number of unique characters.
void rebuildtree(tree head, FILE *file, int *count, int unique_length){
    char c = getc(file);
    if(c != EOF && *count < unique_length){
        if(c == '0'){
            tree newtree = (tree)malloc(sizeof(struct tree));
            head->left = newtree;
            tree newertree = (tree)malloc(sizeof(struct tree));
            head->right = newertree;    
            rebuildtree(head->left, file, count, unique_length); 
            rebuildtree(head->right, file, count, unique_length);        
        }
        else if(c == '1'){
            head->data=getc(file);
            *count++; 
        }
    }
}

// Returns us if we are in a leaf or not
int checkleaf(tree B){
    return (B->left) == NULL && (B->right) == NULL;
}

// Decompresses the given file thanks to the tree created with the header in a new file.
void decompression(tree mytree, FILE *file, FILE *decompressedfile, int total_length){
    char c;
    int count = 0;
    tree temp_tree = mytree;
    while((c = getc(file)) != EOF && count < total_length){
        if(c=='0'){
            temp_tree=temp_tree->left;
        }
        else if(c=='1'){
            temp_tree=temp_tree->right;
        }

        if(checkleaf(temp_tree)==1){
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
    int count = 0;
    char *fileNameDecompress;
    FILE *file;
    FILE *decompressedfile;
    tree mytree = (tree)malloc(sizeof(struct tree));

    file = fopen(fileName, "r");  
     
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


    fscanf(file, "%d", &length);
    fscanf(file, "%d", &uniqchar);
    fscanf(file, " ");// to skip the space between the nb of unique chars and the compressed tree.

    rebuildtree(mytree, file, &count, uniqchar);
    decompression(mytree, file, decompressedfile, length);

    fclose(file);
    fclose(decompressedfile);
}