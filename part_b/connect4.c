//You can include any of headers defined in the C11 standard here. They are:
//assert.h, complex.h, ctype.h, errno.h, fenv.h, float.h, inttypes.h, iso646.h, limits.h, locale.h, math.h, setjmp.h, signal.h, stdalign.h, stdarg.h, stdatomic.h, stdbool.h, stddef.h, stdint.h, stdio.h, stdlib.h, stdnoreturn.h, string.h, tgmath.h, threads.h, time.h, uchar.h, wchar.h or wctype.h
//You may not include any other headers.
#include <stdio.h>
#include <stdlib.h>
#include "connect4.h"

#define MALLOC_MAX (16711568 / 8)

struct board_structure
{
  char **data; // 2-d array of board1
  int row;     // row length used
  int col;     // col length used
  int x_count;
  int o_count;
  int empty_cells;
  struct winner *winners;
  int winners_len;
  char winner_state;
};

struct actual_move
{
  int row;
  int column;
  int row_direct;
};

struct winner
{
  int row;
  int col;
  int direction;
  char player;
};

struct actual_move parse_move(struct move m, board u);
struct winner winning_check_recur(board u, int direction, char player, int row, int col, int count);
void capi_winner(board u, int direction, char player, int row, int col, int count);
void fall_board(board u);
char to_capital(char player);

board setup_board()
{
  struct board_structure _u;
  board u = (board)malloc(sizeof(_u));
  if (u == NULL)
  {
    fprintf(stderr, "Failed to allocate a memory to a variable.\n");
    exit(1);
  }
  // can have a maximum of 2 winners --> draw the game
  u->winners = (struct winner *)malloc(sizeof(struct winner) * 2);
  if (u->winners == NULL)
  {
    fprintf(stderr, "Failed to allocate a memory to a variable.\n");
    exit(1);
  }
  return u;
}

void cleanup_board(board u)
{
  for (int i = 0; i < u->row; i++)
    free(u->data[i]);
  free(u->data);
  free(u->winners);
  free(u);
}

void read_in_file(FILE *infile, board u)
{
  if (infile == NULL)
  {
    fprintf(stderr, "File stream is null\n");
    exit(5);
  }
  char character;
  int total_rows = 0;
  int total_cols = 0;
  int first_line_meet = 0;

  while (fscanf(infile, "%c", &character) != EOF)
  {
    if (character == '\n')
    {
      total_rows++;
      first_line_meet = 1;
    }
    if (first_line_meet != 1)
    {
      total_cols++;
    }
  }

  if (total_rows < 1 || total_cols < 4 || total_cols > 512)
  {
    fprintf(stderr, "Invaild input size. Check if row and column lengths are correct.\n");
    exit(2);
  }

  u->row = total_rows;
  u->col = total_cols;
  u->data = (char **)malloc(sizeof(char *) * total_rows);
  if (u->data == NULL)
  {
    fprintf(stderr, "Failed to allocate a memory to a variable.\n");
    exit(1);
  }
  for (int i = 0; i < total_rows; i++)
  {
    u->data[i] = (char *)malloc(sizeof(char) * total_cols);
    if (u->data[i] == NULL)
    {
      fprintf(stderr, "Failed to allocate a memory to a variable.\n");
      exit(1);
    }
  }

  rewind(infile);

  int index = 0;
  while (fscanf(infile, "%c", &character) != EOF)
  {
    if (character == '.' || character == 'x' || character == 'o' || character == '\n')
    {
      if (character != '\n')
      {
        int _row = index / total_cols;
        int _col = index % total_cols;
        u->data[_row][_col] = character;
        switch (character)
        {
        case '.':
          u->empty_cells++;
          break;
        case 'x':
          u->x_count++;
          break;
        case 'o':
          u->o_count++;
          break;
        default:
          break;
        }
        index++;
      }
    }
    else
    {
      fprintf(stderr, "Incorrect character detected in the file: \n");
      exit(3);
    }
  }
}

void write_out_file(FILE *outfile, board u)
{
  if (outfile == NULL)
  {
    fprintf(stderr, "File stream is null\n");
    exit(5);
  }
  for (int i = 0; i < u->winners_len; i++)
  {
    struct winner cur_winner = u->winners[i];
    capi_winner(u, cur_winner.direction, cur_winner.player, cur_winner.row, cur_winner.col, 0);
  }
  for (int i = 0; i < u->row; i++)
  {
    for (int j = 0; j < u->col; j++)
      fprintf(outfile, "%c", u->data[i][j]);
    fprintf(outfile, "\n");
  }
}

char next_player(board u)
{
  return u->x_count <= u->o_count ? 'x' : 'o';
}

char current_winner(board u)
{
  int winners_len = 0;
  for (int i = 0; i < u->row; i++)
  {
    for (int j = 0; j < u->col; j++)
    {
      if (u->data[i][j] != '.')
      {
        for (int m = 0; m < 4; m++)
        {
          struct winner _winner = winning_check_recur(u, m, u->data[i][j], i, j, 0);
          if (_winner.player != '.')
          {
            u->winners[winners_len] = _winner;
            winners_len++;
            break;
          }
        }
      }
    }
  }
  u->winners_len = winners_len;

  if (u->empty_cells == 0 && winners_len == 0)
    return 'd';

  char player;
  switch (winners_len)
  {
  case 0:
    player = '.';
    break;
  case 1:
    player = u->winners[0].player;
    break;
  case 2:
    player = 'd';
  default:
    player = '.';
    break;
  }
  u->winner_state = player;
  return player;
}

struct move read_in_move(board u)
{
  struct move _m;
  char *row_input = malloc(sizeof(char) * MALLOC_MAX);
  char *col_input = malloc(sizeof(char) * MALLOC_MAX);
  if (row_input == NULL || col_input == NULL)
  {
    fprintf(stderr, "Failed to allocate a memory to a variable.\n");
    exit(1);
  }
  printf("Player %c enter column to place your token: ", next_player(u)); //Do not edit this line
  scanf("%s", col_input);
  printf("Player %c enter row to rotate: ", next_player(u)); // Do not edit this line
  scanf("%s", row_input);
  _m.row = atoi(row_input);
  // if parse failed, return 0, same as do not move a row;
  _m.column = atoi(col_input);
  free(row_input);
  free(col_input);
  return _m;
}

int is_valid_move(struct move m, board u)
{
  struct actual_move act_move = parse_move(m, u);
  if (act_move.row_direct != -1 && act_move.row_direct != 1)
    return 0;
  if (act_move.column < 0 || act_move.column > u->col - 1 || u->data[0][act_move.column] != '.')
    return 0;
  // when act_move.row == u->row, it means the player will not rotate a row
  if (act_move.row < 0 || act_move.row > u->row)
    return 0;
  return 1;
}

char is_winning_move(struct move m, board u)
{
  // copy a new board
  board c_u = setup_board();
  c_u->row = u->row;
  c_u->col = u->col;
  // initial data memory space
  c_u->data = (char **)malloc(sizeof(char *) * u->row);
  if (c_u->data == NULL)
  {
    fprintf(stderr, "Failed to allocate a memory to a variable.\n");
    exit(1);
  }
  for (int i = 0; i < u->row; i++)
  {
    c_u->data[i] = (char *)malloc(sizeof(char) * u->col);
    if (c_u->data[i] == NULL)
    {
      fprintf(stderr, "Failed to allocate a memory to a variable.\n");
      exit(1);
    }
  }
  // copy board data
  for (int i = 0; i < u->row; i++)
  {
    for (int j = 0; j < u->col; j++)
    {
      c_u->data[i][j] = u->data[i][j];
    }
  }

  play_move(m, c_u);
  char winner = c_u->winner_state;
  cleanup_board(c_u);
  return winner;
}

void play_move(struct move m, board u)
{
  if (is_valid_move(m, u) == 1)
  {
    struct actual_move act_m = parse_move(m, u);
    char n_player = next_player(u);
    // update column
    u->data[0][act_m.column] = n_player;
    if (n_player == 'x')
      u->x_count++;
    else if (n_player == 'o')
      u->o_count++;
    u->empty_cells--;
    fall_board(u);
    // update row
    if (act_m.row < u->row)
    {
      if (act_m.row_direct == 1)
      {
        char key = u->data[act_m.row][u->col - 1];
        for (int i = u->col - 2; i >= 0; i--)
          u->data[act_m.row][i + 1] = u->data[act_m.row][i];
        u->data[act_m.row][0] = key;
        fall_board(u);
      }
      else if (act_m.row_direct == -1)
      {
        char key = u->data[act_m.row][0];
        for (int i = 1; i < u->col; i++)
          u->data[act_m.row][i - 1] = u->data[act_m.row][i];
        u->data[act_m.row][u->col - 1] = key;
        fall_board(u);
      }
    }
    current_winner(u);
  }
  else
  {
    printf("Move invaild. Retry!");
  }
  if (u->empty_cells + u->x_count + u->o_count != u->row * u->col)
  {
    fprintf(stderr, "Token inconsistence.\n");
    exit(4);
  }
}

struct actual_move parse_move(struct move m, board u)
{
  struct actual_move act_m;
  act_m.row_direct = m.row < 0 ? -1 : 1;
  act_m.row = u->row - abs(m.row);
  act_m.column = m.column - 1;
  return act_m;
}

void fall_board(board u)
{
  for (int col = 0; col < u->col; col++)
  {
    for (int i = u->row - 1; i >= 0; i--)
    {
      char key = u->data[i][col];
      int j = i + 1;
      while (j < u->row && u->data[j][col] == '.')
      {
        u->data[j - 1][col] = u->data[j][col];
        j++;
      }
      u->data[j - 1][col] = key;
    }
  }
}

struct winner winning_check_recur(board u, int direction, char player, int row, int col, int count)
{
  struct winner _winner = {-1, -1, -1, '.'};
  int next_left_col = col - 1 < 0 ? u->col - 1 : col - 1;
  int next_right_col = col + 1 < u->col ? col + 1 : 0;
  if (u->data[row][col] == player)
  {
    count++;
    if (direction == 0)
    {
      if (row + 1 < u->row)
        return winning_check_recur(u, direction, player, row + 1, col, count);
    }
    else if (direction == 1)
      return winning_check_recur(u, direction, player, row, next_right_col, count);
    else if (direction == 2)
    {
      if (row + 1 < u->row)
        return winning_check_recur(u, direction, player, row + 1, next_left_col, count);
    }
    else if (direction == 3)
    {
      if (row + 1 < u->row)
        return winning_check_recur(u, direction, player, row + 1, next_right_col, count);
    }
  }
  if (count == 4)
  {
    _winner.row = row;
    _winner.col = col;
    _winner.direction = direction;
    _winner.player = player;
    return _winner;
  }
  return _winner;
}

char to_capital(char player)
{
  if (player == 'x')
    return 'X';
  else if (player == 'o')
    return 'O';
  return '.';
}

void capi_winner(board u, int direction, char player, int row, int col, int count)
{
  u->data[row][col] = to_capital(u->data[row][col]);
  if (count == 4)
    return;
  int next_left_col = col - 1 < 0 ? u->col - 1 : col - 1;
  int next_right_col = col + 1 < u->col ? col + 1 : 0;
  count++;
  if (direction == 0)
  {
    if (row >= 0)
      capi_winner(u, direction, player, row - 1, col, count);
  }
  else if (direction == 1)
    capi_winner(u, direction, player, row, next_left_col, count);
  else if (direction == 2)
  {
    if (row >= 0)
      capi_winner(u, direction, player, row - 1, next_right_col, count);
  }
  else if (direction == 3)
  {
    if (row >= 0)
      capi_winner(u, direction, player, row - 1, next_left_col, count);
  }
}
