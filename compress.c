#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct list {
    int freq;
    struct list *next;
    struct tree * tree;
}* list;

typedef struct tree{
    char data;
    struct tree *left, *right;
} * tree;

list ConstructList(int frequence, list next, tree tree){
    list new = (list)malloc(sizeof(struct list));
    new->freq = frequence;
    new->next = next;
    new->tree = tree;
    return new;
}

tree ConstructTree(char data, tree left, tree right){
    tree new = (tree)malloc(sizeof(struct tree));
    new->data = data;
    new->left = left;
    new->right = right;
    return new;
}

// void tree_print(tree root, int level){
//     for (int i = 0; i < level; i++)
//         printf(i == level - 1 ? "|-" : "  ");
//     if(root->data) printf("%c\n", root->data);
//     else printf("\n");
//     if(root->left) tree_print(root->left, level + 1);
//     if(root->right) tree_print(root->right, level + 1);
// }

// void print(struct list *head) {
//     for (; head; head = head->next)
//         printf(" - frequence of %c is %d\n", head->tree->data, head->freq);
// }

void SortedInsert(struct list** headRef, struct list* newNode) {
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

void InsertSort(struct list** headRef) {
    list result = NULL, current = *headRef, next;
    while (current!=NULL) {
        next = current->next;
        SortedInsert(&result, current);
        current = next;
    }
    *headRef = result;
}

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

int leafcheck(tree B){
    return !(B->left) && !(B->right);
}

tree CodingTree(struct list **head){
    list * current = head ;
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

void writebinary(tree B, FILE *header){
    if(leafcheck(B)){
        fprintf(header, "1%c", B->data);
    }
    else
        fprintf(header, "0");

    if(B->left){
        writebinary(B->left, header);
    }

    if(B->right){
        writebinary(B->right, header);
    }
}

//CETTE FONCTION VA NOUS PERMETTRE DE STOCKER DANS UN TABLEAU LES CARACTERES ET LEUR CODES ASSOCIES ACCORDING TO THE TREE
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

void occurency(char *fileNAME)
{
    FILE *file;
    FILE *compressedfile;
    file = fopen(fileNAME, "r");
    compressedfile = fopen("Output/compressedfile.txt", "w");

    list h = NULL;
    list *a = &h;
    tree T = NULL;
    char c;
    char d;
    int nbUnqChar=0;
    int nbTotChar=0;


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
    fprintf(compressedfile, "%d ", nbTotChar);
    fprintf(compressedfile, "%d ", nbUnqChar);
    list *a2 = a;
    
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
            fprintf(compressedfile, "%d", codes[c][i]);
        }

        
    }
    fclose(compressedfile);
    fclose(file);
    
} 

