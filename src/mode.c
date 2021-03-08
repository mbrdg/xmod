#include "../headers/mode.h"

#define OCTAL_LENGHT 4
#define MAX_STR_LENGTH 5
#define MIN_STR_LENGTH 3
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
    if(mode[1]!='-' && mode[1]!='=' && mode[1]!='+'){
        fprintf(stderr, "Something"); //Alterar texto
        exit(1);  //Invalid str mode signal
    }
    mode_result_t result = { .mode=mode_mask, .signal=mode[1] };
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
    
    if(stat(file_path,&file_mode)<0){ return 1;}

    if(mode->signal=='+'){
        mode->mode|=file_mode.st_mode;
    }else if(mode->signal=='='){
        mode_t test=mode->mode|=0770;
        test|=file_mode.st_mode;
        mode->mode|=file_mode.st_mode;
    }

    printf("%o\n",file_mode.st_mode);
    printf("%o\n",mode->mode);
    //if(mode->mode>)

    if(mode->mode>63) mode->mode|=0077;
    //mode_t test = file_mode.st_mode&mode->mode;  //Igualar com and
    printf("%o",file_mode.st_mode);
    //printf("%o",test);

    return 0;
}
