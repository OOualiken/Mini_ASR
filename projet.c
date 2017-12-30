
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>




void sing(char *text , char *filname){
    
    FILE *fichier ;
    
    fichier= fopen(filname, "a+");
    if (fichier != NULL) {
        printf("%s",text);
        fprintf(fichier,"%s", text);
        fclose(fichier);
        
    }
}

int check_if_it_is_my_turn(){
    
    FILE *pf = fopen("toto.txt", "r");
    
    int pid ;
    char *pid_writen;
    int pid2;
    
    pid_writen = malloc(sizeof(char)*10);
    
    if (pf !=NULL ) {
        pid= getpid();
        fgets(pid_writen,10,pf);
        pid2=atoi(pid_writen);
        
        if (pid == pid2) {
            return 0;
            
        }
        
        fclose(pf);
    }
    return 1;
}
void next_child_must_sing(int pid ){
    
    FILE *pf=fopen("toto.txt", "w");
    
    if (pf != NULL) {
        fprintf(pf, "%d", pid);
        fclose(pf);
    }
    
}


//
int read_X_line(FILE *file ,char **buffer , int x){
    
    int i;
    if(file == NULL || buffer == NULL || x <= 0)
        return -1;
    
    for (i=0; i<x; i++) {
        if (fgets(buffer[i], 500, file)==NULL) {
            return i;
        }
    }
    
    return i; }

int create_new_child(char *text,char **buffer,int line,char *fileName){
    
    int pid , i ;
    if (line <=0) {
        return -1;
    }
    pid = fork();
    if (pid==0) {
        for (i=0; i<line; i++) {
            strcat(text,buffer[i]);
            
        }
        
        while (1) {
            if (check_if_it_is_my_turn() == 0) {
                sing(text,fileName);
                _exit(1);
            }else{
                sleep(1);
            }
        }
    }
    
    return pid;
}

int* init_array( int taille){
    int *array;
    array=malloc(sizeof(int) *taille);
    return array;
    
}
// rajouter un élément à un tableau

int *add_elemnt(int element , int *array, int taille){
    int *new_tab;
    int i=0;
    new_tab= init_array(taille+1);
    for (i=0; i<taille; i++) {
        new_tab[i] =array[i];
        
    }
    new_tab[taille]=element;
    
    free(array);
    
    
    
    return new_tab;
    
}
int* read_file_and_create_child(char *nom_du_fichier , char *nom_du_fichier_de_sortie,int x , int *nombre_de_processus){
    int nb=0;
    int *array;
    char **buffer;
    char *text;
    int tmp, tmp2 ;
    buffer=malloc(sizeof(char*)*x);
    for (nb=0; nb<x; nb++) {
        buffer[nb]=malloc(sizeof(char)*500);
    }
    
    text=malloc(sizeof(char)*(500*x));
    array = malloc(sizeof(int));
    
    FILE *pf=fopen(nom_du_fichier, "r");
    if (pf!=NULL) {
        *nombre_de_processus=0;
        //le pere lit le fichier
        while ((tmp2 =read_X_line(pf, buffer, x))!=0) {//si il a reussi a recuperer des lignes du fichier
            //il cree un fils
            tmp = create_new_child(text, buffer, tmp2, nom_du_fichier_de_sortie);//la fonction retourne le pid du fils cree
            //printf("A\n");
            array = add_elemnt(tmp, array, *nombre_de_processus);//le pid est ajoute au tableau de pid
            //printf("B\n");
            (*nombre_de_processus)++;
            //printf("C\n");
        }
    }
    
    
    return array;
    
}



int main(int argc, const char * argv[]) {
    int nombre_de_ligne=0;
    char nom_du_fichier[30];
    char nom_du_fichier_desortie[30];
    int *array_pid;
    int taille;
    int cpt,status;
    
    
    
    nombre_de_ligne = atoi(argv[1]);
    strcpy(nom_du_fichier,argv[2]);
    strcpy(nom_du_fichier_desortie,argv[3]);
    //printf("A\n");
    
    array_pid= read_file_and_create_child(nom_du_fichier, nom_du_fichier_desortie, nombre_de_ligne, &taille);
    //printf("B\n");
    for (cpt=0; cpt<taille; cpt++) {
        //printf("pid: %d\n",array_pid[cpt]);
        next_child_must_sing(array_pid[cpt]);
        waitpid(array_pid[cpt],&status,WUNTRACED);
    }
    free(array_pid);
    
    
    return 0;
}




