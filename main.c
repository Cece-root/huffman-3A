#include "decompress.c"

//Main
void main(){
    char choice = '0';
    char fname[200]="";
    FILE *file;
    while(choice != 'b' && choice != 'c' && choice != 'd'){
        printf("Do you wish the compress (c) or decompress (d) a file or to do both (b) ? : ");
        scanf("%c%*c", &choice);
    }
    if (choice == 'c'){
        printf("Enter the path of the file you wish to compress : ");
        scanf("%200s",fname);
        while((file = fopen(fname,"r")) == NULL){ //check whether or not the file exists.
                printf("No file has been found for this path ! \nEnter the path of the file you wish to compress : ");
                scanf("%200s",fname);
        }
        fclose(file);
        comp(fname);
                

    }
    else if (choice == 'd'){
        printf("Enter the path of the file you wish to decompress : ");
        scanf("%200s",fname);
    	while((file = fopen(fname,"r")) == NULL){ //check whether or not the file exists.
                printf("No file has been found for this path ! \nEnter the path of the file you wish to decompress : ");
                scanf("%200s",fname);
        }
        fclose(file);
        decomp(fname);
          
    }
    else{
    printf("Enter the path of the file you wish to compress and then decompress : ");
        scanf("%200s",fname);
    	while((file = fopen(fname,"r")) == NULL){ //check whether or not the file exists.
                printf("No file has been found for this path ! \nEnter the path of the file you wish to decompress : ");
                scanf("%200s",fname);
        }
        fclose(file);
        decomp(comp(fname));
    }
}