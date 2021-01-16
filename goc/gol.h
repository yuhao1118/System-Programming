#ifndef GOL_H
#define GOL_H


struct universe {
/*Put some appropriate things here*/
    char** data;
    int col,row;
    //+1
    int totelCellCount;
    int liveCellNow;
    int liveCellSum;
    int evolveCount;
};

/*Do not modify the next seven lines*/
void read_in_file(FILE *infile, struct universe *u);
void write_out_file(FILE *outfile, struct universe *u);
int is_alive(struct universe *u, int column, int row);
int will_be_alive(struct universe *u, int column, int row);
int will_be_alive_torus(struct universe *u,  int column, int row);
void evolve(struct universe *u, int (*rule)(struct universe *u, int column, int row));
void print_statistics(struct universe *u);
/*You can modify after this line again*/

#endif /* GOL_H */
