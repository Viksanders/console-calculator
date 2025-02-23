#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_LX_LENGTH 33                //maximum lexem length
#define MAX_LENGTH 255

typedef struct known_var
{
    int id_set;
    char buffer[MAX_LENGTH];
} known_var;

typedef struct lx_t
{
    unsigned int type;          //LX_ADD, LX_SUB, etc
    double fval;                //lexem value as double (if it is LX_NUM)
    char cval[MAX_LX_LENGTH];   //lexem value as a string
} lx_t;

typedef struct lx_list
{
    lx_t lexem;
    struct lx_list* next;
} lx_list;

typedef struct postfix_tokenised
{
    lx_list* postfix_notation;
    unsigned int is_equation;
} postfix_tokenised;


#endif // MAIN_H_INCLUDED
