#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INT_MAX (16711568 / 8)

typedef struct _options *options;
struct _options
{
  int n;
  int r;
  char *o;
  char *i;
  char *h;
};
struct input_st
{
  char **data;
  int row;
};
options parse_options(int argc, char *argv[]);
struct input_st read_input(FILE *input);
int cmpstr(const void *a, const void *b);
int cmpnum(const void *a, const void *b);

options parse_options(int argc, char *argv[])
{
  char *HELP =
      "sort: can read %d number of lines from standard input or file specified.\n\
sort: can read %d number of character pre line.\n\n\
Usage: ./sort [infile] [-n: sort numbers] [-r: reverse order] [-h: print helps] [-o outfile]\n";
  struct _options _op;
  options __op = (options)malloc(sizeof(_op));
  __op->r = 0;
  __op->n = 0;
  __op->h = (char *)malloc(sizeof(char) * INT_MAX);
  __op->o = (char *)malloc(sizeof(char) * INT_MAX);
  __op->i = (char *)malloc(sizeof(char) * INT_MAX);
  if (__op->h == NULL || __op->o == NULL || __op->i == NULL)
  {
    fprintf(stderr, "Fail to allocate memory space. 3\n");
    exit(1);
  }

  strcpy(__op->h, HELP);

  if (argc > 1)
  {
    for (int i = 1; i < argc; i++)
    {
      if (strcmp(__op->o, argv[i]) == 0)
        continue;

      if (argv[i][0] == '-')
      {
        char options = argv[i][1];
        switch (options)
        {
        case 'n':
          __op->n = 1;
          break;
        case 'o':
          if (i + 1 < argc)
          {
            strcpy(__op->o, argv[i + 1]);
          }
          else
          {
            fprintf(stderr, "sort: option requires an argument -- o\n\n");
            fprintf(stderr, __op->h, INT_MAX, INT_MAX);
            exit(1);
          }
          break;
        case 'r':
          __op->r = 1;
          break;
        case 'h':
          fprintf(stdout, __op->h, INT_MAX, INT_MAX);
          exit(0);
          break;
        default:
          break;
        }
        continue;
      }

      // here should be the file name
      strcpy(__op->i, argv[i]);
    }
  }
  return __op;
}

struct input_st read_input(FILE *input)
{
  char **lines = (char **)malloc(sizeof(char *) * INT_MAX);
  char *buff = (char *)malloc(sizeof(char) * INT_MAX);
  struct input_st _input_st;
  int index = 0;

  if (lines == NULL || buff == NULL)
  {
    fprintf(stderr, "Fail to allocate memory space. 1\n");
    exit(1);
  }
  // read lines
  while (fgets(buff, INT_MAX, input))
  {
    char *line = (char *)malloc(sizeof(char) * INT_MAX);
    if (line == NULL)
    {
      fprintf(stderr, "Fail to allocate memory space. 2\n");
      exit(1);
    }
    strcpy(line, buff);
    lines[index] = line;
    index++;
  }

  free(buff);

  _input_st.data = lines;
  _input_st.row = index;

  return _input_st;
}

int cmpstr(const void *a, const void *b)
{
  return strcmp(*(char **)a, *(char **)b);
}

int cmpnum(const void *a, const void *b)
{
  double num_a = atof(*(char **)a);
  double num_b = atof(*(char **)b);
  // two cases when atoi() returns 0
  // 1: when -n flag is ticked but is passed by non-number string
  // 2: when a and b are both 0 integer
  if (num_a == 0.0 && num_b == 0.0)
    return strcmp(*(char **)a, *(char **)b);
  return num_a - num_b;
}

int main(int argc, char *argv[])
{
  options cmd_ops = parse_options(argc, argv);
  FILE *infile = fopen(cmd_ops->i, "r");
  FILE *outfile = fopen(cmd_ops->o, "w");
  struct input_st inputs;

  if (infile == NULL)
    infile = stdin;
  if (outfile == NULL)
    outfile = stdout;

  inputs = read_input(infile);
  fclose(infile);

  if (cmd_ops->n == 1)
    qsort(inputs.data, inputs.row, sizeof(char *), cmpnum);
  else
    qsort(inputs.data, inputs.row, sizeof(char *), cmpstr);

  // check if reverse flag ticked
  if (cmd_ops->r == 1)
    for (int i = inputs.row - 1; i >= 0; i--)
      fprintf(outfile, "%s", inputs.data[i]);
  else
    for (int i = 0; i < inputs.row; i++)
      fprintf(outfile, "%s", inputs.data[i]);
  if (strlen(cmd_ops->o) > 0)
    fclose(outfile);

  // free pointers
  for (int i = 0; i < INT_MAX; i++)
    free(inputs.data[i]);
  free(inputs.data);
  free(cmd_ops->o);
  free(cmd_ops->i);
  free(cmd_ops->h);
  free(cmd_ops);
  return 0;
}
