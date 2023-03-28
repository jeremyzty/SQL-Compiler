/*main.c*/
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "analyzer.h"
#include "ast.h"
#include "database.h"
#include "parser.h"
#include "scanner.h"
#include "token.h"
#include "tokenqueue.h"
#include "util.h"

//
// header comment with Name, etc.
//

// #include files

// your functions

// int main()

// your functions

char *ColumnTypeStr(int v) {
  switch (v) {
  case COL_TYPE_INT:
    return "int";
    break;

  case COL_TYPE_REAL:
    return "real";
    break;

  case COL_TYPE_STRING:
    return "string";
    break;
  }

  return "unknow";
}

char *IndexTypeStr(int v) {
  switch (v) {
  case COL_NON_INDEXED:
    return "non-indexed";
    break;

  case COL_INDEXED:
    return "indexed";
    break;

  case COL_UNIQUE_INDEXED:
    return "unique indexed";
    break;
  }

  return "unknow";
}

void print_schema(struct Database *pDB) {
  printf("**DATABASE SCHEMA**\n");
  printf("Database: %s\n", pDB->name);

  for (int i = 0; i < pDB->numTables; i++) {//array
    struct TableMeta *pTM = pDB->tables + i;
    printf("Table: %s\n", pTM->name);
    printf("  Record size: %d\n", pTM->recordSize);
    for (int j = 0; j < pTM->numColumns; j++) {
      struct ColumnMeta *pCM = pTM->columns + j;//array
      printf("  Column: %s, %s, %s\n", pCM->name, ColumnTypeStr(pCM->colType),
             IndexTypeStr(pCM->indexType));
    }
  }
  printf("**END OF DATABASE SCHEMA**\n");
}

void print_ast(struct QUERY *qu) {
  if (qu != NULL) {
    if (qu->queryType == SELECT_QUERY) {
      printf("**QUERY AST**\n");
      struct SELECT *select = qu->q.select;  
      printf("Table: %s\n", select->table);

      struct COLUMN *p = select->columns;   //list
      while (p != NULL) {
        if (p->function == NO_FUNCTION) {
          printf("Select column: %s.%s\n", p->table, p->name);
        } else {
          char *fun = "";
          if (p->function == MAX_FUNCTION) {
            fun = "MAX";
          } else if (p->function == MIN_FUNCTION) {
            fun = "MIN";
          } else if (p->function == AVG_FUNCTION) {
            fun = "AVG";
          } else if (p->function == SUM_FUNCTION) {
            fun = "SUM";
          } else if (p->function == COUNT_FUNCTION) {
            fun = "COUNT";
          }

          printf("Select column: %s(%s.%s)\n", fun, p->table, p->name);
        }

        p = p->next;
      }

      struct JOIN *join = select->join; //pointer
      if (join == NULL) {
        printf("Join (NULL)\n");
      }
      else
      {
        struct COLUMN *left =  join->left;
        struct COLUMN *right =  join->right;
        printf("Join %s On %s.%s = %s.%s\n",  join->table ,   left->table ,left->name ,   right->table ,right->name   );
      }

      struct WHERE *where = select->where;//pointer
      if (where == NULL) {
        printf("Where (NULL)\n");
      } else {
        struct COLUMN *col = where->expr->column;
        int type = where->expr->litType;
        int oper = where->expr->operator;

        char *strOpe = "";
        if (oper == EXPR_EQUAL) {
          strOpe = "=";
        } else if (oper == EXPR_GTE) {
          strOpe = ">=";
        } else if (oper == EXPR_NOT_EQUAL) {
          strOpe = "<>";
        } else if (oper == EXPR_LT) {
          strOpe = "<";
        } else if (oper == EXPR_LTE) {
          strOpe = "<=";
        } else if (oper == EXPR_GT) {
          strOpe = ">";
        } else if (oper == EXPR_LIKE) {
          strOpe = "like";
        }

        if (type == STRING_LITERAL) {

          if( strchr( where->expr->value ,  '\'') == NULL)
          {
                      printf("Where %s.%s %s '%s'\n", col->table, col->name, strOpe,
                 where->expr->value);
          }
          else
          {
               printf("Where %s.%s %s \"%s\"\n", col->table, col->name, strOpe,
                 where->expr->value);
          }

        } else {
          printf("Where %s.%s %s %s\n", col->table, col->name, strOpe,
                 where->expr->value);
        }
      }

      struct ORDERBY *order = select->orderby;//pointer
      if (order == NULL) {
        printf("Order By (NULL)\n");
      } else {
        struct COLUMN *col = order->column;
        char *asc = order->ascending ? "ASC" : "DESC";

        if (col->function == NO_FUNCTION) {
          printf("Order By %s.%s %s\n", col->table, col->name, asc);
        } else {
          char *fun = "";
          if (col->function == MAX_FUNCTION) {
            fun = "MAX";
          } else if (col->function == MIN_FUNCTION) {
            fun = "MIN";
          } else if (col->function == AVG_FUNCTION) {
            fun = "AVG";
          } else if (col->function == SUM_FUNCTION) {
            fun = "SUM";
          } else if (col->function == COUNT_FUNCTION) {
            fun = "COUNT";
          }

          printf("Order By %s(%s.%s) %s\n", fun, col->table, col->name, asc);
        }
      }

      struct LIMIT *limit = select->limit;//pointer
      if (limit == NULL) {
        printf("Limit (NULL)\n");
      }
      else
      {
         printf("Limit %d\n", limit->N);
      }

      struct INTO *into = select->into;//pointer
      if (into == NULL) {
        printf("Into (NULL)\n");
      }
      else
      {
        printf("Into %s\n", into->table);
      }

      printf("**END OF QUERY AST**\n");
    }
  }
}

void execute_query(struct Database *db, struct QUERY *qu) {
  if (qu != NULL) {

    struct SELECT *select = qu->q.select;//pointer
    char *table = select->table;

    for (int i = 0; i < db->numTables; i++) {
      struct TableMeta *pTM = db->tables + i;
      if (icmpStrings(pTM->name, table) == 0) {
        table = pTM->name;
        break;
      }
    }

    char *path = (char *)malloc(256);
    sprintf(path, "./%s/%s.data", db->name, table);
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
      return;
    }

    char *buf = (char *)malloc(1024);
    for (int j = 0; j < 5; ++j) {
      fgets(buf, 1024, fp);
      printf("%s", buf);
    }

    fclose(fp);
    
  free(path); 
  free(buf);
  }
}

int main() {
  printf("database? ");
  char *dbname = (char *)malloc(DATABASE_MAX_ID_LENGTH);
  scanf("%s", dbname);
  struct Database *db = database_open(dbname);
  if (db == NULL) {
    printf("**Error: unable to open database '%s'\n", dbname); 
    free(dbname);
    exit(-1);
  }
  
  print_schema(db);

  while (1) {
    printf("query? ");

    parser_init();
    struct TokenQueue *pTQ = parser_parse(stdin);
    if (pTQ == NULL) {
      if (parser_eof()) {
        break;
      }
    } else {
      struct QUERY *qu = analyzer_build(db, pTQ);
      print_ast(qu);
      execute_query(db, qu);
    }
  }

  free(dbname);
  database_close(db);

  return 0;
}
