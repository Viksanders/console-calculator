#include "main.h"
#include "interpreter.h"

//lexer constants
const unsigned int LX_ADD = 1;          // +
const unsigned int LX_SUB = 2;          // -
const unsigned int LX_MUL = 3;          // *
const unsigned int LX_DIV = 4;          // /
const unsigned int LX_POW = 5;          // ^
const unsigned int LX_LBR = 6;          // (
const unsigned int LX_RBR = 7;          // )
const unsigned int LX_NUM = 8;          // number as double: 123.456
const unsigned int LX_ID  = 9;          // variable
const unsigned int LX_SIN = 10;         // sin()
const unsigned int LX_COS = 11;         // cos()
const unsigned int LX_LN  = 12;         // ln
const unsigned int LX_ER  = 13;         // undefined, error

void LX_const_printer(const unsigned int lx_cnst)
{
    if (lx_cnst == LX_ADD)          printf("LX_ADD");
    else if (lx_cnst == LX_SUB)     printf("LX_SUB");
    else if (lx_cnst == LX_MUL)     printf("LX_MUL");
    else if (lx_cnst == LX_DIV)     printf("LX_DIV");
    else if (lx_cnst == LX_POW)     printf("LX_POW");
    else if (lx_cnst == LX_LBR)     printf("LX_LBR");
    else if (lx_cnst == LX_RBR)     printf("LX_RBR");
    else if (lx_cnst == LX_NUM)     printf("LX_NUM");
    else if (lx_cnst == LX_ID)      printf("LX_ID");
    else if (lx_cnst == LX_SIN)     printf("LX_SIN");
    else if (lx_cnst == LX_COS)     printf("LX_COS");
    else if (lx_cnst == LX_LN)      printf("LX_LN");
    else                            printf("UNDEFINED");
}

int get_priority(unsigned int lx_type)
{
    if (lx_type == LX_LBR) return 1;
    else if ((lx_type == LX_ADD) || (lx_type == LX_SUB)) return 2;
    else if ((lx_type == LX_MUL) || (lx_type == LX_DIV)) return 3;
    else if (lx_type == LX_POW) return 4;
    else return 0;
}

int is_operator(unsigned int lx_type)
{
    if ((lx_type == LX_ADD) || (lx_type == LX_SUB) || (lx_type == LX_MUL) || (lx_type == LX_DIV) || (lx_type == LX_POW) || (lx_type == LX_SIN) || (lx_type == LX_COS) || (lx_type == LX_LN)) return 1;
    else return 0;
}

int is_function(unsigned int lx_type)
{
    if ((lx_type == LX_SIN) || (lx_type == LX_COS) || (lx_type == LX_LN)) return 1;
    else return 0;
}

void lx_list_append(lx_list** lexemes, unsigned int lx_type, double lx_fval, char lx_cval[])
{
    if ((*lexemes) == NULL)
    {
        (*lexemes) = (lx_list*)malloc(sizeof(lx_list));
        (*lexemes)->lexem.type = lx_type;
        (*lexemes)->lexem.fval = lx_fval;
        for(unsigned int i = 0; i < MAX_LX_LENGTH; ++i)
        {
            (*lexemes)->lexem.cval[i] = lx_cval[i];
            if (lx_cval[i] == '\0') break;
        }
        (*lexemes)->next = NULL;
    }
    else
    {
        lx_list* loop_ptr = *lexemes;
        while(loop_ptr->next != NULL)
        {
            loop_ptr = loop_ptr->next;
        }
        loop_ptr->next = (lx_list*)malloc(sizeof(lx_list));
        loop_ptr->next->lexem.type = lx_type;
        loop_ptr->next->lexem.fval = lx_fval;
        for(unsigned int i = 0; i < MAX_LX_LENGTH; ++i)
        {
            loop_ptr->next->lexem.cval[i] = lx_cval[i];
            if (lx_cval[i] == '\0') break;
        }
        loop_ptr->next->next = NULL;
    }
}

void lx_list_push(lx_list** lexemes, lx_list** new_node)
{
    if ((*lexemes) == NULL)
    {
        *lexemes = *new_node;
        (*lexemes)->next = NULL;
        return;
    }

    lx_list* old_head = *lexemes;
    *lexemes = *new_node;
    (*new_node)->next = old_head;
}

lx_list* lx_list_pop(lx_list** lexemes)
{
    if ((*lexemes) == NULL) return NULL;

    lx_list* output = *lexemes;
    *lexemes = (*lexemes)->next;
    //output->next = NULL;
    return output;
}


unsigned int lx_list_top_type(lx_list** lexemes)
{
    if (*lexemes == NULL) return LX_ER;
    return (*lexemes)->lexem.type;
}

void lx_list_add(lx_list** lexemes, lx_list** new_node)
{
    if ((*lexemes) == NULL)
    {
        *lexemes = *new_node;
        (*lexemes)->next = NULL;
        return;
    }

    lx_list* loop_ptr = *lexemes;
    while(loop_ptr->next != NULL)
    {
        loop_ptr = loop_ptr->next;
    }
    loop_ptr->next = *new_node;
    loop_ptr->next->next = NULL;
}

lx_list* lx_list_pick(lx_list** lexemes)
{
    if ((*lexemes) == NULL) return NULL;

    lx_list* output;
    lx_list* loop_ptr = *lexemes;
    if (loop_ptr->next == NULL)
    {
        output = loop_ptr;
        (*lexemes) = NULL;
        return output;
    }

    while(loop_ptr->next->next != NULL)
    {
        loop_ptr = loop_ptr->next;
    }
    output = loop_ptr->next;
    loop_ptr->next = NULL;
    return output;
}


void lx_list_free(lx_list** lexemes)
{
    if ((*lexemes) == NULL) return;
    lx_list* loop_ptr = *lexemes;
    lx_list* temp;
    while(loop_ptr != NULL)
    {
        temp = loop_ptr->next;
        free(loop_ptr);
        loop_ptr = temp;
    }
    *lexemes = NULL;
}

void print_lx_list_full(lx_list* lexemes)
{
    lx_list* loop_ptr = lexemes;
    if (loop_ptr == NULL)
    {
        printf("\nLIST IS EMPTY\n");
        return;
    }
    while(loop_ptr != NULL)
    {
        printf("\n");
        LX_const_printer(loop_ptr->lexem.type);
        printf("\n");
        printf("value = %f\n", loop_ptr->lexem.fval);
        printf("as string: "); printf("%s", loop_ptr->lexem.cval);
        loop_ptr = loop_ptr->next;
    }
    printf("\nend of lexemes list reached\n");
}

void print_lx_list_short(lx_list* lexemes)
{
    lx_list* loop_ptr = lexemes;
    while(loop_ptr != NULL)
    {
        printf("< ");
        LX_const_printer(loop_ptr->lexem.type);
        printf(" : %s >\n", loop_ptr->lexem.cval);
        loop_ptr = loop_ptr->next;
    }
}

unsigned int remove_spaces(char* str)
{
    unsigned int i_start = 0;
    unsigned int size_new = 0;

    unsigned int i = 0;
    while(str[i] != '\0')
    {
        if ((str[i] >= 'A') && (str[i] <= 'Z')) str[i] = str[i] + (char)32;
        if(str[i] != ' ')
        {
            if (i != i_start) str[i_start] = str[i];
            ++size_new;
            ++i_start;
        }
        ++i;

        if(i >= MAX_LENGTH)
        {
            str[0] = '\0';
            return 0;
        }
    }

    str[i_start] = '\0';
    return size_new;
}

int is_match(char check, char example)
{
    if (check == example) return 1;
    else return 0;
}

int is_letter(char ch)
{
    if (((int)ch >= 97) && ((int)ch <= 122)) return 1;
    else return 0;
}

int is_digit(char ch)
{
    if (((int)ch >= 48) && ((int)ch <= 57)) return 1;
    else return 0;
}

int is_str_equal(char* str_1, char* str_2)
{
    unsigned int i = 0;
    while(1)
    {
        if (str_1[i] != str_2[i]) return 0;
        if ((str_1[i] == '\0') || (str_2[i] == '\0')) return 1;
        ++i;
    }
    return 1;
}

unsigned int match_known_word(char* str, known_var* k_var)
{
    unsigned int i = 0;
    if (is_str_equal(str, "ln")) return LX_LN;
    else if (is_str_equal(str, "sin")) return LX_SIN;
    else if (is_str_equal(str, "cos")) return LX_COS;
    else if (k_var->id_set == 0)
    {
        k_var->id_set = 1;
        while(str[i] != '\0')
        {
            k_var->buffer[i] = str[i];
            ++i;
        }
        k_var->buffer[i] = '\0';
        return LX_ID;
    }
    else if (is_str_equal(str, k_var->buffer)) return LX_ID;
    else return LX_ER;
}

double pow_dbl(double number, unsigned int pov)
{
    double output = number;
    for(unsigned int i = 1; i < pov; ++i)
    {
        output = output * number;
    }
    return output;
}

double get_num_from_str(char* str, unsigned int* position, int* error)
{
    double output = 0.0;
    unsigned int divider_pow = 1;
    unsigned short flg_point = 0;
    *error = 0;
    if (is_digit(str[*position]) == 1)
    {
        while((is_digit(str[*position]) == 1) || (str[*position] == '.'))
        {
            if (str[*position] == '.')
            {
               if (flg_point == 0)
               {
                   flg_point = 1;
                   *position = *position + 1;
                   continue;
               }
               else
               {
                   *error = 1;
                   return output;
               }
            }
            else if (is_digit(str[*position]) == 1)
            {
                if (flg_point == 0)
                {
                    output = output*(double)10.0 + (double)(str[*position] - 48);
                    *position = *position + 1;
                }
                else
                {
                    output = output + ( (double)(str[*position] - 48) ) / ( pow_dbl((double)10.0, divider_pow) );
                    divider_pow = divider_pow + 1;
                    *position = *position + 1;
                }
            }
            else
            {
                *error = 1;
                return output;
            }
        }
    }
    else
    {
        *error = 1;
        return output;
    }
    return output;
}

double get_num_from_str_signed(char* str, unsigned int* position, int* error)
{
    double output = 0.0;
    unsigned int divider_pow = 1;
    unsigned short flg_point = 0;
    *error = 0;
    unsigned int minus_flg = 0;
    if ((is_digit(str[*position]) == 1) || (str[*position] == '-'))
    {
        if (str[*position] == '-')
        {
            minus_flg = 1;
            *position = *position + 1;
        }
        while(str[*position] != '\0')
        {
            if ((str[*position] != '.') && (is_digit(str[*position]) == 0))
            {
                *error = 1;
                return -0.0;
            }

            if (str[*position] == '.')
            {
               if (flg_point == 0)
               {
                   flg_point = 1;
                   *position = *position + 1;
                   continue;
               }
               else
               {
                   *error = 1;
                   return output;
               }
            }
            else if (is_digit(str[*position]) == 1)
            {
                if (flg_point == 0)
                {
                    output = output*(double)10.0 + (double)(str[*position] - 48);
                    *position = *position + 1;
                }
                else
                {
                    output = output + ( (double)(str[*position] - 48) ) / ( pow_dbl((double)10.0, divider_pow) );
                    divider_pow = divider_pow + 1;
                    *position = *position + 1;
                }
            }
            else
            {
                *error = 1;
                return output;
            }
        }
    }
    else
    {
        *error = 1;
        return output;
    }
    if (minus_flg == 1) output =  ((double)(-1.0))*output;
    return output;
}

lx_list* lx_list_creator_from_input_string(char* str, unsigned int* is_equation)
{
    lx_list* lexer_list = NULL;
    unsigned int str_size = remove_spaces(str);     //remove spaces and make all chars to lower case
    known_var k_var;
    k_var.id_set = 0;
    unsigned int position = 0;
    unsigned int is_right_side = 0;     //set to 1 when '=' symbol is reached
    unsigned int first_right = 0;
    unsigned int first_after_lbr = 0;
    int bracket_count = 0;
    *is_equation = 0;

    while(position < str_size)
    {
        if ( is_match(str[position], '+') )
        {
            //printf("  +  ");
            if ((is_right_side == 1) && (bracket_count == 0))
            {
                first_right = 0;
                if (first_after_lbr != 0) lx_list_append(&lexer_list, LX_ADD, (double)LX_ADD, "+");
                lx_list_append(&lexer_list, LX_NUM, -1.0, "-1.0");
                lx_list_append(&lexer_list, LX_MUL, (double)LX_MUL, "*");
            }
            else
            {
                if ((position != 0) && (first_after_lbr != 0)) lx_list_append(&lexer_list, LX_ADD, (double)LX_ADD, "+");
            }
            first_after_lbr = 1;
        }
        else if ( is_match(str[position], '-') )
        {
            //printf("  -  ");
            //lx_list_append(&lexer_list, LX_SUB, (double)LX_SUB, "-");
            if ((is_right_side == 1) && (bracket_count == 0))
            {
                first_right = 0;
                lx_list_append(&lexer_list, LX_ADD, (double)LX_ADD, "+");
            }
            else
            {
                if (first_after_lbr != 0) lx_list_append(&lexer_list, LX_ADD, (double)LX_ADD, "+");
                lx_list_append(&lexer_list, LX_NUM, -1.0, "-1.0");
                lx_list_append(&lexer_list, LX_MUL, (double)LX_MUL, "*");
            }
            first_after_lbr = 1;
        }
        else if ( is_match(str[position], '*') )
        {
            //printf("  *  ");
            lx_list_append(&lexer_list, LX_MUL, (double)LX_MUL, "*");
            first_after_lbr = 1;
        }
        else if ( is_match(str[position], '/') )
        {
            //printf("  /  ");
            lx_list_append(&lexer_list, LX_DIV, (double)LX_DIV, "/");
            first_after_lbr = 1;
        }
        else if ( is_match(str[position], '^') )
        {
            //printf("  ^  ");
            lx_list_append(&lexer_list, LX_POW, (double)LX_POW, "^");
            first_after_lbr = 1;
        }
        else if ( is_match(str[position], '(') )
        {
            //printf("  (  ");
            lx_list_append(&lexer_list, LX_LBR, (double)LX_LBR, "(");
            first_after_lbr = 0;
            ++bracket_count;
        }
        else if ( is_match(str[position], ')') )
        {
            //printf("  )  ");
            lx_list_append(&lexer_list, LX_RBR, (double)LX_RBR, ")");
            first_after_lbr = 1;
            --bracket_count;
            if (bracket_count < 0) { lx_list_free(&lexer_list); exit(-7); }
        }
        else if ( is_digit(str[position]) )
        {
            char buffer[MAX_LX_LENGTH];
            unsigned int i_buf = 0;
            double number = 0.0;
            unsigned int startpos = 0;  //unused here
            int error;
            while ( is_digit(str[position]) || is_match(str[position], '.') )
            {
                buffer[i_buf] = str[position];
                ++i_buf;
                ++position;
                if (i_buf > MAX_LX_LENGTH) { lx_list_free(&lexer_list); exit(-1); }
            }
            buffer[i_buf] = '\0';
            number = get_num_from_str(buffer, &startpos, &error);
            if (error) { lx_list_free(&lexer_list); exit(-2); }
            //printf("%f", number);
            if (first_right == 0) lx_list_append(&lexer_list, LX_NUM, number, buffer);
            else
            {
                first_right = 0;
                lx_list_append(&lexer_list, LX_ADD, (double)LX_ADD, "+");
                lx_list_append(&lexer_list, LX_NUM, -1.0, "-1.0");
                lx_list_append(&lexer_list, LX_MUL, (double)LX_MUL, "*");
                lx_list_append(&lexer_list, LX_NUM, number, buffer);
            }
            first_after_lbr = 1;
            continue;
        }
        else if ( is_letter(str[position]) || is_match(str[position], '_') )
        {
            char buffer[MAX_LX_LENGTH];
            unsigned int i_buf = 0;
            unsigned int lx_current;
            while ( is_letter(str[position]) || is_match(str[position], '_') )
            {
                buffer[i_buf] = str[position];
                ++i_buf;
                ++position;
                if (i_buf > MAX_LX_LENGTH) { lx_list_free(&lexer_list); exit(-1); }
            }
            buffer[i_buf] = '\0';
            lx_current = match_known_word(buffer, &k_var);
            if (lx_current == LX_ER) { lx_list_free(&lexer_list); exit(-3); }
            if (lx_current == LX_ID) *is_equation = 1;
            //printf(buffer); printf("{%d}", lx_current);
            if (first_right == 0) lx_list_append(&lexer_list, lx_current, (double)lx_current, buffer);
            else
            {
                first_right = 0;
                lx_list_append(&lexer_list, LX_ADD, (double)LX_ADD, "+");
                lx_list_append(&lexer_list, LX_NUM, -1.0, "-1.0");
                lx_list_append(&lexer_list, LX_MUL, (double)LX_MUL, "*");
                lx_list_append(&lexer_list, lx_current, (double)lx_current, buffer);
            }
            first_after_lbr = 1;
            continue;
        }
        else if ( is_match(str[position], '=') )
        {
            if (bracket_count != 0) { lx_list_free(&lexer_list); exit(-8); }
            if ((position == 0) || (position == (str_size - 1))) { lx_list_free(&lexer_list); exit(-5); }
            if (is_right_side == 1) { lx_list_free(&lexer_list); exit(-6); }
            is_right_side = 1;
            first_right = 1;
            first_after_lbr = 1;
        }
        else { lx_list_free(&lexer_list); exit(-4); }

        ++position;
    }

    if (bracket_count != 0) { lx_list_free(&lexer_list); exit(-8); }

    return lexer_list;
}

void simple_rule_checker(lx_list** lexer_list)
{
    lx_list* loop_ptr = *lexer_list;
    if (loop_ptr == NULL) exit(-9);

    unsigned int ctr = 0;

    while(1)
    {
        if (((loop_ptr->lexem.type == LX_SIN) || (loop_ptr->lexem.type == LX_COS) || (loop_ptr->lexem.type == LX_LN)) && ((loop_ptr->next == NULL) || (loop_ptr->next->lexem.type != LX_LBR)))
        {
            lx_list_free(lexer_list);
            exit(-10);
        }

        if ( (loop_ptr->lexem.type == LX_LBR) && ( (loop_ptr->next == NULL) || (loop_ptr->next->lexem.type == LX_RBR)) )
        {
            lx_list_free(lexer_list);
            exit(-11);
        }

        if ( ((loop_ptr->lexem.type == LX_NUM) || (loop_ptr->lexem.type == LX_ID)) && ( (loop_ptr->next != NULL) && !( (loop_ptr->next->lexem.type == LX_RBR) || (loop_ptr->next->lexem.type == LX_ADD) || (loop_ptr->next->lexem.type == LX_SUB) || (loop_ptr->next->lexem.type == LX_MUL) || (loop_ptr->next->lexem.type == LX_DIV) || (loop_ptr->next->lexem.type == LX_POW) ) ) )
        {
            lx_list_free(lexer_list);
            exit(-12);
        }

        if (loop_ptr->next == NULL) return;
        loop_ptr = loop_ptr->next;
        ++ctr;
    }
}

lx_list* translate_to_postfix(lx_list** lexer_output)
{
    lx_list* postfix_notation = NULL;
    lx_list* current = NULL;
    lx_list* stack_operators = NULL;
    lx_list* temp = NULL;

    while(1)
    {
        if ((*lexer_output) == NULL) break;
        current = lx_list_pop(lexer_output);

        if ((current->lexem.type == LX_NUM) || (current->lexem.type == LX_ID))
        {
            lx_list_add(&postfix_notation, &current);
        }
        else if (current->lexem.type == LX_LBR)
        {
            lx_list_push(&stack_operators, &current);
        }
        else if (is_operator(current->lexem.type))
        {
            //while( (stack_operators != NULL) && ( is_function(lx_list_top_type(&stack_operators)) || ( get_priority(lx_list_top_type(&stack_operators)) >= get_priority(current->lexem.type) ) ) )
            while( (stack_operators != NULL) && ( get_priority(lx_list_top_type(&stack_operators)) >= get_priority(current->lexem.type) ) )
            {
                if (is_function(current->lexem.type)) break;
                temp = lx_list_pop(&stack_operators);
                if ((temp != NULL) && (temp->lexem.type != LX_LBR)) lx_list_add(&postfix_notation, &temp);
                else free(temp);
            }
            lx_list_push(&stack_operators, &current);
            //temp = lx_list_pop(&stack_operators);
            //if ((temp != NULL) && (temp->lexem.type != LX_LBR)) lx_list_add(&postfix_notation, &temp);
            //lx_list_push(&stack_operators, &current);
        }
        else if (current->lexem.type == LX_RBR)
        {
            while(1)
            {
                temp = lx_list_pop(&stack_operators);
                if (temp == NULL) break;
                if (temp->lexem.type == LX_LBR)
                {
                    free(temp);
                    break;
                }
                lx_list_add(&postfix_notation, &temp);
            }
            if (is_function(lx_list_top_type(&stack_operators)))
            {
                temp = lx_list_pop(&stack_operators);
                lx_list_add(&postfix_notation, &temp);
            }
            free(current);
        }
        /*
        printf("scanning:  "); LX_const_printer(current->lexem.type); printf("\n");
        print_lx_list_short(postfix_notation);
        printf("\n___STACK:\n");
        print_lx_list_short(stack_operators);
        printf("\n-----------------------\n");
        //*/
    }

    while(1)
    {
        if (stack_operators == NULL) break;
        temp = lx_list_pop(&stack_operators);
        lx_list_add(&postfix_notation, &temp);
    }

    return postfix_notation;
}

void stack_double_push(stack_double** calculator, double set_value)
{
    stack_double* new_head = (stack_double*)malloc(sizeof(stack_double));
    new_head->next = *calculator;
    new_head->value = set_value;
    *calculator = new_head;
}

double stack_double_pop(stack_double** calculator)
{
    if ((*calculator) == NULL) return 0.0;

    double output = (*calculator)->value;
    stack_double* new_head = (*calculator)->next;
    free(*calculator);
    (*calculator) = new_head;
    return output;
}

stack_double* stack_double_top(stack_double** calculator)
{
    if ((*calculator) == NULL) return NULL;
    else return *calculator;
}

void stack_double_free(stack_double** calculator)
{
    if ((*calculator) == NULL) return;

    stack_double* loop_ptr = *calculator;
    stack_double* new_head;
    while(loop_ptr != NULL)
    {
        new_head = loop_ptr->next;
        free(loop_ptr);
        loop_ptr = new_head;
    }
}

#define CHECK_101  {if (stack_double_top(&stack_calculator) == NULL) {lx_list_free(postfix_notation); stack_double_free(&stack_calculator); exit(-101);}}

double postfix_calculator(lx_list** postfix_notation, double ID_value_set)
{
    lx_list* current_list = NULL;
    stack_double* stack_calculator = NULL;
    double first_operand;
    double second_operand;

    while(1)
    {
        if ((*postfix_notation) == NULL) break;

        current_list = lx_list_pop(postfix_notation);
        if (current_list->lexem.type == LX_NUM)
        {
            stack_double_push(&stack_calculator, current_list->lexem.fval);
        }
        else if (current_list->lexem.type == LX_ID)
        {
            stack_double_push(&stack_calculator, ID_value_set);
        }
        else if (current_list->lexem.type == LX_ADD)
        {
            CHECK_101
            second_operand = stack_double_pop(&stack_calculator);
            CHECK_101
            first_operand = stack_double_pop(&stack_calculator);
            stack_double_push(&stack_calculator, first_operand + second_operand);
        }
        else if (current_list->lexem.type == LX_SUB)
        {
            CHECK_101
            second_operand = stack_double_pop(&stack_calculator);
            CHECK_101
            first_operand = stack_double_pop(&stack_calculator);
            stack_double_push(&stack_calculator, first_operand - second_operand);
        }
        else if (current_list->lexem.type == LX_MUL)
        {
            CHECK_101
            second_operand = stack_double_pop(&stack_calculator);
            CHECK_101
            first_operand = stack_double_pop(&stack_calculator);
            stack_double_push(&stack_calculator, first_operand * second_operand);
        }
        else if (current_list->lexem.type == LX_DIV)
        {
            CHECK_101
            second_operand = stack_double_pop(&stack_calculator);
            CHECK_101
            first_operand = stack_double_pop(&stack_calculator);
            stack_double_push(&stack_calculator, first_operand / second_operand);
        }
        else if (current_list->lexem.type == LX_POW)
        {
            CHECK_101
            second_operand = stack_double_pop(&stack_calculator);
            CHECK_101
            first_operand = stack_double_pop(&stack_calculator);
            stack_double_push(&stack_calculator, pow(first_operand, second_operand));
        }
        else if (current_list->lexem.type == LX_SIN)
        {
            CHECK_101
            second_operand = stack_double_pop(&stack_calculator);
            stack_double_push(&stack_calculator, sin(second_operand));
        }
        else if (current_list->lexem.type == LX_COS)
        {
            CHECK_101
            second_operand = stack_double_pop(&stack_calculator);
            stack_double_push(&stack_calculator, cos(second_operand));
        }
        else if (current_list->lexem.type == LX_LN)
        {
            CHECK_101
            second_operand = stack_double_pop(&stack_calculator);
            stack_double_push(&stack_calculator, log(second_operand));
        }
        else
        {
            stack_double_free(&stack_calculator);
            lx_list_free(postfix_notation);
            exit(-102);
        }
        free(current_list);
    }
    if (stack_double_top(&stack_calculator) != NULL) return stack_double_pop(&stack_calculator);
    else
    {
        exit(-103);
        return -999.999;
    }
}

double postfix_calculator_multiple(lx_list** postfix_notation, double ID_value_set)
{
    lx_list* postfix_notation_wrapper = *postfix_notation;
    lx_list* current_list = NULL;
    stack_double* stack_calculator = NULL;
    double first_operand;
    double second_operand;

    while(1)
    {
        if ((postfix_notation_wrapper) == NULL) break;
        //printf("\nHERE\n");
        current_list = lx_list_pop(&postfix_notation_wrapper);
        if (current_list->lexem.type == LX_NUM)
        {
            stack_double_push(&stack_calculator, current_list->lexem.fval);
        }
        else if (current_list->lexem.type == LX_ID)
        {
            stack_double_push(&stack_calculator, ID_value_set);
        }
        else if (current_list->lexem.type == LX_ADD)
        {
            CHECK_101
            second_operand = stack_double_pop(&stack_calculator);
            CHECK_101
            first_operand = stack_double_pop(&stack_calculator);
            stack_double_push(&stack_calculator, first_operand + second_operand);
        }
        else if (current_list->lexem.type == LX_SUB)
        {
            CHECK_101
            second_operand = stack_double_pop(&stack_calculator);
            CHECK_101
            first_operand = stack_double_pop(&stack_calculator);
            stack_double_push(&stack_calculator, first_operand - second_operand);
        }
        else if (current_list->lexem.type == LX_MUL)
        {
            CHECK_101
            second_operand = stack_double_pop(&stack_calculator);
            CHECK_101
            first_operand = stack_double_pop(&stack_calculator);
            stack_double_push(&stack_calculator, first_operand * second_operand);
        }
        else if (current_list->lexem.type == LX_DIV)
        {
            CHECK_101
            second_operand = stack_double_pop(&stack_calculator);
            CHECK_101
            first_operand = stack_double_pop(&stack_calculator);
            stack_double_push(&stack_calculator, first_operand / second_operand);
        }
        else if (current_list->lexem.type == LX_POW)
        {
            CHECK_101
            second_operand = stack_double_pop(&stack_calculator);
            CHECK_101
            first_operand = stack_double_pop(&stack_calculator);
            stack_double_push(&stack_calculator, pow(first_operand, second_operand));
        }
        else if (current_list->lexem.type == LX_SIN)
        {
            CHECK_101
            second_operand = stack_double_pop(&stack_calculator);
            stack_double_push(&stack_calculator, sin(second_operand));
        }
        else if (current_list->lexem.type == LX_COS)
        {
            CHECK_101
            second_operand = stack_double_pop(&stack_calculator);
            stack_double_push(&stack_calculator, cos(second_operand));
        }
        else if (current_list->lexem.type == LX_LN)
        {
            CHECK_101
            second_operand = stack_double_pop(&stack_calculator);
            stack_double_push(&stack_calculator, log(second_operand));
        }
        else
        {
            stack_double_free(&stack_calculator);
            lx_list_free(postfix_notation);
            exit(-102);
        }
        //free(current_list);
    }
    if (stack_double_top(&stack_calculator) != NULL) return stack_double_pop(&stack_calculator);
    else
    {
        exit(-103);
        return -999.999;
    }
    print_lx_list_full(postfix_notation_wrapper);
}

double calculate_string_value(char* input_string, double ID_value)
{
    unsigned int is_equation = 0;
    lx_list* lexer_output = lx_list_creator_from_input_string(input_string, &is_equation);
    simple_rule_checker(&lexer_output);
    lx_list* postfix_notation = translate_to_postfix(&lexer_output);
    return postfix_calculator(&postfix_notation, ID_value);
}

postfix_tokenised get_postfix_tokens_from_str(char* input_string)
{
    postfix_tokenised output;
    unsigned int is_eq = 0;
    lx_list* lexer_output = lx_list_creator_from_input_string(input_string, &is_eq);
    simple_rule_checker(&lexer_output);
    output.postfix_notation = translate_to_postfix(&lexer_output);
    output.is_equation = is_eq;
    lx_list_free(&lexer_output);
    return output;
}
