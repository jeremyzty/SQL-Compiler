scanner_nextToken

Returns the next token in the given input stream, advancing the line
number and column number as appropriate. The token's string-based
value is returned via the "value" parameter. For example, if the
token returned is an integer literal, then the value returned is
the actual literal in string form, e.g. "123". For an identifer,
the value is the identifer itself, e.g. "ID" or "title". For a
string literal, the value is the contents of the string literal
without the quotes.
