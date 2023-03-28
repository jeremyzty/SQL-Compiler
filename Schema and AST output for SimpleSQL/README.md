1. Inputs a database name, and opens this database by calling the provided database_open( ) function.<br>
2. Traverses and outputs the database meta-data (“schema”) returned by database_open( ).<br>
3. Inputs a query from the user, calls the provided parser and analyzer functions to parse and analyze this query.<br>
4. If analysis was successful, traverses and outputs the AST returned by the analyzer_build( ) function.<br>
5. For the table specified in the query’s From clause, outputs the first 5 lines from the table’s data file.<br>
