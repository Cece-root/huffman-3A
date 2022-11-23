int main(int argc, char const *argv[]){
    FILE *fic = NULL;
    char choice = '0';
    char fname[200]="";
    while(choice != 'c' && choice != 'd'){
        printf("Do you wish the compress (c) or decompress (d) a file ?");
        scanf("%c", &choice);
    }
    if (choice == 'c'){
        while(fic == NULL){ //check whether or not the file exists.
                printf("Enter the path of the file you wish to compress");
                scanf("%200s",fname);
                fic=fopen(fname,"r");
        }
    }
    fclose(fic);
    return 0;
}

