/*main.c*/

//
// Unit tests for SimpleSQL execute_query component.
// These tests are written using the Google Test
// framework.
//
// YOUR NAME
// Northwestern University
// CS 211, Winter 2023
//
// Initial version: Prof. Joe Hummel
//

#include <assert.h>  // assert
#include <ctype.h>   // isdigit
#include <math.h>    // fabs
#include <stdbool.h> // true, false
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // strcpy, strcat

#include "analyzer.h"
#include "ast.h"
#include "database.h"
#include "execute.h"
#include "parser.h"
#include "resultset.h"
#include "scanner.h"
#include "tokenqueue.h"
#include "util.h"

#include "gtest/gtest.h"

//
// Test case: Select * From Movies2 limit 3;
//
// NOTE: this was presented in class on Tues, Jan 31.
// This test is *NOT* complete and considered an
// example of how to structure a test, but it needs
// to be extended with more checking:
//    - correct # of columns, in correct order / types?
//    - correct # of rows?
//    - is the data in each row correct?
//
TEST(execute_query, basic_select) {
  //
  // Write our test query to a file, then open that
  // file for parsing:
  //
  FILE *output = fopen("input.txt", "w");
  fprintf(output, "Select * From Movies2 limit 3;\n$\n");
  fclose(output);
  FILE *input = fopen("input.txt", "r");

  //
  // open the database the query needs, and then
  // parse, analyze, and execute the query:
  //
  struct Database *db = database_open("MovieLens2");
  parser_init();

  struct TokenQueue *tq = parser_parse(input);
  ASSERT_TRUE(tq != NULL);

  struct QUERY *query = analyzer_build(db, tq);
  ASSERT_TRUE(query != NULL);

  struct ResultSet *rs = execute_query(db, query);
  ASSERT_TRUE(rs != NULL);


  ASSERT_TRUE(rs->numCols == 4);
  ASSERT_TRUE(rs->numRows == 3);


  
  struct RSColumn* column1 = rs->columns;
  ASSERT_TRUE(column1 != NULL); 
  ASSERT_TRUE(column1->data != NULL);
  ASSERT_TRUE(column1->coltype == COL_TYPE_INT);
  ASSERT_TRUE(column1->function == NO_FUNCTION);
  ASSERT_TRUE(strcmp( column1->colName, "ID") == 0);   
  ASSERT_TRUE(strcmp( column1->tableName, "Movies2") == 0);   
  ASSERT_TRUE(column1->N ==3 );
  
  struct RSColumn* column2 = column1->next;
  ASSERT_TRUE(column2 != NULL);
  ASSERT_TRUE(column2->data != NULL);
  ASSERT_TRUE(column2->coltype == COL_TYPE_STRING);
  ASSERT_TRUE(column2->function == NO_FUNCTION);
  ASSERT_TRUE(strcmp( column2->colName, "Title") == 0);   
  ASSERT_TRUE(strcmp( column2->tableName, "Movies2") == 0);   
  ASSERT_TRUE(column2->N ==3 );

  
  struct RSColumn* column3 = column2->next;
  ASSERT_TRUE(column3 != NULL);
  ASSERT_TRUE(column3->data != NULL); 
  ASSERT_TRUE(column3->coltype == COL_TYPE_INT);
  ASSERT_TRUE(column3->function == NO_FUNCTION);
  ASSERT_TRUE(strcmp( column3->colName, "Year") == 0);   
  ASSERT_TRUE(strcmp( column3->tableName, "Movies2") == 0);   
  ASSERT_TRUE(column3->N ==3 );
  
  struct RSColumn* column4 = column3->next;
  ASSERT_TRUE(column4 != NULL); 
  ASSERT_TRUE(column4->data != NULL);
  ASSERT_TRUE(column4->coltype == COL_TYPE_REAL);
  ASSERT_TRUE(column4->function == NO_FUNCTION);
  ASSERT_TRUE(strcmp( column4->colName, "Revenue") == 0);   
  ASSERT_TRUE(strcmp( column4->tableName, "Movies2") == 0);   
  ASSERT_TRUE(column4->N ==3 );
  
  ASSERT_TRUE(column4->next == NULL);
  
  //
  // check the data that should be in row 2:
  //
  ASSERT_TRUE(resultset_getInt(rs, 2, 1) == 111);
  ASSERT_TRUE(strcmp(resultset_getString(rs, 2, 2), "Scarface") == 0);
  ASSERT_TRUE(resultset_getInt(rs, 2, 3) == 1983);
  ASSERT_TRUE(fabs(resultset_getReal(rs, 2, 4) - 65884703.0) < 0.00001);

  //
  // TODO: add more checks to this test?
  //
}



//
// TODO: add more tests!
//
TEST(execute_query, where) {
  //
  // Write our test query to a file, then open that
  // file for parsing:


  //
  // open the database the query needs, and then
  // parse, analyze, and execute the query:
  //
  struct Database *db = database_open("MovieLens2");
  parser_init();



  { 

          //
        FILE *output = fopen("input2.txt", "w");
        fprintf(output, "Select * From Movies2 where ID > 1089;\n$\n");
        fclose(output);
        FILE *input = fopen("input2.txt", "r");
          
          
        struct TokenQueue *tq = parser_parse(input);
        ASSERT_TRUE(tq != NULL);
      
        struct QUERY *query = analyzer_build(db, tq);
        ASSERT_TRUE(query != NULL);
      
        struct ResultSet *rs = execute_query(db, query);
        ASSERT_TRUE(rs != NULL);
 
  
    
        ASSERT_TRUE(rs->numCols == 4);
        ASSERT_TRUE(rs->numRows == 3); 

 struct RSColumn* column1 = rs->columns;
  ASSERT_TRUE(column1 != NULL); 
  ASSERT_TRUE(column1->data != NULL);
  ASSERT_TRUE(column1->coltype == COL_TYPE_INT);
  ASSERT_TRUE(column1->function == NO_FUNCTION);
  ASSERT_TRUE(strcmp( column1->colName, "ID") == 0);   
  ASSERT_TRUE(strcmp( column1->tableName, "Movies2") == 0);   
  ASSERT_TRUE(column1->N ==3 );
  
  struct RSColumn* column2 = column1->next;
  ASSERT_TRUE(column2 != NULL);
  ASSERT_TRUE(column2->data != NULL);
  ASSERT_TRUE(column2->coltype == COL_TYPE_STRING);
  ASSERT_TRUE(column2->function == NO_FUNCTION);
  ASSERT_TRUE(strcmp( column2->colName, "Title") == 0);   
  ASSERT_TRUE(strcmp( column2->tableName, "Movies2") == 0);   
  ASSERT_TRUE(column2->N ==3 );

  
  struct RSColumn* column3 = column2->next;
  ASSERT_TRUE(column3 != NULL);
  ASSERT_TRUE(column3->data != NULL); 
  ASSERT_TRUE(column3->coltype == COL_TYPE_INT);
  ASSERT_TRUE(column3->function == NO_FUNCTION);
  ASSERT_TRUE(strcmp( column3->colName, "Year") == 0);   
  ASSERT_TRUE(strcmp( column3->tableName, "Movies2") == 0);   
  ASSERT_TRUE(column3->N ==3 );
  
  struct RSColumn* column4 = column3->next;
  ASSERT_TRUE(column4 != NULL); 
  ASSERT_TRUE(column4->data != NULL);
  ASSERT_TRUE(column4->coltype == COL_TYPE_REAL);
  ASSERT_TRUE(column4->function == NO_FUNCTION);
  ASSERT_TRUE(strcmp( column4->colName, "Revenue") == 0);   
  ASSERT_TRUE(strcmp( column4->tableName, "Movies2") == 0);   
  ASSERT_TRUE(column4->N ==3 );
  
  ASSERT_TRUE(column4->next == NULL);
    
 
          
        ASSERT_TRUE(resultset_getInt(rs, 1, 1) == 1213);
        ASSERT_TRUE(strcmp(resultset_getString(rs, 1, 2), "The Talented Mr. Ripley") == 0);
        ASSERT_TRUE(resultset_getInt(rs, 1, 3) == 1999);
        ASSERT_TRUE(fabs(resultset_getReal(rs, 1, 4) - 128798265.000000) < 0.00001);
  }

  { 

          //
        FILE *output = fopen("input2.txt", "w");
        fprintf(output, "Select * From Movies2 where ID >= 1089;\n$\n");
        fclose(output);
        FILE *input = fopen("input2.txt", "r");
          
          
        struct TokenQueue *tq = parser_parse(input);
        ASSERT_TRUE(tq != NULL);
      
        struct QUERY *query = analyzer_build(db, tq);
        ASSERT_TRUE(query != NULL);
      
        struct ResultSet *rs = execute_query(db, query);
        ASSERT_TRUE(rs != NULL);

     
       
        ASSERT_TRUE(rs->numCols == 4);
        ASSERT_TRUE(rs->numRows == 4);


 struct RSColumn* column1 = rs->columns;
  ASSERT_TRUE(column1 != NULL); 
  ASSERT_TRUE(column1->data != NULL);
  ASSERT_TRUE(column1->coltype == COL_TYPE_INT);
  ASSERT_TRUE(column1->function == NO_FUNCTION);
  ASSERT_TRUE(strcmp( column1->colName, "ID") == 0);   
  ASSERT_TRUE(strcmp( column1->tableName, "Movies2") == 0);    
  
  struct RSColumn* column2 = column1->next;
  ASSERT_TRUE(column2 != NULL);
  ASSERT_TRUE(column2->data != NULL);
  ASSERT_TRUE(column2->coltype == COL_TYPE_STRING);
  ASSERT_TRUE(column2->function == NO_FUNCTION);
  ASSERT_TRUE(strcmp( column2->colName, "Title") == 0);   
  ASSERT_TRUE(strcmp( column2->tableName, "Movies2") == 0);    

  
  struct RSColumn* column3 = column2->next;
  ASSERT_TRUE(column3 != NULL);
  ASSERT_TRUE(column3->data != NULL); 
  ASSERT_TRUE(column3->coltype == COL_TYPE_INT);
  ASSERT_TRUE(column3->function == NO_FUNCTION);
  ASSERT_TRUE(strcmp( column3->colName, "Year") == 0);   
  ASSERT_TRUE(strcmp( column3->tableName, "Movies2") == 0);    
  
  struct RSColumn* column4 = column3->next;
  ASSERT_TRUE(column4 != NULL); 
  ASSERT_TRUE(column4->data != NULL);
  ASSERT_TRUE(column4->coltype == COL_TYPE_REAL);
  ASSERT_TRUE(column4->function == NO_FUNCTION);
  ASSERT_TRUE(strcmp( column4->colName, "Revenue") == 0);   
  ASSERT_TRUE(strcmp( column4->tableName, "Movies2") == 0);    
  
  ASSERT_TRUE(column4->next == NULL);
    
        //
        // check the data that should be in row 2:
        //
        ASSERT_TRUE(resultset_getInt(rs, 2, 1) == 2959);
        ASSERT_TRUE(strcmp(resultset_getString(rs, 2, 2), "License to Wed") == 0);
        ASSERT_TRUE(resultset_getInt(rs, 2, 3) == 2007);
        ASSERT_TRUE(fabs(resultset_getReal(rs, 3, 4) - 83531958.000000) < 0.00001);


        ASSERT_TRUE(resultset_getInt(rs, 3, 1) == 1089);
        ASSERT_TRUE(strcmp(resultset_getString(rs, 3, 2), "Point Break") == 0);
        ASSERT_TRUE(resultset_getInt(rs, 3, 3) == 1991);
        ASSERT_TRUE(fabs(resultset_getReal(rs, 3, 4) - 83531958.000000) < 0.00001);
    
  }

{ 

          //
        FILE *output = fopen("input2.txt", "w");
        fprintf(output, "Select * From Movies2 where ID = 1089;\n$\n");
        fclose(output);
        FILE *input = fopen("input2.txt", "r");
          
          
        struct TokenQueue *tq = parser_parse(input);
        ASSERT_TRUE(tq != NULL);
      
        struct QUERY *query = analyzer_build(db, tq);
        ASSERT_TRUE(query != NULL);
      
        struct ResultSet *rs = execute_query(db, query);
        ASSERT_TRUE(rs != NULL);
       
        ASSERT_TRUE(rs->numCols == 4);
        ASSERT_TRUE(rs->numRows == 1); 

   struct RSColumn* column1 = rs->columns;
  ASSERT_TRUE(column1 != NULL); 
  ASSERT_TRUE(column1->data != NULL);
  ASSERT_TRUE(column1->coltype == COL_TYPE_INT);
  ASSERT_TRUE(column1->function == NO_FUNCTION);
  ASSERT_TRUE(strcmp( column1->colName, "ID") == 0);   
  ASSERT_TRUE(strcmp( column1->tableName, "Movies2") == 0);    
  
  struct RSColumn* column2 = column1->next;
  ASSERT_TRUE(column2 != NULL);
  ASSERT_TRUE(column2->data != NULL);
  ASSERT_TRUE(column2->coltype == COL_TYPE_STRING);
  ASSERT_TRUE(column2->function == NO_FUNCTION);
  ASSERT_TRUE(strcmp( column2->colName, "Title") == 0);   
  ASSERT_TRUE(strcmp( column2->tableName, "Movies2") == 0);    
  
  struct RSColumn* column3 = column2->next;
  ASSERT_TRUE(column3 != NULL);
  ASSERT_TRUE(column3->data != NULL); 
  ASSERT_TRUE(column3->coltype == COL_TYPE_INT);
  ASSERT_TRUE(column3->function == NO_FUNCTION);
  ASSERT_TRUE(strcmp( column3->colName, "Year") == 0);   
  ASSERT_TRUE(strcmp( column3->tableName, "Movies2") == 0);    
  
  struct RSColumn* column4 = column3->next;
  ASSERT_TRUE(column4 != NULL); 
  ASSERT_TRUE(column4->data != NULL);
  ASSERT_TRUE(column4->coltype == COL_TYPE_REAL);
  ASSERT_TRUE(column4->function == NO_FUNCTION);
  ASSERT_TRUE(strcmp( column4->colName, "Revenue") == 0);   
  ASSERT_TRUE(strcmp( column4->tableName, "Movies2") == 0);    
  
  ASSERT_TRUE(column4->next == NULL);

      

        ASSERT_TRUE(resultset_getInt(rs, 1, 1) == 1089);
        ASSERT_TRUE(strcmp(resultset_getString(rs, 1, 2), "Point Break") == 0);
        ASSERT_TRUE(resultset_getInt(rs, 1, 3) == 1991);
        ASSERT_TRUE(fabs(resultset_getReal(rs, 1, 4) - 83531958.000000) < 0.00001);
    
  }

{ 

          //
        FILE *output = fopen("input2.txt", "w");
        fprintf(output, "Select * From Movies2 where ID <= 1089;\n$\n");
        fclose(output);
        FILE *input = fopen("input2.txt", "r");
          
          
        struct TokenQueue *tq = parser_parse(input);
        ASSERT_TRUE(tq != NULL);
      
        struct QUERY *query = analyzer_build(db, tq);
        ASSERT_TRUE(query != NULL);
      
        struct ResultSet *rs = execute_query(db, query);
        ASSERT_TRUE(rs != NULL);

  
        ASSERT_TRUE(rs->numCols == 4);
        ASSERT_TRUE(rs->numRows == 6); 

   struct RSColumn* column1 = rs->columns;
  ASSERT_TRUE(column1 != NULL); 
  ASSERT_TRUE(column1->data != NULL);
  ASSERT_TRUE(column1->coltype == COL_TYPE_INT);
  ASSERT_TRUE(column1->function == NO_FUNCTION);
  ASSERT_TRUE(strcmp( column1->colName, "ID") == 0);   
  ASSERT_TRUE(strcmp( column1->tableName, "Movies2") == 0);    
  
  struct RSColumn* column2 = column1->next;
  ASSERT_TRUE(column2 != NULL);
  ASSERT_TRUE(column2->data != NULL);
  ASSERT_TRUE(column2->coltype == COL_TYPE_STRING);
  ASSERT_TRUE(column2->function == NO_FUNCTION);
  ASSERT_TRUE(strcmp( column2->colName, "Title") == 0);   
  ASSERT_TRUE(strcmp( column2->tableName, "Movies2") == 0);    

  
  struct RSColumn* column3 = column2->next;
  ASSERT_TRUE(column3 != NULL);
  ASSERT_TRUE(column3->data != NULL); 
  ASSERT_TRUE(column3->coltype == COL_TYPE_INT);
  ASSERT_TRUE(column3->function == NO_FUNCTION);
  ASSERT_TRUE(strcmp( column3->colName, "Year") == 0);   
  ASSERT_TRUE(strcmp( column3->tableName, "Movies2") == 0);    
  
  struct RSColumn* column4 = column3->next;
  ASSERT_TRUE(column4 != NULL); 
  ASSERT_TRUE(column4->data != NULL);
  ASSERT_TRUE(column4->coltype == COL_TYPE_REAL);
  ASSERT_TRUE(column4->function == NO_FUNCTION);
  ASSERT_TRUE(strcmp( column4->colName, "Revenue") == 0);   
  ASSERT_TRUE(strcmp( column4->tableName, "Movies2") == 0);    
  
  ASSERT_TRUE(column4->next == NULL);
  
  
        ASSERT_TRUE(resultset_getInt(rs, 1, 1) == 912);
        ASSERT_TRUE(strcmp(resultset_getString(rs, 1, 2), "The Thomas Crown Affair") == 0);
        ASSERT_TRUE(resultset_getInt(rs, 1, 3) == 1968);
        ASSERT_TRUE(fabs(resultset_getReal(rs, 1, 4) - 0.000000) < 0.00001);

 
        ASSERT_TRUE(resultset_getInt(rs,5, 1) == 1089);
        ASSERT_TRUE(strcmp(resultset_getString(rs, 5, 2), "Point Break") == 0);
        ASSERT_TRUE(resultset_getInt(rs, 5, 3) == 1991);
        ASSERT_TRUE(fabs(resultset_getReal(rs, 5, 4) - 83531958.000000) < 0.00001);
  
    
  }

{ 

          //
        FILE *output = fopen("input2.txt", "w");
        fprintf(output, "Select * From Movies2 where ID < 1089;\n$\n");
        fclose(output);
        FILE *input = fopen("input2.txt", "r");
          
          
        struct TokenQueue *tq = parser_parse(input);
        ASSERT_TRUE(tq != NULL);
      
        struct QUERY *query = analyzer_build(db, tq);
        ASSERT_TRUE(query != NULL);
      
        struct ResultSet *rs = execute_query(db, query);
        ASSERT_TRUE(rs != NULL);
 
        ASSERT_TRUE(rs->numCols == 4);
        ASSERT_TRUE(rs->numRows == 5); 

   struct RSColumn* column1 = rs->columns;
  ASSERT_TRUE(column1 != NULL); 
  ASSERT_TRUE(column1->data != NULL);
  ASSERT_TRUE(column1->coltype == COL_TYPE_INT);
  ASSERT_TRUE(column1->function == NO_FUNCTION);
  ASSERT_TRUE(strcmp( column1->colName, "ID") == 0);   
  ASSERT_TRUE(strcmp( column1->tableName, "Movies2") == 0);    
  
  struct RSColumn* column2 = column1->next;
  ASSERT_TRUE(column2 != NULL);
  ASSERT_TRUE(column2->data != NULL);
  ASSERT_TRUE(column2->coltype == COL_TYPE_STRING);
  ASSERT_TRUE(column2->function == NO_FUNCTION);
  ASSERT_TRUE(strcmp( column2->colName, "Title") == 0);   
  ASSERT_TRUE(strcmp( column2->tableName, "Movies2") == 0);    

  
  struct RSColumn* column3 = column2->next;
  ASSERT_TRUE(column3 != NULL);
  ASSERT_TRUE(column3->data != NULL); 
  ASSERT_TRUE(column3->coltype == COL_TYPE_INT);
  ASSERT_TRUE(column3->function == NO_FUNCTION);
  ASSERT_TRUE(strcmp( column3->colName, "Year") == 0);   
  ASSERT_TRUE(strcmp( column3->tableName, "Movies2") == 0);    
  
  struct RSColumn* column4 = column3->next;
  ASSERT_TRUE(column4 != NULL); 
  ASSERT_TRUE(column4->data != NULL);
  ASSERT_TRUE(column4->coltype == COL_TYPE_REAL);
  ASSERT_TRUE(column4->function == NO_FUNCTION);
  ASSERT_TRUE(strcmp( column4->colName, "Revenue") == 0);   
  ASSERT_TRUE(strcmp( column4->tableName, "Movies2") == 0);    
  
  ASSERT_TRUE(column4->next == NULL);

  
        ASSERT_TRUE(resultset_getInt(rs, 1, 1) == 912);
        ASSERT_TRUE(strcmp(resultset_getString(rs, 1, 2), "The Thomas Crown Affair") == 0);
        ASSERT_TRUE(resultset_getInt(rs, 1, 3) == 1968);
        ASSERT_TRUE(fabs(resultset_getReal(rs, 1, 4) - 0.000000) < 0.00001);
 
        
        ASSERT_TRUE(resultset_getInt(rs,5, 1) == 293);
        ASSERT_TRUE(strcmp(resultset_getString(rs, 5, 2), "A River Runs Through It") == 0);
        ASSERT_TRUE(resultset_getInt(rs, 5, 3) == 1992);
        ASSERT_TRUE(fabs(resultset_getReal(rs, 5, 4) - 43440294.000000) < 0.00001);
    
  }

  

  
  { 

          //
        FILE *output = fopen("input2.txt", "w");
        fprintf(output, "Select * From Movies2 where ID <> 2959;\n$\n");
        fclose(output);
        FILE *input = fopen("input2.txt", "r");
          
          
        struct TokenQueue *tq = parser_parse(input);
        ASSERT_TRUE(tq != NULL);
      
        struct QUERY *query = analyzer_build(db, tq);
        ASSERT_TRUE(query != NULL);
      
        struct ResultSet *rs = execute_query(db, query);
        ASSERT_TRUE(rs != NULL);

  
  
        ASSERT_TRUE(rs->numCols == 4);
        ASSERT_TRUE(rs->numRows == 8); 
    
  }

  //////////////////////////////////////////////////////////////



{ 

          //
        FILE *output = fopen("input2.txt", "w");
        fprintf(output, "Select * From Movies2 where Title >='The';\n$\n");
        fclose(output);
        FILE *input = fopen("input2.txt", "r");
          
          
        struct TokenQueue *tq = parser_parse(input);
        ASSERT_TRUE(tq != NULL);
      
        struct QUERY *query = analyzer_build(db, tq);
        ASSERT_TRUE(query != NULL);
      
        struct ResultSet *rs = execute_query(db, query);
        ASSERT_TRUE(rs != NULL);

  

        ASSERT_TRUE(rs->numCols == 4);
        ASSERT_TRUE(rs->numRows == 4);
      
    
  }

{ 

          //
        FILE *output = fopen("input2.txt", "w");
        fprintf(output, "Select * From Movies2 where Title >'Pzzz';\n$\n");
        fclose(output);
        FILE *input = fopen("input2.txt", "r");
          
          
        struct TokenQueue *tq = parser_parse(input);
        ASSERT_TRUE(tq != NULL);
      
        struct QUERY *query = analyzer_build(db, tq);
        ASSERT_TRUE(query != NULL);
      
        struct ResultSet *rs = execute_query(db, query);
        ASSERT_TRUE(rs != NULL);

  
       
       
        ASSERT_TRUE(rs->numCols == 4);
        ASSERT_TRUE(rs->numRows == 5); 
    
  }

{ 

          //
        FILE *output = fopen("input2.txt", "w");
        fprintf(output, "Select * From Movies2 where Title <'Murder She Said';\n$\n");
        fclose(output);
        FILE *input = fopen("input2.txt", "r");
          
          
        struct TokenQueue *tq = parser_parse(input);
        ASSERT_TRUE(tq != NULL);
      
        struct QUERY *query = analyzer_build(db, tq);
        ASSERT_TRUE(query != NULL);
      
        struct ResultSet *rs = execute_query(db, query);
        ASSERT_TRUE(rs != NULL);

  
  
        ASSERT_TRUE(rs->numCols == 4);
        ASSERT_TRUE(rs->numRows == 2); 
    
  }

{ 

          //
        FILE *output = fopen("input2.txt", "w");
         fprintf(output, "Select * From Movies2 where Title <='Murder She Said';\n$\n");
        fclose(output);
        FILE *input = fopen("input2.txt", "r");
          
          
        struct TokenQueue *tq = parser_parse(input);
        ASSERT_TRUE(tq != NULL);
      
        struct QUERY *query = analyzer_build(db, tq);
        ASSERT_TRUE(query != NULL);
      
        struct ResultSet *rs = execute_query(db, query);
        ASSERT_TRUE(rs != NULL);
 
  
        ASSERT_TRUE(rs->numCols == 4);
        ASSERT_TRUE(rs->numRows == 3); 
    
  }

  
 
{ 

          //
        FILE *output = fopen("input2.txt", "w");
         fprintf(output, "Select * From Movies2 where Title ='Murder She Said';\n$\n");
        fclose(output);
        FILE *input = fopen("input2.txt", "r");
          
          
        struct TokenQueue *tq = parser_parse(input);
        ASSERT_TRUE(tq != NULL);
      
        struct QUERY *query = analyzer_build(db, tq);
        ASSERT_TRUE(query != NULL);
      
        struct ResultSet *rs = execute_query(db, query);
        ASSERT_TRUE(rs != NULL);

 
  
        ASSERT_TRUE(rs->numCols == 4);
        ASSERT_TRUE(rs->numRows == 1); 
    
  }

{ 

          //
        FILE *output = fopen("input2.txt", "w");
         fprintf(output, "Select * From Movies2 where Title <>'Murder She Said';\n$\n");
        fclose(output);
        FILE *input = fopen("input2.txt", "r");
          
          
        struct TokenQueue *tq = parser_parse(input);
        ASSERT_TRUE(tq != NULL);
      
        struct QUERY *query = analyzer_build(db, tq);
        ASSERT_TRUE(query != NULL);
      
        struct ResultSet *rs = execute_query(db, query);
        ASSERT_TRUE(rs != NULL); 
  
        ASSERT_TRUE(rs->numCols == 4);
        ASSERT_TRUE(rs->numRows == 8); 
    
  }

 
  
  ////////////////////////////////////////////////////////////////
 

  

  { 

          //
        FILE *output = fopen("input2.txt", "w");
        fprintf(output, "Select * From Movies2 where Revenue <= 65884703.000000;\n$\n");
        fclose(output);
        FILE *input = fopen("input2.txt", "r");
          
          
        struct TokenQueue *tq = parser_parse(input);
        ASSERT_TRUE(tq != NULL);
      
        struct QUERY *query = analyzer_build(db, tq);
        ASSERT_TRUE(query != NULL);
      
        struct ResultSet *rs = execute_query(db, query);
        ASSERT_TRUE(rs != NULL); 
    
        ASSERT_TRUE(rs->numCols == 4);
        ASSERT_TRUE(rs->numRows == 5);
  }


{ 

          //
        FILE *output = fopen("input2.txt", "w");
        fprintf(output, "Select * From Movies2 where Revenue < 43440294.000000;\n$\n");
        fclose(output);
        FILE *input = fopen("input2.txt", "r");
          
          
        struct TokenQueue *tq = parser_parse(input);
        ASSERT_TRUE(tq != NULL);
      
        struct QUERY *query = analyzer_build(db, tq);
        ASSERT_TRUE(query != NULL);
      
        struct ResultSet *rs = execute_query(db, query);
        ASSERT_TRUE(rs != NULL);
       
    
        ASSERT_TRUE(rs->numCols == 4);
        ASSERT_TRUE(rs->numRows == 3);
  }

  

  { 

          //
        FILE *output = fopen("input2.txt", "w");
        fprintf(output, "Select  * From Movies2 where Revenue > 65884703.000000;\n$\n");
        fclose(output);
        FILE *input = fopen("input2.txt", "r");
          
          
        struct TokenQueue *tq = parser_parse(input);
        ASSERT_TRUE(tq != NULL);
      
        struct QUERY *query = analyzer_build(db, tq);
        ASSERT_TRUE(query != NULL);
      
        struct ResultSet *rs = execute_query(db, query);
        ASSERT_TRUE(rs != NULL);
       
    
        ASSERT_TRUE(rs->numCols == 4);
        ASSERT_TRUE(rs->numRows == 4);
  }


 { 

          //
        FILE *output = fopen("input2.txt", "w");
        fprintf(output, "Select * From Movies2 where Revenue <> 65884703.000000;\n$\n");
        fclose(output);
        FILE *input = fopen("input2.txt", "r");
          
          
        struct TokenQueue *tq = parser_parse(input);
        ASSERT_TRUE(tq != NULL);
      
        struct QUERY *query = analyzer_build(db, tq);
        ASSERT_TRUE(query != NULL);
      
        struct ResultSet *rs = execute_query(db, query);
        ASSERT_TRUE(rs != NULL);
       
    
        ASSERT_TRUE(rs->numCols == 4);
        ASSERT_TRUE(rs->numRows == 8);
  }

   { 

          //
        FILE *output = fopen("input2.txt", "w");
        fprintf(output, "Select * From Movies2 where Revenue >= 65884703.000000;\n$\n");
        fclose(output);
        FILE *input = fopen("input2.txt", "r");
          
          
        struct TokenQueue *tq = parser_parse(input);
        ASSERT_TRUE(tq != NULL);
      
        struct QUERY *query = analyzer_build(db, tq);
        ASSERT_TRUE(query != NULL);
      
        struct ResultSet *rs = execute_query(db, query);
        ASSERT_TRUE(rs != NULL);
       
    
        ASSERT_TRUE(rs->numCols == 4);
        ASSERT_TRUE(rs->numRows == 5);
  }

    { 

          //
        FILE *output = fopen("input2.txt", "w");
        fprintf(output, "Select * From Movies2 where Revenue = 65884703.000000;\n$\n");
        fclose(output);
        FILE *input = fopen("input2.txt", "r");
          
          
        struct TokenQueue *tq = parser_parse(input);
        ASSERT_TRUE(tq != NULL);
      
        struct QUERY *query = analyzer_build(db, tq);
        ASSERT_TRUE(query != NULL);
      
        struct ResultSet *rs = execute_query(db, query);
        ASSERT_TRUE(rs != NULL);
      
    
        ASSERT_TRUE(rs->numCols == 4);
        ASSERT_TRUE(rs->numRows == 1);
  }

   
  
  //
  // TODO: add more checks to this test?
  //
}





//
// TODO: add more tests!
//
TEST(execute_query, column) {
  //
  // Write our test query to a file, then open that
  // file for parsing:


  //
  // open the database the query needs, and then
  // parse, analyze, and execute the query:
  //
  struct Database *db = database_open("MovieLens");
  parser_init();
 

   { 

          //
        FILE *output = fopen("input3.txt", "w");
        fprintf(output, "Select Revenue, Title, ID  From Movies where ID = 862;\n$\n");
        fclose(output);
        FILE *input = fopen("input3.txt", "r");
          
          
        struct TokenQueue *tq = parser_parse(input);
        ASSERT_TRUE(tq != NULL);
      
        struct QUERY *query = analyzer_build(db, tq);
        ASSERT_TRUE(query != NULL);
      
        struct ResultSet *rs = execute_query(db, query);
        ASSERT_TRUE(rs != NULL);
       
    
        ASSERT_TRUE(rs->numCols == 3); 
        ASSERT_TRUE(rs->numRows == 1); 

       struct RSColumn* column1 = rs->columns;
       ASSERT_TRUE(column1 != NULL);
       ASSERT_TRUE(column1->data->value.r -373554033.00 < 0.00001);
       ASSERT_TRUE(column1->coltype == COL_TYPE_REAL);
       ASSERT_TRUE(column1->function == NO_FUNCTION);
       ASSERT_TRUE(strcmp( column1->colName, "Revenue") == 0);    
 
     
        struct RSColumn* column2 = column1->next;
        ASSERT_TRUE(column2 != NULL); 
        ASSERT_TRUE(column2->coltype == COL_TYPE_STRING);
        ASSERT_TRUE(column2->function == NO_FUNCTION);
       ASSERT_TRUE(strcmp( column2->colName, "Title") == 0);   
     
      
        struct RSColumn* column3 = column2->next;
        ASSERT_TRUE(column3 != NULL);
        ASSERT_TRUE(column3->data->value.i  == 862 );
        ASSERT_TRUE(column3->coltype == COL_TYPE_INT);
        ASSERT_TRUE(column3->function == NO_FUNCTION);
        ASSERT_TRUE(strcmp( column3->colName, "ID") == 0);   

     
        ASSERT_TRUE(column3->next == NULL);
     
  }
  
  
  
  }





//
// TODO: add more tests!
//
TEST(execute_query, function) {
  //
  // Write our test query to a file, then open that
  // file for parsing:


  //
  // open the database the query needs, and then
  // parse, analyze, and execute the query:
  //
  struct Database *db = database_open("MovieLens");
  parser_init();

  { 

          //
        FILE *output = fopen("input4.txt", "w");
        fprintf(output, "Select Sum(ID) ,Avg(Revenue),Max(year)  From Movies;\n$\n");
        fclose(output);
        FILE *input = fopen("input4.txt", "r");
          
          
        struct TokenQueue *tq = parser_parse(input);
        ASSERT_TRUE(tq != NULL);
      
        struct QUERY *query = analyzer_build(db, tq);
        ASSERT_TRUE(query != NULL);
      
        struct ResultSet *rs = execute_query(db, query);
        ASSERT_TRUE(rs != NULL); 
     
       
        ASSERT_TRUE(rs->numCols == 3);
        ASSERT_TRUE(rs->numRows == 1); 
    
      
      struct RSColumn* column1 = rs->columns;
      ASSERT_TRUE(column1 != NULL);

      ASSERT_TRUE(column1->data->value.i == 36998592);
      ASSERT_TRUE(column1->coltype == COL_TYPE_INT);
      ASSERT_TRUE(column1->function == SUM_FUNCTION);
    
      struct RSColumn* column2 = column1->next;
      ASSERT_TRUE(column2 != NULL);
      ASSERT_TRUE(column2->data->value.r -38431146.157073 < 0.00001);
      ASSERT_TRUE(column2->coltype == COL_TYPE_REAL);
      ASSERT_TRUE(column2->function == AVG_FUNCTION);
    
    
      struct RSColumn* column3 = column2->next;
      ASSERT_TRUE(column3 != NULL);
      ASSERT_TRUE(column3->data->value.i  == 2012 );
      ASSERT_TRUE(column3->coltype == COL_TYPE_INT);
      ASSERT_TRUE(column3->function == MAX_FUNCTION);
    
      ASSERT_TRUE(column3->next == NULL);
    
  }


   { 

          //
        FILE *output = fopen("input4.txt", "w");
        fprintf(output, "Select max(Title) From Movies;\n$\n");
        fclose(output);
        FILE *input = fopen("input4.txt", "r");
          
          
        struct TokenQueue *tq = parser_parse(input);
        ASSERT_TRUE(tq != NULL);
      
        struct QUERY *query = analyzer_build(db, tq);
        ASSERT_TRUE(query != NULL);
      
        struct ResultSet *rs = execute_query(db, query);
        ASSERT_TRUE(rs != NULL);

       

        ASSERT_TRUE(rs->numCols == 1);
        ASSERT_TRUE(rs->numRows == 1); 

        struct RSColumn* column1 = rs->columns;
        ASSERT_TRUE(column1 != NULL);
 
        ASSERT_TRUE(strcmp( column1->data->value.s, "Yankee Zulu") == 0);
       ASSERT_TRUE(column1->coltype == COL_TYPE_STRING);
        ASSERT_TRUE(column1->function == MAX_FUNCTION);
     
        ASSERT_TRUE(column1->next == NULL);    
    
  }


  { 

          //
        FILE *output = fopen("input4.txt", "w");
        fprintf(output, "Select min(Title)  From Movies;\n$\n");
        fclose(output);
        FILE *input = fopen("input4.txt", "r");
          
          
        struct TokenQueue *tq = parser_parse(input);
        ASSERT_TRUE(tq != NULL);
      
        struct QUERY *query = analyzer_build(db, tq);
        ASSERT_TRUE(query != NULL);
      
        struct ResultSet *rs = execute_query(db, query);
        ASSERT_TRUE(rs != NULL);

       

        struct RSColumn* column1 = rs->columns;
        ASSERT_TRUE(column1 != NULL);
 
        ASSERT_TRUE(strcmp( column1->data->value.s, "1-900") == 0);
        ASSERT_TRUE(column1->coltype == COL_TYPE_STRING);
        ASSERT_TRUE(column1->function == MIN_FUNCTION);
    
        ASSERT_TRUE(column1->next == NULL);   
    
  }
  
   { 

          //
        FILE *output = fopen("input4.txt", "w");
        fprintf(output, "Select count(Revenue)  From Movies;\n$\n");
        fclose(output);
        FILE *input = fopen("input4.txt", "r");
          
          
        struct TokenQueue *tq = parser_parse(input);
        ASSERT_TRUE(tq != NULL);
      
        struct QUERY *query = analyzer_build(db, tq);
        ASSERT_TRUE(query != NULL);
      
        struct ResultSet *rs = execute_query(db, query);
        ASSERT_TRUE(rs != NULL);

      

        
      struct RSColumn* column1 = rs->columns;
      ASSERT_TRUE(column1 != NULL);

      ASSERT_TRUE(column1->data->value.i == 1025);
      ASSERT_TRUE(column1->coltype == COL_TYPE_INT);
      ASSERT_TRUE(column1->function == COUNT_FUNCTION);
     
      ASSERT_TRUE(column1->next == NULL);   
    
  }


   
}






//
// TODO: add more tests!
//
TEST(execute_query, order) {
  //
  // Write our test query to a file, then open that
  // file for parsing:


  //
  // open the database the query needs, and then
  // parse, analyze, and execute the query:
  //
  struct Database *db = database_open("MovieLens");
  parser_init();

  { 

          //
        FILE *output = fopen("input5.txt", "w");
        fprintf(output, "Select ID From Movies WHERE id <100  limit 2;\n$\n");
        fclose(output);
        FILE *input = fopen("input5.txt", "r");
          
          
        struct TokenQueue *tq = parser_parse(input);
        ASSERT_TRUE(tq != NULL);
      
        struct QUERY *query = analyzer_build(db, tq);
        ASSERT_TRUE(query != NULL);
      
        struct ResultSet *rs = execute_query(db, query);
        ASSERT_TRUE(rs != NULL); 

    
        // printf("XXXXX %d   XXXX\n",rs-> numRows ); 
        // resultset_print(rs);
     
        ASSERT_TRUE(rs->numRows == 2); 

      struct RSColumn* column1 = rs->columns;
      ASSERT_TRUE(column1 != NULL);
    
      ASSERT_TRUE(column1->N ==2 );
      ASSERT_TRUE(column1->data->value.i == 5);
      ASSERT_TRUE(column1->coltype == COL_TYPE_INT);
      ASSERT_TRUE(column1->function == NO_FUNCTION);

    
  }


}


//
// main program: do not change this, google test will find all your
// tests and run them.  So leave the code below as is!
//
int main() {
  ::testing::InitGoogleTest();

  //
  // run all the tests, returns 0 if they
  // all pass and 1 if any fail:
  //
  int result = RUN_ALL_TESTS();

  return result; // 0 => all passed
}
