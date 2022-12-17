#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitfile.h"
#include <sys/stat.h>

//We define our list structure.
typedef struct list {
    int freq;
    struct list *next;
    struct tree *tree;
} *list;

//We define our tree structure.
typedef struct tree{
    char data;
    struct tree *left, *right;
} *tree;

//Creates the list according to the tree and the frequence of the characters.
list ConstructList(int frequence, list next, tree tree){
    list new = (list)malloc(sizeof(struct list));
    new->freq = frequence;
    new->next = next;
    new->tree = tree;
    return new;
}

//Creates the tree
tree ConstructTree(char data, tree left, tree right){
    tree new = (tree)malloc(sizeof(struct tree));
    new->data = data;
    new->left = left;
    new->right = right;
    return new;
}

// void print(struct list *head) {
//     for (; head; head = head->next)
//         printf(" - frequence of %c is %d\n", head->tree->data, head->freq);
// }

//Allows us to sort our tree
void SortedInsert(struct list **headRef, struct list *newNode) {
    list current = *headRef;
    // Special case for the head end
    if (*headRef == NULL || (*headRef)->freq >= newNode->freq) {
        newNode->next = *headRef;
        *headRef = newNode;
    }
    else {
        // Locate the node before the point of insertion
        while (current->next!=NULL && current->next->freq<newNode->freq) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }
}

//Using SortedInsert to sort what we get in input
void InsertSort(struct list **headRef) {
    list result = NULL, current = *headRef, next;
    while (current!=NULL) {
        next = current->next;
        SortedInsert(&result, current);
        current = next;
    }
    *headRef = result;
}

//Allows us to get the frequency of a letter in order to get the tree later on.
void add_freq(struct list *head, char letter, int newfreq) {
    for(; head; head = head->next)
        if (head->tree->data == letter ) head->freq += newfreq;
}

//Returns 1 if it's already in the list, otherwise returns 0.
int is_in(char letter, struct list *list) {
    for (; list; list = list->next)
        if (list->tree->data == letter && list->tree->data != 0) return 1;
    return 0;
}

//Returns us if we are in a leaf or not
int leafcheck(tree B){
    return (B->left) == NULL && (B->right) == NULL;
}

//Returns the tree once it is fully created
tree CodingTree(struct list **head){
    list *current = head ;
    tree treeresult;
    while( (*current)->next) {
        list node_1 = *head;
        if(((*head))->next == NULL){
        	return treeresult;
        	}
        list node_2 = (*head)->next;
        //printf("---------------------\n");

        int sum = (node_1->freq) + (node_2->freq);

        treeresult = ConstructTree(0, node_1->tree, node_2->tree);
        
        /* Push in the head */ 
        *head = ConstructList(sum, (*head)->next->next, treeresult);

        //printf("Total FREQ : %d", list_tree->freq);
        InsertSort(head);
        //printf("\n");
        //print(*head);
        current = &((*current)->next);
    }
}

//Write the binary code in the header of our compressed file using the tree.
void writebinary(tree B, bit_file_t *header){
    if(leafcheck(B)){
        BitFilePutBit(1, header);
        BitFilePutChar(B->data, header);
    }
    else{
        BitFilePutBit(0, header);
        if(B->left){
            writebinary(B->left, header);
        }

        if(B->right){
            writebinary(B->right, header);
        }
    }
}

//This function will allow us to stock in a 2D table characters and their associated codes.
void getcodes(tree mytree, int codes[256][256], int buffer[256], int bincode){
    int inside = 0;
    if(mytree->left){
        buffer[bincode]=0;
        getcodes(mytree->left, codes, buffer, bincode+1);
        inside = 1;
    }
    if(mytree->right){
        buffer[bincode]=1;
        getcodes(mytree->right, codes, buffer, bincode+1);
        inside = 1;
    }
    if(inside == 0){
        for(int i = 0; i<bincode; i++){
            codes[mytree->data][i]=buffer[i];
        }
        codes[mytree->data][bincode]=2;
    }
}

// Function that allows us to run all the functions needed to compress our given file.
char *comp(char *fileName)
{

    char *fileNameCompress;
    char c;
    char d;
    int nbUnqChar=0;
    int nbTotChar=0;

    list h = NULL;
    list *a = &h;
    tree T = NULL;
    FILE *file;
    bit_file_t *compressedfile;

    file = fopen(fileName, "r");

    // We write a for loop that will delete the . and what comes after.
    for(int i = 0; i < strlen(fileName); i++){
        if(fileName[i] == '.'){
            fileName[i] = '\0';
        }
    }

    // We are going to create the name of the compressed file according to the original name of the file, while making sure that we have no memory leak.
    fileNameCompress = (char*)malloc( (strlen(fileName) + strlen(".cmp"))*sizeof(char) );
    strcpy(fileNameCompress, fileName);
    strcat(fileNameCompress, ".cmp");

    compressedfile = BitFileOpen(fileNameCompress, BF_WRITE);

    while ((c = getc(file)) != EOF) {
        nbTotChar++;
        if (is_in(c, h) == 1)
        { // If in the dictionnary
            //printf("%c is in the dico\n", c);
            add_freq(h, c, 1);
        }
        if (is_in(c, h) == 0)
        { // If not in the dictionnary
            //printf("%c is Not in the dico\n", c);
            nbUnqChar++;
            T = ConstructTree(c, NULL, NULL);
            *a = ConstructList(1, NULL, T);
            a = &((*a)->next);
        }
        
    }
    BitFilePutBitsNum(compressedfile, &nbTotChar, 4*8, sizeof(nbTotChar));
    BitFilePutBitsNum(compressedfile, &nbUnqChar, 4*8, sizeof(nbUnqChar));

    //list *a2 = a;
    
    InsertSort(&h);
    //printf("\n####TABLE OF OCCURENCES####\n");
    //print(h);
    //printf("###########################\n\n");
    tree mytree =  CodingTree(&h);
    tree mytree2 = mytree;
    //tree_print(mytree, 0);
    writebinary(mytree, compressedfile);
    fseek(file, 0, SEEK_SET); 
    int codes[256][256] = {2};
    int buffer[256];
    getcodes(mytree2, codes, buffer, 0);
    // for(int i = 0; i<256; i++){
    //     if (is_in(i, *a2) == 1){
    //         printf("%c: ",i);
    //         for(int j = 0; j<256; j++){
    //             if(codes[i][j]==2)
    //                 break;
    //             printf("%d",codes[i][j]);
    //         }
    //         printf("\n");
    //     }
    // }
    while ((c = getc(file)) != EOF) {
        for(int i = 0; i<256; i++){
            if(codes[c][i]==2)
                break;
            BitFilePutBit(codes[c][i], compressedfile);     
        }

        
    }
    BitFileByteAlign(compressedfile);
    FILE *bfile = BitFileToFILE(compressedfile);

    //go to the end of the file
    fseek(file, 0L, SEEK_END);
    fseek(bfile, 0L, SEEK_END);
    //get size
    int size1 = ftell(file);
    int size2 = ftell(bfile);
    //print size
    if (size1 != -1)
        printf("Size of the original file is %d bytes \n", size1);
    if (size2 != -1)
        printf("Size of the compressed file is %d bytes \n", size2);

    fclose(bfile);
    fclose(file);
    printf("Your file has been compressed under the filename : %s\n", fileNameCompress);
    return fileNameCompress;
} 