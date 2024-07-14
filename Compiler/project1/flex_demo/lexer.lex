%{
#include "lexer.h"
%}
%option     nounput
%option     noyywrap

WS          [ \t]
COMMENTS    "(*"([^\*]|(\*)*[^"*)"])*(\"[^\"\n]*\")*(\*)*"*)"
KEYWORD   "AND"|"ELSIF"|"LOOP"|"PROGRAM"|"VAR"|"ARRAY"|"END"|"MOD"|"READ"|"WHILE"|"BEGIN"|"EXIT"|"NOT"|"RECORD"|"WRITE"|"BY"|"FOR"|"OF"|"RETURN"|"DIV"|"IF"|"OR"|"THEN"|"DO"|"IN"|"OUT"|"TO"|"ELSE"|"IS"|"PROCEDURE"|"TYPE"
IDENTIFIER  ({letter}({letter}|{digit})*)
INTEGER     {digit}+
REAL        {digit}+"."{digit}*
STRING      \"[^\"\n]*{COMMENTS}*\"

digit       [0-9]
letter      [A-Z|a-z]
operator    ":="|"+"|"-"|"*"|"/"|"<"|"<="|">"|">="|"="|"<>"
delimiter   ":"|";"|","|"."|"("|")"|"["|"]"|"{"|"}"|"[<"|">]"|\\
enter       \n

invalidstr  \"[^\"\n]*\n
invalidcom  "(*"([^\*]|(\*)*[^"*)"])*(\"[^\"\n]*\")*(\*)*
bell        \a


%%
{enter}     return ENTER;
{WS}        return BLANK;
{invalidcom} return INVALIDCOM;
{COMMENTS}  return COMMENTS;
<<EOF>>     return T_EOF;
{KEYWORD}   return KEYWORD;
{invalidstr} return INVALIDSTR;
{STRING}    return STRING;
{REAL}      return REAL;
{INTEGER}   return INTEGER;
{operator}  return OPERATOR;
{delimiter} return DELIMITER;
{IDENTIFIER} return IDENTIFIER;
{bell}      return BELL;
%%
