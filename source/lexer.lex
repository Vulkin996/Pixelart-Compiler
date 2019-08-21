%option noyywrap
%option nounput
%option noinput

%{
#include <iostream>
#include <string>
#include "parser.tab.hpp"

using namespace std;

void lexicalError(string msg){
	cerr << "Lexical error: " << msg << endl;
	exit(EXIT_FAILURE);
}

%}

%%
"print" { cout << "print_token ";
					return print_token; }
"add"   { cout << "add_token ";
					return add_token; }
"mul"   { cout << "mul_token ";
					return mul_token; }
"++"    { cout << "inc_token ";
					return inc_token; }
"jump"  { cout << "jump_token ";
					return jump_token; }
"label" { cout << "label_token ";
					return label_token; }
[0-9a-f]+ { cout << "num_token ";
						return num_token; }
[ \t\n] {} //ignore whitespace
. {
	lexicalError("Unknown character: " + *yytext);
}
%%
