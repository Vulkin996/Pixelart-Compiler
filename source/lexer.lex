%option noyywrap
%option nounput
%option noinput

%{
#include <iostream>
#include <string>
#include <cmath>
#include "./header/ast.hpp"
#include "parser.tab.hpp"

using namespace std;

void lexicalError(string msg){
	cerr << "Lexical error: " << msg << endl;
	exit(EXIT_FAILURE);
}

%}

%%
"print" { //cout << "print_token ";
					return print_token; }
"add"   { //cout << "add_token ";
					return add_token; }
"mul"   { //cout << "mul_token ";
					return mul_token; }
"++"    { //cout << "inc_token ";
					return inc_token; }
"jump"  { //cout << "jump_token ";
					return jump_token; }
"label" { //cout << "label_token ";
					return label_token; }
"if" { //cout << "label_token ";
					return if_token; }
"else" { //cout << "label_token ";
					return else_token; }
"while" { //cout << "label_token ";
					return while_token; }
"end_block" { //cout << "label_token ";
					return end_block_token; }
"==" {
	return comp_less_token;
}
"<" {
	return comp_equal_token;
}
0[0-9a-f]* {
						yylval.i = strtol(yytext, NULL, 16) - pow(16, 4)/2;
						//cout << "num_token(" << yylval.i << ") " ;
						return num_token; }
1[0-9a-f]* {
						yylval.i = strtol(yytext+1, NULL, 16);;
						//cout << "address_token(" << yylval.i << ") " ;
						return address_token; }
[ \t\n] {} //ignore whitespace
. {
	lexicalError("Unknown character: " + *yytext);
}
%%
