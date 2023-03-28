//
// Project: Result Set for SimpleSQL //
// Prof. Joe Hummel
// Northwestern University
// CS 211, Winter 2023
//

#include <stdbool.h> // true, false
#include <stdio.h>
#include <stdlib.h>
//
// #include any other system <.h> files?
//
#include <string.h> // strcpy, strcat

//
// #include any other of our ".h" files?
//
#include "ast.h"
#include "execute.h"
#include "resultset.h"
#include "util.h"
//
// implementation of function(s), both private and public
//

struct ColumnMeta *findColumnMeta(struct TableMeta *tablemeta, char *name) { // col in database

  for (int i = 0; i < tablemeta->numColumns; ++i) {
    struct ColumnMeta *cm = tablemeta->columns + i;
    if (icmpStrings(cm->name, name) == 0) {
      return cm;
    }
  }
  return NULL;
}

struct COLUMN * findColumn(struct SELECT * select , char *name) // col in AST
{
  struct COLUMN * pCol = select->columns; 
  while( pCol!=NULL )
  {
     if( icmpStrings(pCol->name , name)==0)
     {
       return  pCol;
     }
     pCol= pCol->next;
  } 
  return NULL;
}
// 需要三个变量，resultset, query, database. query是resultset和database的桥梁
void FilterWhere(struct ResultSet *rs, struct SELECT * select , struct TableMeta *tablemeta ) {
   // find the column in resultSet
   struct WHERE *where = select->where;
   char * colName =where->expr->column->name;
   
   int position= resultset_findColumn( rs, 1,  select->table ,  colName );
   if( position != -1)
   {
    // find the column in database
    struct ColumnMeta * cm =findColumnMeta( tablemeta, colName ); 
    // 把where中的三个关键信息提取出来
    int litType = where->expr->litType;
    int operator= where->expr->operator;
    char *value = where->expr->value;
    
  // 逐行遍历
  for (int i = 0; i < rs->numRows; ) 
  { 
    
    bool flag = 0;
     
    if (cm->colType == COL_TYPE_INT) {
      int c = resultset_getInt(rs, i + 1,  position  );
      int v = atoi(value);
 
      if (operator== EXPR_GT) {
        flag = c > v;
      }
      else if( operator== EXPR_LT)
      {
        flag = c < v;
      }
       else if( operator== EXPR_LTE)
      {
        flag = c <= v;
      }
       else if( operator== EXPR_GTE)
      {
        flag = c >= v;
      } 
       else if( operator== EXPR_NOT_EQUAL)
      {
        flag = c != v;
      }     
       else if( operator== EXPR_EQUAL)
      {
        flag = c == v;
      } 
      
    } else if (cm->colType == COL_TYPE_REAL) {
      double c = resultset_getReal(rs, i + 1, position );
      double v = atof(value);
      if (operator== EXPR_GT) {
        flag = c > v;
      }
      else if( operator== EXPR_LT)
      {
        flag = c < v;
      }
       else if( operator== EXPR_LTE)
      {
        flag = c <= v;
      }
       else if( operator== EXPR_GTE)
      {
        flag = c >= v;
      } 
       else if( operator== EXPR_NOT_EQUAL)
      {
        flag = c != v;
      }     
       else if( operator== EXPR_EQUAL)
      {
        flag = c == v;
      } 
    } else if (cm->colType == COL_TYPE_STRING) {
      char *c = resultset_getString(rs, i + 1, position );
      char *v = value;
      if (operator== EXPR_GT) {
        flag = strcmp(c, v) > 0;
      }
      else if( operator== EXPR_LT)
      {
        flag = strcmp(c, v) < 0;
      }
       else if( operator== EXPR_LTE)
      {
           flag = strcmp(c, v) <= 0;
      }
       else if( operator== EXPR_GTE)
      {
           flag = strcmp(c, v) >= 0;
      } 
       else if( operator== EXPR_NOT_EQUAL)
      {
           flag = strcmp(c, v) != 0;
      }     
       else if( operator== EXPR_EQUAL)
      {
           flag = strcmp(c, v) ==0;
      } 
      free(c); // char* c
    }

    // should delete if flag == true
    if (!flag) {
      resultset_deleteRow(rs, i+1); // getReal i+1, del i+1, then continue loop, still judge from i+1 in resultSet
    }
    else // 对下一行进行比对
    {
      ++i;
    }
  }

     
   }

  
}


void DeleteColumns(struct ResultSet *rs, struct SELECT *select) // resultSet, AST
{
  struct RSColumn *pCol = rs->columns;  
  while (pCol != NULL) 
  {
    struct RSColumn * tmp =  pCol->next;  // tmp to move to the next col in rs
    // 看一下AST select里有没有这个column
    struct COLUMN *  c=findColumn( select,  pCol->colName );
    if( c== NULL) // not occur in AST link-list
    {
      //need to delete in resultSet
      int position= resultset_findColumn( rs, 1,  select->table ,  pCol->colName); //找出再resultSet里的位置
      if( position != -1) // 位置存在
      {
        resultset_deleteColumn( rs, position); // 删除
      }
    } 
    
    pCol = tmp; // 因为要删除，所以需要几下下一个位置
  } 
}

// find the col in AST linkList, find the corresponding col in resultSet, move
void  MoveColumns(struct ResultSet *rs, struct SELECT *select)  // 根据query改resultSet，所以不需要database
{
  int i=1; // column index from 1
  struct COLUMN * pCol = select->columns; // AST
  while( pCol!=NULL )
  {
      int position= resultset_findColumn( rs, 1,  select->table ,  pCol->name); // 找出resultSet中的位置
      if( position != -1)
      { 
        resultset_moveColumn( rs, position ,i); // i和AST linklist的顺序同步，再插回去
      }
     pCol= pCol->next;
     ++i;
  }  
}


void ApplyFunctions(struct ResultSet *rs, struct SELECT *select)
{
  int i=1;
  struct COLUMN * pCol = select->columns; 
  while( pCol!=NULL ) // traver every funtion in AST column link list
  {
     if(pCol->function != NO_FUNCTION) // 如果AST有function，
     { // 就把resultSet对应的column找出来
        int position= resultset_findColumn( rs, 1,  select->table ,  pCol->name);
        if( position != -1)
        {
          resultset_applyFunction(rs, pCol->function , position); // 然后施加对应AST的function
        } 
     }
    
     pCol= pCol->next; // 遍历AST下一个column
     ++i;
  }  
}



void execute_query(struct Database *db, struct QUERY *query) {

  struct SELECT *select = query->q.select; // alias for less

  struct TableMeta *tablemeta = NULL; // find the table 3.a
  for (int t = 0; t < db->numTables; t++) { 
    if (icmpStrings(db->tables[t].name, select->table) == 0) {
      tablemeta = &(db->tables[t]);
      break;
    }
  }

  char path[(2 * DATABASE_MAX_ID_LENGTH) + 10]; // 因为和主程序不在一个文件夹内

  strcpy(path, db->name); // fileName/tableName.data database file name
  strcat(path, "/");
  strcat(path, tablemeta->name);
  strcat(path, ".data");

  FILE *datafile = fopen(path, "r"); // 4.a
  if (datafile == NULL) // unable to open:
  {
    printf("**INTERNAL ERROR: table's data file '%s' not found.\n", path);
    panic("execution halted");
  }

  //
  // (3) allocate a buffer for each line of input, and start reading the data:
  //
  int dataBufferSize =
      tablemeta->recordSize + 3; // ends with $\n + null terminator 4.b
  // allocate a memory to store the data
  char *dataBuffer = (char *)malloc(sizeof(char) * dataBufferSize); // data in each line
  if (dataBuffer == NULL)
    panic("out of memory");

  struct ResultSet *rs = resultset_create(); // 3.a create resultSet

  //  add ALL columns from the table to the resultset 3.b
  for (int i = 0; i < tablemeta->numColumns; ++i) {
    struct ColumnMeta *cm = tablemeta->columns + i;
    resultset_insertColumn(rs, i + 1, tablemeta->name, cm->name, NO_FUNCTION,   cm->colType);
  }


  // add row
  while (1) { // insert row line by line from file
    // read from stream(datafile) to str pointer(dataBuffer)
    fgets(dataBuffer, dataBufferSize, datafile); // 4.d read a/each line

    if (feof(datafile)) // end of the data file, we're done
      break;

    // add a row
    int row = resultset_addRow(rs); // add a row with default value

    // break the input line apart  4.k
    char *cp = dataBuffer;

    for (int i = 0; i < tablemeta->numColumns; ++i) { // solve the line column by column
      struct ColumnMeta *cm = tablemeta->columns + i;

       char *pValue = cp;
       int  mark= 0;  // 初始mark = 0, 0 is fasle, 1 is true 遇到了“ ‘
       char markchar=' '; // 标记空格和” ’

      while (1)  // break each part 4.l
      {
        
        if ((*cp == ' ') && ( mark == 0 ) ) {
          break;
        }

        if ( *cp == '\'' || *cp == '\"')  // 遇到""或者''
        {
          if( mark == 0)
          {
            mark =1; // 遇到第一个"或者'
            markchar = *cp;
          }
          else if(mark ==1) // 又遇到"或者'
          {
            if( markchar == *cp  ) // “ == ” 或者 ' == '
            {
              mark=0;
            }
          }
        }

        cp++;
      }
      *cp++ = '\0'; // 把空格替换成\0，并且让cp指向下一个格

      int col = i + 1; // at the beginning, i = 0, so the first col should be 1
      if (cm->colType == COL_TYPE_INT) { // based on current coulumn type
        int v = atoi(pValue);
        resultset_putInt(rs, row, col, v);
      } else if (cm->colType == COL_TYPE_REAL) {
        double v = atof(pValue);
        resultset_putReal(rs, row, col, v);
      } else if (cm->colType == COL_TYPE_STRING) {
        char dst[1024] = {0};
        strncpy(dst, pValue + 1, strlen(pValue) - 2); // pValue从引号后的字符开始，strlen-2是减去两个引号后的总长度
        resultset_putString(rs, row, col, dst);
      }
    }
  }

  free(dataBuffer);
 // 到此为止，所有row已经存好了数据。即，把整个table赋给resultSet。

  // there’s a where clause
  if (select->where != NULL) {
    FilterWhere(rs, select ,tablemeta);
  }


  //Apply the limit clause
  if(select->limit != NULL)
  {
    int N= select->limit->N;
    for ( int i = rs->numRows; i>N ;i--) 
    { 
      resultset_deleteRow(rs, i);
    } 
  }

  // for the aggregate functions
  ApplyFunctions(rs,select);

   //Delete any columns from the resultset that are not in the query
  DeleteColumns(rs,select); // 把query中没select的column删掉

  // find the column in the resultset  and move it
  MoveColumns(rs, select);

  
  resultset_print(rs);
 
  resultset_destroy( rs );
}
