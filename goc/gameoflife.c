#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "gol.h"

int main(int argc, char *argv[]){
    char inputfile[100], outputfile[100];
    int gen = -1;
    bool inputf = false, outputf = false, genf = false, sf = false, tf = false;
    for(int i = 0; i < argc; i++) {
        if(argv[i][0] == '-'){
            if (strlen(argv[i]) > 2){
                fprintf(stderr, "Error 6: Input argument \'%s\' is too long!\n", argv[i]);
                exit(6);
            }
            switch (argv[i][1]) {
                case 'i':
                    inputf = true;
                    strcpy(inputfile, argv[i + 1]);
                    break;
                case 'o':
                    outputf = true;
                    strcpy(outputfile, argv[i + 1]);
                    break;
                case 'g':
                    genf = true;
                    int tmp = atoi(argv[i + 1]);
                    if(gen == -1){
                        gen = tmp;
                    } else if(gen != tmp){
                        fprintf(stderr, "Error 2: Different values of g are given: %d and %d!\n", gen, tmp);
                        exit(2);
                    }
                    break;
                case 's':
                    sf = true;
                    break;
                case 't':
                    tf = true;
                    break;
                default:
                    fprintf(stderr, "Error 1: Unknown input argument \'%s\'!\n", argv[i]);
                    exit(1);
                    break;
            }
        }
    }
    struct universe v;
    if(inputf){
        FILE *fp;
        fp = fopen(inputfile, "r");
        if(fp == NULL){
            fprintf(stderr, "Error 3: Can't open input file!\n");
            exit(3);
        }
        read_in_file(fp,&v);
    } else {
        printf("Please input the initial generation of the universe (To end the input, press Ctrl+D in a new line):\n");
        read_in_file(stdin, &v);
    }
    if(genf){
        for(int i = 1; i <= gen; i++){
            if(tf){
                evolve(&v, will_be_alive_torus);
            } else {
                evolve(&v, will_be_alive);
            }
        }
    } else{
        for(int i = 1; i <= 5; i++){
            if(tf){
                evolve(&v, will_be_alive_torus);
            } else {
                evolve(&v, will_be_alive);
            }
        }
    }

    if(outputf){
        FILE *fp2;
        fp2=fopen(outputfile, "w");
        printf("Output written to file '%s'\n", outputfile);
        write_out_file(fp2, &v);
    } else {
        printf("Output:\n");
        write_out_file(stdout, &v);
    }
    if(sf){
        print_statistics(&v);
    }
    return 0;
}
