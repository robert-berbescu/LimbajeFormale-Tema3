%{
#include <stdio.h>
#include <stdlib.h>
%}

%%

E:    E '+' T
    | T
;

T:    T '*' F
    | F
;

F:     '(' E ')' 
    |  'a' 
    |  '-'  '(' E ')' 
;


%%

int yyerror(char *s) {
    fprintf(stderr, "%s\n", s);
    return 1;
}

int main() {
    if (yyparse())
        fprintf(stderr, "Successful parsing.\n");
    else
        fprintf(stderr, "Error found.\n");

    return 0;
}