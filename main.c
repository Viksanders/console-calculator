#include "main.h"
#include "interpreter.h"
#include "solver.h"

void get_input_str(char* str_input)
{
    char ch;
    unsigned int i = 0;

    printf("ENTER EXPR: ");
    while ((ch = getchar()) != '\n')
    {
        str_input[i] = ch;
        ++i;
    }
    str_input[i] = '\0';
}

int main(int argc, char *argv[])
{
    double base_epsilon = 0.01;
    double tol = 0.000001;
    double left = -10.0;
    double right = 10.0;
    double result;

    unsigned int pos = 0;
    int error = 0;
    double* argvstruct[4];
    argvstruct[0] = &base_epsilon;
    argvstruct[1] = &tol;
    argvstruct[2] = &left;
    argvstruct[3] = &right;
    unsigned int robust = 0;

    if (argc == 1)
    {
        base_epsilon = 0.01;
        tol = 0.000001;
        left = -100.0;
        right = 100.0;
        robust  = 0;
        printf("programm started with base_eps=%f, tol=%f, left=%f, right=%f\n", base_epsilon, tol, left, right);
        printf("Fast solver selected. May be some root losses.\n");
    }
    else if (argc == 6)
    {
        for (unsigned int i=1; i < argc - 1; ++i)
        {
            pos = 0;
            *(argvstruct[i - 1]) = get_num_from_str_signed(argv[i], &pos, &error);
            if (error == 1)
            {
                printf("\nError in parameters\n");
                exit(-301);
            }
        }
        if ((base_epsilon <= tol) || (left >= right) || (base_epsilon <= 0.0) || (tol <= 0.0))
        {
            printf("programm can NOT start with base_eps=%f, tol=%f, left=%f, right=%f\n", base_epsilon, tol, left, right);
            exit(-303);
        }
        if (is_str_equal(argv[5],"r")) robust = 1;
        else if (is_str_equal(argv[5],"f")) robust = 0;
        else
        {
            printf("solver method not selected\n");
            exit(-304);
        }
        printf("programm started with base_eps=%f, tol=%f, left=%f, right=%f\n", base_epsilon, tol, left, right);
        if (robust) printf("Robust solver selected. Multiple same roots possible.\n");
        else printf("Fast solver selected. May be some root losses.\n");
    }
    else
    {
        printf("\ninput parameters mismatch: waiting 5 or 0 parameters: root_separation_tolerance epsilon left right method\n");
        exit(-302);
    }



    char str_input[MAX_LENGTH];

    while(1)
    {
        get_input_str(str_input);

        if (is_str_equal(str_input, "/exit")) exit(0);
        if (is_str_equal(str_input, "")) continue;

        postfix_tokenised to_clc = get_postfix_tokens_from_str(str_input);

        //print_lx_list_short(to_clc.postfix_notation);

        if (to_clc.is_equation)
        {
            solve_equation(&to_clc, left, right, base_epsilon, tol, robust);
        }
        else
        {
            result = postfix_calculator(&(to_clc.postfix_notation), 0.0);
            printf("\nanswer: %f\n", result);
        }

        lx_list_free(&(to_clc.postfix_notation));
    }

    return 0;
}

