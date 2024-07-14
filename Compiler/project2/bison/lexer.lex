%{
#include "yacc.h"
#include "ast.h"
int row = 1, col = 1;
int tmpint = -1, len = -1;
int tab = 0, eol = 0;
%}
%option     nounput
%option     noyywrap

WS          [ \t]
COMMENTS    "(*"([^\*]|(\*)*[^"*)"])*(\"[^\"\n]*\")*(\*)*"*)"
ID          ({letter}({letter}|{digit})*)
INTEGER     {digit}+
REAL        {digit}+"."{digit}*
STRING      \"[^\"\n]*{COMMENTS}*\"

digit       [0-9]
letter      [A-Z|a-z]
enter       \n

invalidstr  \"[^\"\n]*\n
invalidcom  "(*"([^\*]|(\*)*[^"*)"])*(\"[^\"\n]*\")*(\*)*
bell        \a

%%
<<EOF>>     { return 0; }
{enter}     { row++; col = 1; }
{WS}        { col++; }
{invalidcom} { myerror(0, "an unterminated comment", row, col); for(int i=0;i<strlen(yytext);i++){ if(yytext[i]=='\n') row++; } }
{COMMENTS}  { for(int i=0;i<strlen(yytext);i++){ if(yytext[i]=='\n'){ row++; col=1; } else col++; } }
{invalidstr} { myerror(0, "an unterminated string", row, col); col += strlen(yytext); row++; }
{STRING}    { 
                len = strlen(yytext); tab = 0; eol = 0;
                for(int i=0;i<len;i++){ if(yytext[i]=='\t')tab++; if(yytext[i]=='\n'){ eol++; row++; } }
                if(eol>0){ myerror(0, "an invalid string with enter in it", row, col); col += strlen(yytext); }
                else if(tab==1){ myerror(0, "an invalid string with tab in it", row, col); col += strlen(yytext); }
                else if(tab>1){ myerror(0, "an invalid string with many tabs in it", row, col); col += strlen(yytext); }
                else if(len>257){ myerror(0, "an overly long string", row, col); col += strlen(yytext); }
                else{ yylval.node = newtoken("string", yytext, row, col); col += strlen(yytext); return STRING; }
            }
{REAL}      { yylval.node = newtoken("real", yytext, row, col); col += strlen(yytext); return REAL; }
{INTEGER}   { 
                tmpint = atoi(yytext);
                if(tmpint >= 0) { yylval.node = newtoken("integer", yytext, row, col); col += strlen(yytext); return INTEGER;  }
                else { myerror(0, "an out of range integer", row, col); col += strlen(yytext); }
            }
"PROGRAM"   { yylval.node = newtoken("reserved", yytext, row, col); col += strlen(yytext); return PROGRAM; }
"IS"        { yylval.node = newtoken("reserved", yytext, row, col); col += strlen(yytext); return IS; }
"VAR"       { yylval.node = newtoken("reserved", yytext, row, col); col += strlen(yytext); return VAR; }
"TYPE"      { yylval.node = newtoken("reserved", yytext, row, col); col += strlen(yytext); return TYPE; }
"PROCEDURE" { yylval.node = newtoken("reserved", yytext, row, col); col += strlen(yytext); return PROCEDURE; }
"BEGIN"     { yylval.node = newtoken("reserved", yytext, row, col); col += strlen(yytext); return MYBEGIN; }
"END"       { yylval.node = newtoken("reserved", yytext, row, col); col += strlen(yytext); return END; }
"ARRAY"     { yylval.node = newtoken("reserved", yytext, row, col); col += strlen(yytext); return ARRAY; }
"OF"        { yylval.node = newtoken("reserved", yytext, row, col); col += strlen(yytext); return OF; }
"RECORD"    { yylval.node = newtoken("reserved", yytext, row, col); col += strlen(yytext); return RECORD; }
"WRITE"     { yylval.node = newtoken("reserved", yytext, row, col); col += strlen(yytext); return WRITE; }
"READ"      { yylval.node = newtoken("reserved", yytext, row, col); col += strlen(yytext); return READ; }
"NOT"       { yylval.node = newtoken("reserved", yytext, row, col); col += strlen(yytext); return NOT; }
"AND"       { yylval.node = newtoken("reserved", yytext, row, col); col += strlen(yytext); return AND; }
"OR"        { yylval.node = newtoken("reserved", yytext, row, col); col += strlen(yytext); return OR; }
"MOD"       { yylval.node = newtoken("reserved", yytext, row, col); col += strlen(yytext); return MOD; }
"IF"        { yylval.node = newtoken("reserved", yytext, row, col); col += strlen(yytext); return IF; }
"THEN"      { yylval.node = newtoken("reserved", yytext, row, col); col += strlen(yytext); return THEN; }
"ELSIF"     { yylval.node = newtoken("reserved", yytext, row, col); col += strlen(yytext); return ELSIF; }
"ELSE"      { yylval.node = newtoken("reserved", yytext, row, col); col += strlen(yytext); return ELSE; }
"WHILE"     { yylval.node = newtoken("reserved", yytext, row, col); col += strlen(yytext); return WHILE; }
"DO"        { yylval.node = newtoken("reserved", yytext, row, col); col += strlen(yytext); return DO; }
"LOOP"      { yylval.node = newtoken("reserved", yytext, row, col); col += strlen(yytext); return LOOP; }
"FOR"       { yylval.node = newtoken("reserved", yytext, row, col); col += strlen(yytext); return FOR; }
"TO"        { yylval.node = newtoken("reserved", yytext, row, col); col += strlen(yytext); return TO; }
"BY"        { yylval.node = newtoken("reserved", yytext, row, col); col += strlen(yytext); return BY; }
"EXIT"      { yylval.node = newtoken("reserved", yytext, row, col); col += strlen(yytext); return EXIT; }
"RETURN"    { yylval.node = newtoken("reserved", yytext, row, col); col += strlen(yytext); return RETURN; }
":="        { yylval.node = newtoken("operator", yytext, row, col); col += strlen(yytext); return ASSIGN; }
"="         { yylval.node = newtoken("operator", yytext, row, col); col += strlen(yytext); return EQ; }
"+"         { yylval.node = newtoken("operator", yytext, row, col); col += strlen(yytext); return ADD; }
"-"         { yylval.node = newtoken("operator", yytext, row, col); col += strlen(yytext); return SUB; }
"*"         { yylval.node = newtoken("operator", yytext, row, col); col += strlen(yytext); return MUL; }
"/"|"DIV"   { yylval.node = newtoken("operator", yytext, row, col); col += strlen(yytext); return DIV; }
"<"         { yylval.node = newtoken("operator", yytext, row, col); col += strlen(yytext); return LT; }
">"         { yylval.node = newtoken("operator", yytext, row, col); col += strlen(yytext); return GT; }
"<="        { yylval.node = newtoken("operator", yytext, row, col); col += strlen(yytext); return LE; }
">="        { yylval.node = newtoken("operator", yytext, row, col); col += strlen(yytext); return GE; }
"<>"        { yylval.node = newtoken("operator", yytext, row, col); col += strlen(yytext); return NE; }
"("         { yylval.node = newtoken("delimiter", yytext, row, col); col += strlen(yytext); return LPAREN; }
")"         { yylval.node = newtoken("delimiter", yytext, row, col); col += strlen(yytext); return RPAREN; }
";"         { yylval.node = newtoken("delimiter", yytext, row, col); col += strlen(yytext); return SEMI; }
"{"         { yylval.node = newtoken("delimiter", yytext, row, col); col += strlen(yytext); return LP; }
"}"         { yylval.node = newtoken("delimiter", yytext, row, col); col += strlen(yytext); return RP; }
","         { yylval.node = newtoken("delimiter", yytext, row, col); col += strlen(yytext); return COM; }
"[<"        { yylval.node = newtoken("delimiter", yytext, row, col); col += strlen(yytext); return LARRAY; }
">]"        { yylval.node = newtoken("delimiter", yytext, row, col); col += strlen(yytext); return RARRAY; }
"["         { yylval.node = newtoken("delimiter", yytext, row, col); col += strlen(yytext); return LS; }
"]"         { yylval.node = newtoken("delimiter", yytext, row, col); col += strlen(yytext); return RS; }
":"         { yylval.node = newtoken("delimiter", yytext, row, col); col += strlen(yytext); return COLON; }
"."         { yylval.node = newtoken("delimiter", yytext, row, col); col += strlen(yytext); return DOT; }
{ID}        { 
                if(strlen(yytext) <= 255) { yylval.node = newtoken("identifier", yytext, row, col); col += strlen(yytext); return ID; }
                else { myerror(0, "an overly long identifier", row, col); col += strlen(yytext); }
            }
{bell}      { myerror(0, "a bad character (bell)", row, col); col += strlen(yytext); }
%%
