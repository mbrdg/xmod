#include "../headers/mode.h"

#define OCTAL_LENGHT 4
#define MAX_STR_LENGTH 5
#define MIN_STR_LENGTH 3
#define USER 100
#define GROUP 10
#define OTHER 1
#define ALL 111
#define R_VALUE 4
#define W_VALUE 2
#define X_VALUE 1


static mode_result_t parse_mode_octal(char mode[]) {
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
    mode_result_t result= { .mode=atoi(mode), .signal='='};
    return result;
}

static mode_result_t parse_mode_str(char mode[]) {
    if(strlen(mode)>MAX_STR_LENGTH || strlen(mode)<MIN_STR_LENGTH){
        fprintf(stderr, "Something"); //Alterar texto
        exit(1);  //Invalid str mode size
    }
    int multiplier=1;
    switch (mode[0])
    {
    case 'a':
        multiplier=ALL;
        break;
    case 'u':
        multiplier=USER;
        break;
    case 'g':
        multiplier=GROUP;
        break;
    case 'o':
        multiplier=OTHER;
        break;
    }
    permitions_t permitions = {.r=false, .w=false, .x=false };
    mode_t mode_mask=0u;
    for (size_t i = 2; i < strlen(mode); i++)
    {
        switch (mode[i])
        {
        case 'r':
            if(!permitions.r){
                permitions.r=true;
                mode_mask+=R_VALUE;
            }
            break;
        case 'w':
            if(!permitions.w){
                permitions.w=true;
                mode_mask+=W_VALUE;
            }
            break;
            case 'x':
            if(!permitions.x){
                permitions.x=true;
                mode_mask+=X_VALUE;
            }
            break;
        default:
            fprintf(stderr, "Something"); //Alterar texto
            exit(1);  //Invalid permition
        }
    }
    if(mode[1]!='-' && mode[1]!='=' && mode[1]!='+'){
        fprintf(stderr, "Something"); //Alterar texto
        exit(1);  //Invalid str mode signal
    }
    mode_result_t result = { .mode=mode_mask*multiplier, .signal=mode[1]};
    return result;
}


mode_result_t parse_mode(char mode[]){

    switch (mode[0])
    {
    case '0':
        return parse_mode_octal(mode);
    case 'a':
    case 'u':
    case 'g':
    case 'o':
        return parse_mode_str(mode);
    
    default:
        fprintf(stderr, "Something"); //Alterar texto
        exit(1);  //Invalid mode
    }
}

int update_mode(char file_path[],mode_result_t *mode){
    struct stat file_mode;
    if(stat(file_path,&file_mode)<0) return 1;

    return 0;
}
