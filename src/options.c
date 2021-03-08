#include "../headers/options.h"

/* [Options] parser function */
int parse_options(struct options* opt, char* arg) {
    for (size_t i = 1; i < strlen(arg); ++i) {
        switch (arg[i]) {
            case 'R':
                opt->recursive = true;
                break;
            case 'v':
                opt->verbose = true;
                opt->changes = false;
                break;
            case 'c':
                opt->verbose = false;
                opt->changes = true;
                break;
            
            default:
                /* exit error - invalid option */
                fprintf(stderr, "xmod: invalid option -- '%s'\n", arg);
                exit(1);
        }
    }

    return 0;
}

static void mode_to_str(const mode_t* mode, char* str){
    str[0]= ((*mode) & S_IRUSR) ? 'r' : '-';
    str[1]= ((*mode) & S_IWUSR) ? 'w' : '-';
    str[2]= ((*mode) & S_IXUSR) ? 'x' : '-';
    str[3]= ((*mode) & S_IRGRP) ? 'r' : '-';
    str[4]= ((*mode) & S_IWGRP) ? 'w' : '-';
    str[5]= ((*mode) & S_IXGRP) ? 'x' : '-';
    str[6]= ((*mode) & S_IROTH) ? 'r' : '-';
    str[7]= ((*mode) & S_IWOTH) ? 'w' : '-';
    str[8]= ((*mode) & S_IXOTH) ? 'x' : '-';
    str[9]='\0';
}

void options_output(const struct options* opt, const char* file_path,mode_t* old_mode,mode_t* new_mode){
    (*new_mode)=(*new_mode)&0777;
    (*old_mode)=(*old_mode)&0777;
    if((*old_mode)!=(*new_mode)){
        if(opt->changes || opt->verbose){
            //Print changed
            char str1[10];
            char str2[10];
            mode_to_str(old_mode,str1);
            mode_to_str(new_mode,str2);
            printf("mode of '%s' changed from %04o (%s) to %04o (%s)\n",file_path,(*old_mode),str1,(*new_mode),str2);
        }
    }else{
        if(opt->verbose){
            char str[10];
            mode_to_str(new_mode,str);
            printf("mode of '%s' retained as %04o (%s)\n",file_path,(*old_mode),str);
        }
    }
}