#include <stdio.h>
#include "gol.h"
#include <stdlib.h>

void read_in_file(FILE *infile, struct universe *u){
    char ch;
    int col = 0, row = 0, consistency_check = 0;
    u->data = (char**)malloc(sizeof(char*) * 513);
    for(int i = 0; i < 513; i++){
        u->data[i] = (char*)malloc(sizeof(char) * 513);
    }
    while(fscanf(infile, "%c", &ch) != EOF){
        if(ch == '\n' || ch == '*' || ch == '.' || ch == ' '){
            if(ch == '\n'){
                if (consistency_check == 0){
                    consistency_check = row;
                } else if (row != consistency_check){
                    fprintf(stderr, "Error 5: Inconsistent row lengths in the input!\n");
                    exit(5);
                }
                row = 0;
                col++;
                continue;
            }
            u->data[col][row] = ch;
        } else {
            if (ch != '\r'){
                fprintf(stderr, "Error 4: Wrong character at position %d in line %d!\n", row, col);
                exit(4);
            }
        }
        row++;
    }

    u->col=col;
    u->row=consistency_check;
    //compute other parameter
    u->evolveCount=0;
    u->totelCellCount=col*col;
    int liveCellNow=0;
    for(int i=0;i<col;i++){
        for(int j=0;j<col;j++){
            if(u->data[i][j]=='*'){
                liveCellNow++;
            }
        }
    }
    u->liveCellNow=liveCellNow;
    u->liveCellSum=liveCellNow;
//    printf("col%d row%d lln%d\n",col,row,liveCellNow);
}

void write_out_file(FILE *outfile, struct universe *u){
    for(int i=0;i<u->col;i++){
        fprintf(outfile,"%s\n",u->data[i]);
    }
}

int is_alive(struct universe *u, int column, int row){
    if(u->data[column][row]=='*'){
        return 1;
    }else{
        return 0;
    }
}


int will_be_alive_torus(struct universe *u,  int column, int row){
    int coundalive=0;
    int temp_col = 0;
    int temp_row = 0;
    //if the cell is alive
    if(u->data[column][row]=='*'){
        for(int i=column-1;i<=column+1;i++){
            for(int j=row-1;j<=row+1;j++){
                if(i==column&&j==row)
                    continue;
                else{
                    temp_col = i%(u->col);
                    temp_row = j%(u->row);
                    if (temp_col < 0) temp_col += u->col;
                    if (temp_row < 0) temp_row += u->row;
                    if(is_alive(u, temp_col, temp_row)){
                        coundalive++;
                    }
                }
            }
        }
        if(coundalive==2||coundalive==3){
            return 1;
        }
    }
    //if the cell is dead
    coundalive=0;
    if(u->data[column][row]=='.'){
        for(int i=column-1;i<=column+1;i++){
            for(int j=row-1;j<=row+1;j++){
                if(i==column&&j==row)
                    continue;
                else{
                    temp_col = i%(u->col);
                    temp_row = j%(u->row);
                    if (temp_col < 0) temp_col += u->col;
                    if (temp_row < 0) temp_row += u->row;
                    if(is_alive(u, temp_col, temp_row)){
                        coundalive++;
                    }
                }
            }
        }
        if(coundalive==3){
            return 1;
        }
    }
    return 0;
}

int will_be_alive_lala(struct universe *u, int column, int row){
    int coundalive=0;
    //if the cell is alive
    if(u->data[column][row]=='*'){
        for(int i=column-1;i<=column+1;i++){
            for(int j=row-1;j<=row+1;j++){
                if(i<0||j<0||i>u->col||j>u->row||(i==column&&j==row))
                    continue;
                else{
                    if(is_alive(u,i,j)){
                        coundalive++;
                    }
                }
            }
        }
        if(coundalive==2||coundalive==3){
            return 1;
        }
    }
    //if the cell is dead
    coundalive=0;
    if(u->data[column][row]=='.'){
        for(int i=column-1;i<=column+1;i++){
            for(int j=row-1;j<=row+1;j++){
                if(i<0||j<0||i>u->col||j>u->row||(i==column&&j==row))
                    continue;
                else{
                    if(is_alive(u,i,j)){
                        coundalive++;
                    }
                }
            }
        }
        if(coundalive==3){
            return 1;
        }
    }
    return 0;
}

int will_be_alive(struct universe *u, int column, int row){
    int coundalive=0;
    //if the cell is alive
    if(u->data[column][row]=='*'){
        for(int i=column-1;i<=column+1;i++){
            for(int j=row-1;j<=row+1;j++){
                if(i<0||j<0||i>u->col||j>u->row||(i==column&&j==row))
                    continue;
                else{
                    if(is_alive(u,i,j)){
                        coundalive++;
                    }
                }
            }
        }
        if(coundalive==2||coundalive==3){
            return 1;
        }
    }
    //if the cell is dead
    coundalive=0;
    if(u->data[column][row]=='.'){
        for(int i=column-1;i<=column+1;i++){
            for(int j=row-1;j<=row+1;j++){
                if(i<0||j<0||i>u->col||j>u->row||(i==column&&j==row))
                    continue;
                else{
                    if(is_alive(u,i,j)){
                        coundalive++;
                    }
                }
            }
        }
        if(coundalive==3){
            return 1;
        }
    }
    return 0;
}

void evolve(struct universe *u, int (*rule)(struct universe *u, int column, int row)){
    char** temp;
    int liveCellNow=0;
    temp=(char**)malloc(sizeof(char*)*u->col);
    for(int i=0;i<u->col;i++){
        temp[i]=(char*)malloc(sizeof(char)*u->row);
    }

    //copy data to temp
    for(int i=0;i<u->col;i++){
        for(int j=0;j<u->row;j++){
            temp[i][j] = u->data[i][j];
        }
    }
    //run function,write new data to u
    for(int i=0;i<u->col;i++){
        for(int j=0;j<u->row;j++){
            int tmp=rule(u,i,j);
            if(tmp==1){
                temp[i][j]='*';
                liveCellNow++;
            }
            else if(tmp==0){
                temp[i][j]='.';
            }
        }
    }
    for(int i=0;i<u->col;i++){
        for(int j=0;j<u->row;j++){
            u->data[i][j]=temp[i][j];
        }
    }
    free(temp);
    u->liveCellSum+=liveCellNow;
    u->liveCellNow=liveCellNow;
    u->evolveCount++;
//    printf("%d %d %d %d\n",u->totelCellCount,u->liveCellNow,u->liveCellSum,u->evolveCount);
}

void print_statistics(struct universe *u){
    double CCArate = 0.0, CAOArate = 0.0;
    CCArate = 100 * (double)u->liveCellNow / (double)u->totelCellCount;
    CAOArate = 100 * (double)u->liveCellSum / (double)(u->totelCellCount*(u->evolveCount+1));
    printf("%6.3lf%% of cells currently alive\n",CCArate);
    printf("%6.3lf%% of cells alive on average\n",CAOArate);
}
