#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//bonjour
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

/*void print2D(struct tree* root, int space){
	if(root == NULL);
		return;
	space += 5;
	
	print2D(root->right, space);
	printf("\n");
	for(int i = 5; i < space; i++)
		printf(" ");
	printf("%d\n", root->data);	
	print2D(root->left, space);
}*/
/*void printtree(tree B){
    if (B!=NULL)
    {
        if (B->left != NULL)
        {
            printf("0 ");
        }
        else if(B->right != NULL) {
		printf("1 ");
	}
	printtree(B->right);
        printtree(B->left);
        if (B->left == NULL && B->right == NULL)
        {
            printf("%c\n ", B->data);
        }
    }else{}
}*/

void print(struct list *head) {
    for (; head; head = head->next)
        printf(" - frequence of %c is %d\n", head->tree->data, head->freq);
}

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

int is_in(char letter, struct list *list) {
    for (; list; list = list->next)
        if (list->tree->data == letter) return 1;
    return 0; // is not in
}
int leafcheck(tree B){ //modify
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
        printf("---------------------\n");

        int sum = (node_1->freq) + (node_2->freq);

        treeresult = ConstructTree(' ', node_1->tree, node_2->tree);
        
        /* Push in the head */ 
        *head = ConstructList(sum, (*head)->next->next, treeresult);

        //printf("Total FREQ : %d", list_tree->freq);
        InsertSort(head);
        printf("\n");
        print(*head);
        current = &((*current)->next);
    }
}

void getbinary(tree B, int codes[], int somme, FILE *bincodes){//modify
    if(B->left){
        codes[somme] = 0;
        getbinary(B->left, codes, somme+1, bincodes);
    }

    if(B->right){
        codes[somme] = 1;
        getbinary(B->right, codes, somme+1, bincodes);
    }

    if(leafcheck(B)){
        fprintf(bincodes, "%c:", B->data);
        for (int i = 0; i<somme; i++){
            fprintf(bincodes, "%d", codes[i]);
        }
        fprintf(bincodes, "\n");
    }

}

void occurency(char *fileNAME)
{
    FILE *file;
    FILE *cfile;
    FILE *bincodes;
    file = fopen(fileNAME, "r");
    cfile = fopen("Output/compressed.txt", "w");
    bincodes = fopen("Output/bincodes.txt", "w");

    list h = NULL;
    list *a = &h;
    tree T = NULL;
    char c;
    char d;


    while ((c = getc(file)) != EOF) {

        if (is_in(c, h) == 1)
        { // If in the dictionnary
            printf("%c is in the dico\n", c);
            add_freq(h, c, 1);
        }
        if (is_in(c, h) == 0)
        { // If not in the dictionnary
            printf("%c is Not in the dico\n", c);
            T = ConstructTree(c, NULL, NULL);
            *a = ConstructList(1, NULL, T);
            a = &((*a)->next);
        }
        
    }
    
    
    InsertSort(&h);
    printf("\n####TABLE OF OCCURENCES####\n");
    print(h);
    printf("###########################\n\n");
     

    int codes[100];
    getbinary(CodingTree(&h), codes, 0, bincodes);
    fclose(file);
    fclose(bincodes);
    file = fopen(fileNAME, "r");
    while ((c = getc(file)) != EOF) {
        bincodes = fopen("Output/bincodes.txt", "r");
        while ((d = getc(bincodes)) != EOF) {
            if(c == d){
                d= getc(bincodes);
                d= getc(bincodes);
                while (d == '0' || d == '1') {
                    fprintf(cfile, "%c", d);//GETBITFILE
                    d= getc(bincodes);
                }
            }
        
        }
        fclose(bincodes);
        
    }
    fclose(file);
    fclose(cfile);
} 

