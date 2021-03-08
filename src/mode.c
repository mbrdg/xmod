#include "../headers/mode.h"

#define OCTAL_LENGHT 4
#define MAX_STR_LENGTH 5
#define MIN_STR_LENGTH 3
#define R_VALUE 4
#define W_VALUE 2
#define X_VALUE 1


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
    return (mode_t)strtol(mode,0,8);
}

static unsigned char permitions_set(char mode[]){
    unsigned char mode_mask=0;
    for (size_t i = 2; i < strlen(mode); i++)
    {
        switch (mode[i])
        {
        case 'r':
            mode_mask|=1<<2;
            break;
        case 'w':
            mode_mask|=1<<1;
            break;
        case 'x':
            mode_mask|=1<<0;
            break;
        default:
            fprintf(stderr, "Something"); //Alterar texto
            exit(1);  //Invalid permition
        }
    }
    return mode_mask;
}

static mode_t parse_mode_str(char mode[], char file_path[]) {
    if(strlen(mode)>MAX_STR_LENGTH || strlen(mode)<MIN_STR_LENGTH){
        fprintf(stderr, "Something"); //Alterar texto
        exit(1);  //Invalid str mode size
    }
    if(mode[1]!='-' && mode[1]!='=' && mode[1]!='+'){
        fprintf(stderr, "Something"); //Alterar texto
        exit(1);  //Invalid str mode signal
    }
    
    unsigned short mode_mask = permitions_set(mode);

    struct stat file_mode;
    stat(file_path,&file_mode);
    
    switch (mode[0])
    {
    case 'a':
        mode_mask=(mode_mask<<6) | (mode_mask<<3) | mode_mask;
        break;
    case 'u':
        mode_mask=(mode_mask<<6);
        break;
    case 'g':
        mode_mask=(mode_mask<<3);
        break;
    }
    
    if(mode[1]=='+'){
        mode_mask|=file_mode.st_mode;
    }else if(mode[1]=='-'){
        mode_mask=(~mode_mask) & file_mode.st_mode;
    }else if(mode[1]=='='){
        mode_t temp;
        for (int i = 2; i >= 0; i--)
        {
            temp=7;
            if((mode_mask&(7<<i*3))==0){
                temp=temp<<i*3;
                temp&=file_mode.st_mode;
                mode_mask|=temp;
            }
        }
    }
    return (mode_t)mode_mask;
}


mode_t parse_mode(char mode[],char file_path[]){

    switch (mode[0])
    {
    case '0':
        return parse_mode_octal(mode);
    case 'a':
    case 'u':
    case 'g':
    case 'o':
        return parse_mode_str(mode, file_path);
    
    default:
        fprintf(stderr, "Something"); //Alterar texto
        exit(1);  //Invalid mode
    }
}

