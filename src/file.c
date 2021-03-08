#include "../headers/file.h"

char* parse_file(const char arg[]){
    char* file_path;
    if(access(arg,F_OK)!=-1){
        file_path=malloc(sizeof(char)*(strlen(arg)+1));
        strcpy(file_path,arg);
    }else{
        fprintf(stderr, "Something"); //Alterar texto
        exit(1);  //Invalid path
    }
    return file_path;
}