#include <iostream>
#include <cstdio>
#include <cstring>
#include "lexer.h"
using namespace std;

int yylex();
extern "C" FILE *yyin;
extern "C" char *yytext;

void outputint(int num){
    int size=0;
    cout<<num;
    while(num!=0){
        size++;
        num/=10;
    }
    for(int i=0;i<5-size;i++){
        cout<<" ";
    }
}

int main(int argc, char **argv)
{
    int count=0, row =1, col=1;
    yyin = fopen("/home/zhouxunzhe/COMPILE/project1/tests/case_11.pcat", "r");
    if (!yyin) {
        cout<<"Cannot open file"<<endl;
        yyin = stdin;
    }
    cout<<"ROW  COL  TYPE               TOKEN/ERROR MESSAGE"<<endl;
    while (true){
        int n = yylex();
        if (n == T_EOF){
            break;
        }
        if (n == BELL){
            outputint(row);
            outputint(col);
            cout<<"                   "<<"a bad character (bell)"<<endl;
            col+=strlen(yytext);
        }
        if (n == INVALIDSTR){
            outputint(row);
            outputint(col);
            cout<<"                   "<<"an unterminated string"<<endl;
            col+=strlen(yytext);
            row++;
        }
        if (n == INVALIDCOM){
            int len=strlen(yytext);
            outputint(row);
            outputint(col);
            cout<<"                   "<<"an unterminated comment"<<endl;
            for(int i=0;i<len;i++){
                if(yytext[i]=='\n')row++;
            }
        }
        if (n == STRING){
            int len=strlen(yytext),tab=0,enter=0;
            for(int i=0;i<len;i++){
                if(yytext[i]=='\t')tab++;
                if(yytext[i]=='\n'){
                    enter++;
                    row++;
                }
            }
            if(enter>0){
                outputint(row);
                outputint(col);
                cout<<"                   "<<"an invalid string with enter in it"<<endl;
                col+=len;
            }
            else if(tab==1){
                outputint(row);
                outputint(col);
                cout<<"                   "<<"an invalid string with tab in it"<<endl;
                col+=len;
            }
            else if(tab>1){
                outputint(row);
                outputint(col);
                cout<<"                   "<<"an invalid string with many tabs in it"<<endl;
                col+=len;
            }
            else if(len>257){
                outputint(row);
                outputint(col);
                cout<<"                   "<<"an overly long string"<<endl;
                col+=len;
            }
            else{
                outputint(row);
                outputint(col);
                cout<<"string             ";
                col+=len;
                count++;
                cout << yytext << endl;
            }
        }
        if (n == KEYWORD){
            outputint(row);
            outputint(col);
            cout<<"reserved keyword   ";
            col+=strlen(yytext);
            count++;
            cout << yytext << endl;
        }
        if (n == REAL){
            outputint(row);
            outputint(col);
            cout<<"real               ";
            col+=strlen(yytext);
            count++;
            cout << yytext << endl;
        }
        if (n == INTEGER){
            int val = atoi(yytext);
            if(val<0){
                outputint(row);
                outputint(col);
                cout<<"                   "<<"an out of range integer"<<endl;
                col+=strlen(yytext);
            }
            else{
                outputint(row);
                outputint(col);
                cout<<"integer            ";
                col+=strlen(yytext);
                count++;
                cout << yytext << endl;
            }
        }
        if (n == OPERATOR){
            outputint(row);
            outputint(col);
            cout<<"operator           ";
            col+=strlen(yytext);
            count++;
            cout << yytext << endl;
        }
        if (n == DELIMITER){
            outputint(row);
            outputint(col);
            cout<<"delimiter          ";
            col+=strlen(yytext);
            count++;
            cout << yytext << endl;
        }
        if (n == IDENTIFIER){
            int len=strlen(yytext);
            if(len>255){
                outputint(row);
                outputint(col);
                cout<<"                   "<<"an overly long identifier"<<endl;
                col+=len;
            }
            else{
                outputint(row);
                outputint(col);
                cout<<"identifier         ";
                col+=len;
                count++;
                cout << yytext << endl;
            }
        }
        if (n == COMMENTS){
            int len=strlen(yytext);
            for(int i=0;i<len;i++){
                if(yytext[i]=='\n'){
                    row++;
                    col=1;
                }
                else
                    col++;
            }
        }
        if (n == BLANK){
            col++;
        }
        if (n == ENTER){
            row++;
            col=1;
        }
    }
    cout<<"number of tokens is: "<<count<<endl;
    
    return 0;
}
