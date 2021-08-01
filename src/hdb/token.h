#define __token_h

#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "DStack.h"


char * CreateToken(char * t, int tokennumber);
DStack * CreateTokensEx(char * t, int * pnTokens, int * pRetcd);
char isDBspecialchar(char c);
char isDBnormalchar(char c);
char isDBwhitespacechar(char c);
int TokenTest();



