#include "../headers/mode.h"

#define OCTAL_LENGHT 4

static mode_t parse_mode_octal(char mode[]) {
    if(strlen(mode)!=OCTAL_LENGHT){
        fprintf(stderr, "Something"); //Alterar texto
        exit(1);  //Invalid octal mode
    }
    for (size_t i = 1; i < OCTAL_LENGHT; i++)
    {
        if( !isdigit(mode[i]) || mode[i]>='8'){
            fprintf(stderr, "Something"); //Alterar texto
            exit(1);  //Invalid octal mode
        }
    }
    mode_t result=atoi(mode);
    printf("%d", result);
    return result;
}

static mode_t parse_mode_str(char mode[]) {
    return 0u;
}


mode_t parse_mode(char mode[]){

    switch (mode[0])
    {
    case '0':
        parse_mode_octal(mode);
        break;
    case 'a':
    case 'u':
    case 'g':
    case 'o':
        parse_mode_str(mode);
        break;
    
    default:
        break;
    }
}