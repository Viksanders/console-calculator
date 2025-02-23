#ifndef INTERPRETER_H_INCLUDED
#define INTERPRETER_H_INCLUDED
#include "main.h"
//lexer constants
extern const unsigned int LX_ADD;          // +
extern const unsigned int LX_SUB;          // -
extern const unsigned int LX_MUL;          // *
extern const unsigned int LX_DIV;          // /
extern const unsigned int LX_POW;          // ^
extern const unsigned int LX_LBR;          // (
extern const unsigned int LX_RBR;          // )
extern const unsigned int LX_NUM;          // number as double: 123.456
extern const unsigned int LX_ID;          // variable
extern const unsigned int LX_SIN;         // sin()
extern const unsigned int LX_COS;         // cos()
extern const unsigned int LX_LN;         // ln
extern const unsigned int LX_ER;         // undefined, error


//stack for calculator
typedef struct stack_double
{
    double value;
    struct stack_double* next;
} stack_double;


void LX_const_printer(const unsigned int lx_cnst);
int get_priority(unsigned int lx_type);
int is_operator(unsigned int lx_type);
int is_function(unsigned int lx_type);
void lx_list_append(lx_list** lexemes, unsigned int lx_type, double lx_fval, char lx_cval[]);
void lx_list_push(lx_list** lexemes, lx_list** new_node);
lx_list* lx_list_pop(lx_list** lexemes);
unsigned int lx_list_top_type(lx_list** lexemes);
void lx_list_add(lx_list** lexemes, lx_list** new_node);
lx_list* lx_list_pick(lx_list** lexemes);
void lx_list_free(lx_list** lexemes);
void print_lx_list_full(lx_list* lexemes);
void print_lx_list_short(lx_list* lexemes);
unsigned int remove_spaces(char* str);
int is_match(char check, char example);
int is_letter(char ch);
int is_digit(char ch);
int is_str_equal(char* str_1, char* str_2);
unsigned int match_known_word(char* str, known_var* k_var);
double pow_dbl(double number, unsigned int pov);
double get_num_from_str(char* str, unsigned int* position, int* error);
double get_num_from_str_signed(char* str, unsigned int* position, int* error);
lx_list* lx_list_creator_from_input_string(char* str, unsigned int* is_equation);
void simple_rule_checker(lx_list** lexer_list);
lx_list* translate_to_postfix(lx_list** lexer_output);
void stack_double_push(stack_double** calculator, double set_value);
double stack_double_pop(stack_double** calculator);
stack_double* stack_double_top(stack_double** calculator);
void stack_double_free(stack_double** calculator);
double postfix_calculator(lx_list** postfix_notation, double ID_value_set);
double postfix_calculator_multiple(lx_list** postfix_notation, double ID_value_set);
double calculate_string_value(char* input_string, double ID_value);
postfix_tokenised get_postfix_tokens_from_str(char* input_string);

#endif // INTERPRETER_H_INCLUDED
