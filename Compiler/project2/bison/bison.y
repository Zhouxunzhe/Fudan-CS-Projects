%{
#include <iostream>
#include <string>

#include "lexer.c"
#include "ast.h"
using namespace std;

struct Node *root;

void yyerror(const char* msg){
  cerr << msg << endl;
}
%}

%union {
  struct Node *node;
}

%token <node> LT GT EQ NE LE GE
%token <node> ADD SUB 
%token <node> MUL DIV
%token <node> ASSIGN COLON LS RS LP RP LPAREN RPAREN LARRAY RARRAY SEMI DOT COM
%token <node> STRING REAL INTEGER ID
%token <node> LOOP PROGRAM VAR ARRAY END READ WHILE MYBEGIN EXIT RECORD WRITE BY FOR RETURN DO TO IS PROCEDURE TYPE
%token <node> NOT IF THEN ELSIF ELSE LUM OR MOD AND
%token <node> OF

%left         OR;
%left         AND;
%nonassoc     EQ NE;
%nonassoc     LT LE GT GE;
%left         ADD SUB;
%left         MUL LUM DIV MOD;
%right        POS NEG NOT;

/* sm:select one or more */
%type <node> program // test_program
%type <node> body
%type <node> declaration declaration_sm
%type <node> var_decl var_decl_sm
%type <node> id_comma_sm
%type <node> type_decl type_decl_sm
%type <node> procedure_decl procedure_decl_sm
%type <node> type
%type <node> component component_sm
%type <node> formal_params
%type <node> fp_section fp_section_semicolon_sm
%type <node> statement statement_sm
%type <node> elseif_then_sm 
%type <node> write_params
%type <node> write_expr write_expr_comma_sm
%type <node> expression 
%type <node> expression_id_semicolon_assign_sm
%type <node> l_value l_value_comma_sm
%type <node> actual_params
%type <node> expression_comma_sm
%type <node> comp_values
%type <node> array_values array_value array_value_comma_sm
%type <node> number

%%
// program
program: PROGRAM IS body SEMI { $$ = newgram("program", $1->row, $1->col); addnode($$, $3); root = $$; }
  | error { $$ = newgram("", row, col); }
  | error body SEMI{ $$ = newgram("program_error", $2->row, $2->col); addnode($$, $2); root = $$; $$->error = "Error"; }
  ;
body: declaration_sm MYBEGIN statement_sm END { $$ = newgram("body", $1->row, $1->col); addnode($$, $1); addnode($$, $3); }
  ;
// declaration----------------------------------------------
declaration_sm: { $$ = newgram("", row, col); }
  | declaration declaration_sm { $$ = newgram("declaration_sm", $1->row, $1->col); addnode($$, $1); addchild($$, $2); }
  ;
declaration: VAR var_decl_sm { $$  = newgram("declaration", $1->row, $1->col); addnode($$, $2); }
  | TYPE type_decl_sm { $$ = newgram("declaration", $1->row, $1->col); addnode($$, $2);}
  | PROCEDURE procedure_decl_sm { $$ = newgram("declaration", $1->row, $1->col); addnode($$, $2); }
  ;
// val_decl-------------------------------------------------
var_decl_sm: { $$ = newgram("", row, col); }
  | var_decl var_decl_sm { $$ = newgram("var_decl_sm", $1->row, $1->col); addnode($$, $1); addchild($$, $2); }
  ;
var_decl: ID id_comma_sm COLON type ASSIGN expression SEMI { $$ = newgram("var_decl", $1->row, $1->col); addnode($$, $1); addnode($$, $2); addnode($$, $4); addnode($$, $6);}
  | ID id_comma_sm ASSIGN expression SEMI { $$ = newgram("var_decl", $1->row, $1->col); addnode($$, $1); addnode($$, $2); addnode($$, $4);}
  | error SEMI { $$ = newgram("var_decl_error", $2->row, $2->col); $$->error = "Error"; }
  ;
// type_decl------------------------------------------------
type_decl_sm: { $$ = newgram("", row, col); }
  | type_decl type_decl_sm { $$ = newgram("type_decl_sm", $1->row, $1->col); addnode($$, $1); addchild($$, $2); }
  ;  
type_decl: ID IS type SEMI { $$ = newgram("type_decl", $1->row, $1->col); addnode($$, $1); addnode($$, $3); }
  | error SEMI { $$ = newgram("type_decl_error", $2->row, $2->col); $$->error = "Error"; }
  ;
// procedure_decl-------------------------------------------
procedure_decl_sm: { $$ = newgram("", row, col); }
  | procedure_decl procedure_decl_sm { $$ = newgram("procedure_decl_sm", $1->row, $1->col); addnode($$, $1); addchild($$, $2); }
  ;
procedure_decl: ID formal_params COLON type IS body SEMI { $$ = newgram("procedure_decl", $1->row, $1->col); addnode($$, $1); addnode($$, $2); addnode($$, $4); addnode($$, $6); }
  | ID formal_params IS body SEMI { $$ = newgram("procedure_decl", $1->row, $1->col); addnode($$, $1); addnode($$, $2); addnode($$, $4); }
  | ID formal_params COLON type error body SEMI { $$ = newgram("procedure_decl_error", $6->row, $6->col); addnode($$, $1); addnode($$, $2); addnode($$, $4); addnode($$, $6); $$->error = "Error"; }
  | ID formal_params error body SEMI { $$ = newgram("procedure_decl_error", $4->row, $4->col); addnode($$, $1); addnode($$, $2); addnode($$, $4); $$->error = "Error"; }
  ;
// type-----------------------------------------------------
type: ID { $$ = newgram("type", $1->row, $1->col); addnode($$, $1); $$->info = $1->info; }
  | ARRAY OF type { $$ = newgram("type", $1->row, $1->col); addnode($$, $3); }
  | RECORD component component_sm END { $$ = newgram("type", $1->row, $1->col); addnode($$, $2); addnode($$, $3); }
  ;
// component------------------------------------------------
component_sm: { $$ = newgram("", row, col); }
  | component component_sm { $$ = newgram("component_sm", $1->row, $1->col); addnode($$, $1); addchild($$, $2); }
  ;
component: ID COLON type SEMI { $$ = newgram("component", $1->row, $1->col); addnode($$, $1); addnode($$, $3); }
  | error SEMI { $$ = newgram("component_error", $2->row, $2->col); $$->error = "Error"; }
  ;

formal_params: LPAREN fp_section fp_section_semicolon_sm RPAREN  { $$ = newgram("formal_params", $1->row, $1->col); addnode($$, $2); addnode($$, $3); }
  | LPAREN RPAREN { $$ = newgram("formal_params", $1->row, $1->col); }
  ;
// fp_section-----------------------------------------------
fp_section_semicolon_sm: { $$ = newgram("", row, col); }
  | SEMI fp_section fp_section_semicolon_sm { $$ = newgram("fp_section_semicolon_sm", $1->row, $1->col); addnode($$, $2); addchild($$, $3); }
  ;
fp_section: ID id_comma_sm COLON type { $$ = newgram("fp_section", $1->row, $1->col); addnode($$, $1); addnode($$, $2); addnode($$, $4); }
  ;
id_comma_sm: { $$ = newgram("", row, col); }
  | COM ID id_comma_sm { $$ = newgram("id_comma_sm", $1->row, $1->col); addnode($$, $2); addchild($$, $3); }
  ;
// statement------------------------------------------------
statement_sm: { $$ = newgram("", row, col); }
  | statement statement_sm { $$ = newgram("statement_sm", $1->row, $1->col); addnode($$, $1); addchild($$, $2); }
  ;
statement: l_value ASSIGN expression SEMI { $$ = newgram("statement", $1->row, $1->col); addnode($$, $1); addnode($$, $3); }
  | ID actual_params SEMI { $$ = newgram("statement", $1->row, $1->col); addnode($$, $1); addnode($$, $2); }
  | READ LPAREN l_value l_value_comma_sm RPAREN SEMI { $$ = newgram("statement", $1->row, $1->col); addnode($$, $3); addnode($$, $4); }
  | WRITE write_params SEMI { $$ = newgram("statement", $1->row, $1->col); addnode($$, $2); }
  | IF expression THEN statement_sm elseif_then_sm ELSE statement_sm END SEMI { $$ = newgram("statement", $1->row, $1->col); addnode($$, $2); addnode($$, $4); addnode($$, $5); addnode($$, $7); }
  | IF expression THEN statement_sm elseif_then_sm END SEMI { $$ = newgram("statement", $1->row, $1->col); addnode($$, $2); addnode($$, $4); addnode($$, $5); }
  | WHILE expression DO statement_sm END SEMI { $$ = newgram("statement", $1->row, $1->col); addnode($$, $2); addnode($$, $4); }
  | LOOP statement_sm END SEMI { $$ = newgram("statement", $1->row, $1->col); addnode($$, $2); }
  | FOR ID ASSIGN expression TO expression BY expression DO statement_sm END SEMI { $$ = newgram("statement", $1->row, $1->col); addnode($$, $2); addnode($$, $4); addnode($$, $6); addnode($$, $8); addnode($$, $10); }
  | FOR ID ASSIGN expression TO expression DO statement_sm END SEMI { $$ = newgram("statement", $1->row, $1->col); addnode($$, $2); addnode($$, $4); addnode($$, $6); addnode($$, $8); }
  | EXIT SEMI { $$ = newgram("statement", $1->row, $1->col); }
  | RETURN expression SEMI { $$ = newgram("statement", $1->row, $1->col); addnode($$, $2); }
  | RETURN SEMI { $$ = newgram("statement", $1->row, $1->col); }
  | error SEMI { $$ = newgram("statement_error", $2->row, $2->col); $$->error = "Error"; }
  ;
elseif_then_sm: { $$ = newgram("", row, col); }
  | ELSIF expression THEN statement_sm elseif_then_sm { $$ = newgram("elseif_then_sm", $1->row, $1->col); addnode($$, $2); addnode($$, $4); addchild($$, $5); }
  ;
// write---------------------------------------------------
write_params: LPAREN write_expr write_expr_comma_sm RPAREN { $$ = newgram("write_params", $1->row, $1->col); addnode($$, $2); addnode($$, $3); }
  | LPAREN RPAREN { $$ = newgram("write_params", $1->row, $1->col); }
  ;
write_expr: STRING { $$ = newgram("write_expr", $1->row, $1->col); addnode($$, $1); }
  | expression { $$ = newgram("write_expr", $1->row, $1->col); addnode($$, $1); }
  ;
write_expr_comma_sm: { $$ = newgram("", row, col); }
  | COM write_expr write_expr_comma_sm { $$ = newgram("write_expr_comma_sm", $1->row, $1->col); addnode($$, $2); addchild($$, $3); }
  ;
// expression----------------------------------------------
expression: number { $$ = newgram("expression", $1->row, $1->col); addnode($$, $1); $$->info = $1->info; }
  | l_value { $$ = newgram("expression", $1->row, $1->col); addnode($$, $1); $$->info = $1->info; }
  | LPAREN expression RPAREN { $$ = newgram("expression", $1->row, $1->col); addnode($$, $2); $$->info = "("+$2->info+")"; }
  | ADD expression %prec POS { $$ = newgram("expression", $1->row, $1->col); addnode($$, $2); $$->info = "POS"+$2->info; }
  | SUB expression %prec NEG { $$ = newgram("expression", $1->row, $1->col); addnode($$, $2); $$->info = "NEG"+$2->info; }
  | NOT expression { $$ = newgram("expression", $1->row, $1->col); addnode($$, $2); $$->info = "NOT"+$2->info; }
  | expression ADD expression { $$ = newgram("expression", $1->row, $1->col); addnode($$, $1); addnode($$, $3); $$->info = $1->info+"ADD"+$3->info; }
  | expression SUB expression { $$ = newgram("expression", $1->row, $1->col); addnode($$, $1); addnode($$, $3); $$->info = $1->info+"SUB"+$3->info; }
  | expression MUL expression { $$ = newgram("expression", $1->row, $1->col); addnode($$, $1); addnode($$, $3); $$->info = $1->info+"MUL"+$3->info; }
  | expression LUM expression { $$ = newgram("expression", $1->row, $1->col); addnode($$, $1); addnode($$, $3); $$->info = $1->info+"LUM"+$3->info; }
  | expression DIV expression { $$ = newgram("expression", $1->row, $1->col); addnode($$, $1); addnode($$, $3); $$->info = $1->info+"DIV"+$3->info; }
  | expression MOD expression { $$ = newgram("expression", $1->row, $1->col); addnode($$, $1); addnode($$, $3); $$->info = $1->info+"MOD"+$3->info; }
  | expression OR expression { $$ = newgram("expression", $1->row, $1->col); addnode($$, $1); addnode($$, $3); $$->info = $1->info+"OR"+$3->info; }
  | expression AND expression { $$ = newgram("expression", $1->row, $1->col); addnode($$, $1); addnode($$, $3); $$->info = $1->info+"AND"+$3->info; }
  | expression GT expression { $$ = newgram("expression", $1->row, $1->col); addnode($$, $1); addnode($$, $3); $$->info = $1->info+"GT"+$3->info; }
  | expression LT expression { $$ = newgram("expression", $1->row, $1->col); addnode($$, $1); addnode($$, $3); $$->info = $1->info+"LT"+$3->info; }
  | expression EQ expression { $$ = newgram("expression", $1->row, $1->col); addnode($$, $1); addnode($$, $3); $$->info = $1->info+"EQ"+$3->info; }
  | expression GE expression { $$ = newgram("expression", $1->row, $1->col); addnode($$, $1); addnode($$, $3); $$->info = $1->info+"GE"+$3->info; }
  | expression LE expression { $$ = newgram("expression", $1->row, $1->col); addnode($$, $1); addnode($$, $3); $$->info = $1->info+"LE"+$3->info; }
  | expression NE expression { $$ = newgram("expression", $1->row, $1->col); addnode($$, $1); addnode($$, $3); $$->info = $1->info+"NE"+$3->info; }
  | ID actual_params { $$ = newgram("expression", $1->row, $1->col); addnode($$, $1); addnode($$, $2); $$->info = $1->info+$2->info; }
  | ID comp_values { $$ = newgram("expression", $1->row, $1->col); addnode($$, $1); addnode($$, $2); $$->info = $1->info+$2->info; }
  | ID array_values { $$ = newgram("expression", $1->row, $1->col); addnode($$, $1); addnode($$, $2); $$->info = $1->info+$2->info; }
  ;
expression_comma_sm: { $$ = newgram("", row, col); }
  | COM expression expression_comma_sm { $$ = newgram("expression_comma_sm", $1->row, $1->col); addnode($$, $2); addchild($$, $3); }
  ;
expression_id_semicolon_assign_sm: { $$ = newgram("", row, col); }
  | SEMI ID ASSIGN expression expression_id_semicolon_assign_sm { $$ = newgram("expression_id_semicolon_assign_sm", $1->row, $1->col); addnode($$, $2); addnode($$, $4); addchild($$, $5); }
  ;
// l_value--------------------------------------------------
l_value_comma_sm: { $$ = newgram("", row, col); }
  | COM l_value l_value_comma_sm { $$ = newgram("l_value_comma_sm", $1->row, $1->col); addnode($$, $2); addchild($$, $3); }
  ;
l_value: ID { $$ = newgram("l_value", $1->row, $1->col); addnode($$, $1); $$->info = $1->info; }
  | l_value LS expression RS { $$ = newgram("l_value", $1->row, $1->col); addnode($$, $1); addnode($$, $3); $$->info = $1->info+"["+$3->info+"]"; }
  | l_value DOT ID { $$ = newgram("l_value", $1->row, $1->col); addnode($$, $1); addnode($$, $3); $$->info = $1->info+"."+$3->info; }
  ;
actual_params: LPAREN expression expression_comma_sm RPAREN { $$ = newgram("actual_params", $1->row, $1->col); addnode($$, $2); addchild($$, $3); }
  | LPAREN RPAREN { $$ = newgram("actual_params", $1->row, $1->col); }
  ;
// value----------------------------------------------------
comp_values: LP ID ASSIGN expression expression_id_semicolon_assign_sm RP { $$ = newgram("comp_values", $1->row, $1->col); addnode($$, $2); addnode($$, $4); addnode($$, $5); $$->info = "{"+$2->info+":="+$4->info+$5->info+"}"; }
  ;
array_values: LARRAY array_value array_value_comma_sm RARRAY { $$ = newgram("array_values", $1->row, $1->col); addnode($$, $2); addnode($$, $3); }
  ;
array_value: expression OF expression { $$ = newgram("array_value", $1->row, $1->col); addnode($$, $1); addnode($$, $3); }
  | expression { $$ = newgram("array_value", $1->row, $1->col); addnode($$, $1); }
  ;
array_value_comma_sm: { $$ = newgram("", row, col); }
  | COM array_value array_value_comma_sm { $$ = newgram("array_value_comma_sm", $1->row, $1->col); addnode($$, $2); addchild($$, $3); }
  ;
number: INTEGER { $$ = $1; }
  | REAL { $$ = $1; }
  ;
%%
