/*scanner.c*/

//
// Scanner for SimpleSQL programming language. The scanner reads the input
// stream and turns the characters into language Tokens, such as identifiers,
// keywords, and punctuation.
//

#include <stdio.h>
#include <stdbool.h>  // true, false
#include <ctype.h>    // isspace, isdigit, isalpha
#include <string.h>   // stricmp
#include <strings.h>
#include "util.h"
#include "scanner.h"


//
// SimpleSQL keywords, in alphabetical order. Note that "static" means 
// the array / variable is not accessible outside this file, which is
// intentional and good design.
//
static char* keywords[] = { "asc", "avg", "by", "count", "delete", "desc", "from", "inner",
  "insert", "intersect", "into", "join", "like", "limit", "max", "min", "on", "order", 
  "select", "set", "sum", "union", "update", "values", "where" };

static int numKeywords = sizeof(keywords) / sizeof(keywords[0]);


//
// scanner_init
//
// Initializes line number, column number, and value before
// the start of the next input sequence. 
//
void scanner_init(int* lineNumber, int* colNumber, char* value) {
    if (lineNumber == NULL || colNumber == NULL || value == NULL)
        panic("one or more parameters are NULL (scanner_init)");

    *lineNumber = 0;
    *colNumber  = 0;
    value[0]    = '\0';  // empty string ""
}


//
// scanner_nextToken
//
// Returns the next token in the given input stream, advancing the line
// number and column number as appropriate. The token's string-based 
// value is returned via the "value" parameter. For example, if the 
// token returned is an integer literal, then the value returned is
// the actual literal in string form, e.g. "123". For an identifer,
// the value is the identifer itself, e.g. "ID" or "title". For a 
// string literal, the value is the contents of the string literal
// without the quotes.
//
struct Token scanner_nextToken(FILE* input, int* lineNumber, int* colNumber, char* value) {
    if (input == NULL)
        panic("input stream is NULL (scanner_nextToken)");
    if (lineNumber == NULL || colNumber == NULL || value == NULL)
        panic("one or more parameters are NULL (scanner_nextToken)");

    struct Token T;

    //
    // repeatedly input characters one by one until a token is found:
    //
    while (true) {
        int c = fgetc(input);

        if(c == EOF){          // no more input, return EOS:
            T.id = SQL_EOS;
            T.line = *lineNumber;
            T.col = *colNumber;

            value[0] = '$';
            value[1] = '\0';

            return T;
        }
        else{
            if(*lineNumber == 0)
                *lineNumber = 1;
            *colNumber = *colNumber + 1;
        }

        if(c == '$'){     // this is also EOF / EOS
            T.id = SQL_EOS;
            T.line = *lineNumber;
            T.col = *colNumber;

            value[0] = '$';
            value[1] = '\0';

            return T;
        }
        else if(c == ';'){     // read a semicolon
            T.id = SQL_SEMI_COLON;
            T.line = *lineNumber;
            T.col = *colNumber;

            value[0] = ';';
            value[1] = '\0';

            return T;
        }
        else if(c == '('){
            T.id = SQL_LEFT_PAREN;
            T.line = *lineNumber;
            T.col = *colNumber;

            value[0] = '(';
            value[1] = '\0';

            return T;
        }
        else if(c == ')'){
            T.id = SQL_RIGHT_PAREN;
            T.line = *lineNumber;
            T.col = *colNumber;

            value[0] = ')';
            value[1] = '\0';

            return T;
        }
        else if(c == '*'){
            T.id = SQL_ASTERISK;
            T.line = *lineNumber;
            T.col = *colNumber;

            value[0] = '*';
            value[1] = '\0';

            return T;
        }
        else if(c == '.'){
            T.id = SQL_DOT;
            T.line = *lineNumber;
            T.col = *colNumber;

            value[0] = '.';
            value[1] = '\0';

            return T;
        }
        else if (c == '#') {
            T.id = SQL_HASH;
            T.line = *lineNumber;
            T.col = *colNumber;

            value[0] = '#';
            value[1] = '\0';

            return T;
        }
        else if(c == ','){
            T.id = SQL_COMMA;
            T.line = *lineNumber;
            T.col = *colNumber;

            value[0] = ',';
            value[1] = '\0';

            return T;
        }
        else if(c == '='){
            T.id = SQL_EQUAL;
            T.line = *lineNumber;
            T.col = *colNumber;

            value[0] = '=';
            value[1] = '\0';

            return T;
        }
        else if(c == '>'){ // could be > or >=
            //
            // peek ahead to the next char:
            //
            c = fgetc(input);
            *colNumber = *colNumber + 1;    

            if(c == '='){
                T.id = SQL_GTE;
                T.line = *lineNumber;
                T.col = *colNumber - 1;

                value[0] = '>';
                value[1] = '=';
                value[2] = '\0';

                return T;
            }
            else{
                //
                // if we get here, then next char did not form a token, so 
                // we need to put char back to be processed on next call:
                //
                ungetc(c, input);
                *colNumber = *colNumber - 1;

                T.id = SQL_GT;
                T.line = *lineNumber;
                T.col = *colNumber;

                value[0] = '>';
                value[1] = '\0';

                return T;
            }
        }
        else if(c == '<'){ // could be < or <= or <>
            //
            // peek ahead to the next char:
            //
            c = fgetc(input);
            *colNumber = *colNumber + 1;

            if(c == '='){
                T.id = SQL_LTE;
                T.line = *lineNumber;
                T.col = *colNumber - 1;

                value[0] = '<';
                value[1] = '=';
                value[2] = '\0';

                return T;
            }
            else if(c == '>'){
                T.id = SQL_NOT_EQUAL;
                T.line = *lineNumber;
                T.col = *colNumber - 1;

                value[0] = '<';
                value[1] = '>';
                value[2] = '\0';

                return T;
            }
            else{
                ungetc(c, input);
                *colNumber = *colNumber - 1;

                T.id = SQL_LT;
                T.line = *lineNumber;
                T.col = *colNumber;

                value[0] = '<';
                value[1] = '\0';

                return T;
            }
        }
        else if(c == '-'){
            c = fgetc(input);
            *colNumber = *colNumber + 1;

            if(c == '-'){
                //
                // comment, skip all until newline
                //
                do{
                    c = fgetc(input);
                } while(c != '\n' && c != EOF);

                ungetc(c, input);

                continue;
            }
            else if(isdigit(c)){
                //
                // a negative number (integer or real)
                //

                int i = 0;
                value[i++] = '-';
                do{
                    value[i++] = c;
                    c = fgetc(input);
                    *colNumber = *colNumber + 1;
                } while(isdigit(c) || c=='.');
                value[i] = '\0';

                ungetc(c, input);   // put back extra character
                *colNumber = *colNumber - 1;

                if(strchr(value, '.') == NULL)
                    T.id = SQL_INT_LITERAL;
                else
                    T.id = SQL_REAL_LITERAL;
                T.line = *lineNumber;
                T.col = *colNumber - i + 1;

                return T;
            }
            else{
                //
                // sinle '-' is UNKNOWN token
                //
                ungetc(c, input);
                *colNumber = *colNumber - 1;

                T.id = SQL_UNKNOWN;
                T.line = *lineNumber;
                T.col = *colNumber;

                value[0] = '-';
                value[1] = '\0';

                return T;
            }
        }
        else if(c == '+'){
            c = fgetc(input);
            *colNumber = *colNumber + 1;

            if(isdigit(c)){
                //
                // a positive number (integer or real)
                //

                int i = 0;
                value[i++] = '+';
                do{
                    value[i++] = c;
                    c = fgetc(input);
                    *colNumber = *colNumber + 1;
                } while(isdigit(c) || c=='.');
                value[i] = '\0';

                ungetc(c, input);   // put back extra character
                *colNumber = *colNumber - 1;

                if(strchr(value, '.') == NULL)
                    T.id = SQL_INT_LITERAL;
                else
                    T.id = SQL_REAL_LITERAL;
                T.line = *lineNumber;
                T.col = *colNumber - i + 1;

                return T;
            }
            else{
                //
                // sinle '+' is UNKNOWN token
                //
                ungetc(c, input);
                *colNumber = *colNumber - 1;

                T.id = SQL_UNKNOWN;
                T.line = *lineNumber;
                T.col = *colNumber;

                value[0] = '+';
                value[1] = '\0';

                return T;
            }
        }
        else if(isdigit(c)){
            //
            // a positaive number (no leading '+' sign)
            //

            int i = 0;
            do{
                value[i++] = c;
                c = fgetc(input);
                *colNumber = *colNumber + 1;
            } while(isdigit(c) || c=='.');
            value[i] = '\0';

            ungetc(c, input);   // put back extra character
            *colNumber = *colNumber - 1;

            if(strchr(value, '.') == NULL)
                T.id = SQL_INT_LITERAL;
            else
                T.id = SQL_REAL_LITERAL;
            T.line = *lineNumber;
            T.col = *colNumber - i + 1;

            return T;
        }
        else if(c == '"'){
            //
            // string literal start with '"' (no escape)
            //

            int i = 0;
            while(true){
                c = fgetc(input);
                *colNumber = *colNumber + 1;
                if(c == '"')
                    break;
                value[i++] = c;
            }
            value[i] = '\0';

            T.id = SQL_STR_LITERAL;
            T.line = *lineNumber;
            T.col = *colNumber - i - 1;

            return T;
        }
        else if(c == '\''){
            //
            // string literal start with "'" (no escape)
            //

            int i = 0;
            while(true){
                c = fgetc(input);
                *colNumber = *colNumber + 1;
                if(c == '\'')
                    break;
                value[i++] = c;
            }
            value[i] = '\0';

            T.id = SQL_STR_LITERAL;
            T.line = *lineNumber;
            T.col = *colNumber - i - 1;

            return T;
        }
        else if(isalpha(c)){
            //
            // can be keyword or identifier
            //

            int i = 0;
            do{
                value[i++] = c;
                c = fgetc(input);
                *colNumber = *colNumber + 1;
            } while(isalpha(c) || isdigit(c) || c=='_');
            value[i] = '\0';

            ungetc(c, input);   // put back extra character
            *colNumber = *colNumber - 1;

            //
            // use strcasecmp instead of stricmp, reference:
            //      https://stackoverflow.com/questions/1784767
            //
            if(strcasecmp("ASC", value) == 0){
                T.id = SQL_KEYW_ASC;
            }
            else if(strcasecmp("AVG", value) == 0){
                T.id = SQL_KEYW_AVG;
            }
            else if(strcasecmp("BY", value) == 0){
                T.id = SQL_KEYW_BY;
            }
            else if(strcasecmp("COUNT", value) == 0){
                T.id = SQL_KEYW_COUNT;
            }
            else if(strcasecmp("DELETE", value) == 0){
                T.id = SQL_KEYW_DELETE;
            }
            else if(strcasecmp("DESC", value) == 0){
                T.id = SQL_KEYW_DESC;
            }
            else if(strcasecmp("FROM", value) == 0){
                T.id = SQL_KEYW_FROM;
            }
            else if(strcasecmp("INNER", value) == 0){
                T.id = SQL_KEYW_INNER;
            }
            else if(strcasecmp("INSERT", value) == 0){
                T.id = SQL_KEYW_INSERT;
            }
            else if(strcasecmp("INTERSECT", value) == 0){
                T.id = SQL_KEYW_INTERSECT;
            }
            else if(strcasecmp("INTO", value) == 0){
                T.id = SQL_KEYW_INTO;
            }
            else if(strcasecmp("JOIN", value) == 0){
                T.id = SQL_KEYW_JOIN;
            }
            else if(strcasecmp("LIKE", value) == 0){
                T.id = SQL_KEYW_LIKE;
            }
            else if(strcasecmp("LIMIT", value) == 0){
                T.id = SQL_KEYW_LIMIT;
            }
            else if(strcasecmp("MAX", value) == 0){
                T.id = SQL_KEYW_MAX;
            }
            else if(strcasecmp("MIN", value) == 0){
                T.id = SQL_KEYW_MIN;
            }
            else if(strcasecmp("ON", value) == 0){
                T.id = SQL_KEYW_ON;
            }
            else if(strcasecmp("ORDER", value) == 0){
                T.id = SQL_KEYW_ORDER;
            }
            else if(strcasecmp("SELECT", value) == 0){
                T.id = SQL_KEYW_SELECT;
            }
            else if(strcasecmp("SET", value) == 0){
                T.id = SQL_KEYW_SET;
            }
            else if(strcasecmp("SUM", value) == 0){
                T.id = SQL_KEYW_SUM;
            }
            else if(strcasecmp("UNION", value) == 0){
                T.id = SQL_KEYW_UNION;
            }
            else if(strcasecmp("UPDATE", value) == 0){
                T.id = SQL_KEYW_UPDATE;
            }
            else if(strcasecmp("VALUES", value) == 0){
                T.id = SQL_KEYW_VALUES;
            }
            else if(strcasecmp("WHERE", value) == 0){
                T.id = SQL_KEYW_WHERE;
            }
            else{
                T.id = SQL_IDENTIFIER;
            }

            T.line = *lineNumber;
            T.col = *colNumber - i + 1;

            return T;
        }
        else if (c==' ' || c=='\t' || c=='\r' || c=='\f'){
            //
            // empty space encounter
            //

            continue;
        }
        else if (c == '\n'){
            //
            // newline encounter
            //
            *lineNumber = *lineNumber + 1;
            *colNumber = 0;

            continue;
        }
        else {
            //
            // if we get here, then char denotes an UNKNOWN token:
            //
            T.id = SQL_UNKNOWN;
            T.line = *lineNumber;
            T.col = *colNumber;

            value[0] = (char)c;
            value[1] = '\0';

            return T;
        }
    }//while

  //
  // execution should never get here, return occurs
  // from within loop
  //
}
