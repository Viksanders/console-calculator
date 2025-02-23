#include "main.h"
#include "solver.h"
#include "interpreter.h"

void init_queue(queue_s* q)
{
    q->head = NULL;
}

void add_to_ab_queue(queue_s* q, double a, double b)
{
    if (q->head == NULL)
    {
        node_queue_s* new_node = (node_queue_s*)malloc(sizeof(node_queue_s));
        new_node->data.a = a;
        new_node->data.b = b;
        new_node->next_node = NULL;
        q->head = new_node;
        return;
    }

    node_queue_s* loop_ptr = q->head;
    while(loop_ptr->next_node != NULL)
    {
        loop_ptr = loop_ptr->next_node;
    }

    node_queue_s* new_node = (node_queue_s*)malloc(sizeof(node_queue_s));
    new_node->data.a = a;
    new_node->data.b = b;
    new_node->next_node = NULL;
    loop_ptr->next_node = new_node;
}

interval_s take_from_ab_queue(queue_s* q)
{
    interval_s wrong;
    wrong.a = -1.0;
    wrong.b = -1.0;
    if (q->head == NULL) return wrong;
    interval_s value_to_return;
    value_to_return.a = q->head->data.a;
    value_to_return.b = q->head->data.b;
    node_queue_s* it_was_head = q->head;
    q->head = q->head->next_node;
    free(it_was_head);
    return value_to_return;
}

void free_queue_s(queue_s* q)
{
    if (q == NULL) return;
    if (q->head == NULL) return;

    node_queue_s* loop_ptr = q->head;
    node_queue_s* u_re_next = NULL;
    while(loop_ptr != NULL)
    {
        u_re_next = loop_ptr->next_node;
        free(loop_ptr);
        loop_ptr = u_re_next;
    }
}

void print_queue(queue_s* q)
{
    if (q->head == NULL) printf("Queue is empty\n");

    node_queue_s* loop_ptr = q->head;
    while(loop_ptr != NULL)
    {
        printf("a = %f; b = %f; \n", loop_ptr->data.a, loop_ptr->data.b);
        loop_ptr = loop_ptr->next_node;
    }
}

int is_queue_empty(queue_s* q)
{
    if (q->head == NULL)
    {
        return 1;
    }
    else return 0;
}

void separate_roots(queue_s* q, postfix_tokenised* postfixed, double a, double b, double h, double eps, unsigned int robust)
{
    double a_check = postfix_calculator_multiple(&(postfixed->postfix_notation), a);
    double b_check = postfix_calculator_multiple(&(postfixed->postfix_notation), b);
    printf("Check [a=%f, b=%f] interval: f(%f)=%f, f(%f)=%f\n", a, b, a, a_check, b, b_check);
    double x1 = a;
    double x2 = x1 + h;
    double y1 = postfix_calculator_multiple(&(postfixed->postfix_notation), x1);
    double y2;
    unsigned int flg = 0;

    printf("\nRoot separation with %f tolerance\n", h);
    while(x2 < b)
    {
        y2 = postfix_calculator_multiple(&(postfixed->postfix_notation), x2);

        if ( (y1*y2 <= 0.0) || ( (robust) && (y1*y2 <= eps) && (flg == 0) ) )
        {
            flg = 1;
            add_to_ab_queue(q, x1, x2);
            printf("FOUND: a=%f; b=%f;\n", x1, x2);
        }
        else if (y1*y2 <= eps) flg = 0;
        x1 = x2;
        x2 = x1 + h;
        y1 = y2;
        //printf("x1 = %f,  x2 = %f,  y1 = %f\n", x1, x2, y1);
    }
}

double hordy(interval_s* interval, postfix_tokenised* postfixed, double epsilon, unsigned int robust)
{
    double a = interval->a;
    double b = interval->b;

    if ((!(postfix_calculator_multiple(&(postfixed->postfix_notation), a)*postfix_calculator_multiple(&(postfixed->postfix_notation), b) <= 0)) && (robust))
    {
        if ((a > -epsilon) && (a < epsilon)) a = (a > 0.0) ? a + epsilon : a - epsilon;
        if ((b > -epsilon) && (b < epsilon)) b = (b > 0.0) ? b + epsilon : b - epsilon;
    }

    double xn_pre = a - postfix_calculator_multiple(&(postfixed->postfix_notation), a)*(b - a)/(postfix_calculator_multiple(&(postfixed->postfix_notation), b) - postfix_calculator_multiple(&(postfixed->postfix_notation), a));
    double low_than_zero_check = postfix_calculator_multiple(&(postfixed->postfix_notation), a)*postfix_calculator_multiple(&(postfixed->postfix_notation), xn_pre);
    double xn;

    unsigned int counter = 0;
    unsigned int counter_max =  (unsigned int)((double)1.0/epsilon)*10;

    while(1)
    {
        if (counter >= counter_max)
        {
            printf("You've entered expr like: x=x, sin(x)^2 + cos(x)^2 = 1, sin(x) = x or similarilly-senseless. Goodbye.\n");
            exit(-200);
        }

        if (low_than_zero_check <= (double)0.0)
        {
            b = xn_pre;
        }
        else
        {
            a = xn_pre;
        }
        xn = a - postfix_calculator_multiple(&(postfixed->postfix_notation), a)*(b - a)/(postfix_calculator_multiple(&(postfixed->postfix_notation), b) - postfix_calculator_multiple(&(postfixed->postfix_notation), a));
        //printf("%f\n", xn);
        if ((xn-xn_pre <= epsilon) && (xn-xn_pre >= -epsilon))
        //if (calculate_string_value(str_fcn, xn) <= epsilon)
        {
            return xn;
        }
        xn_pre = xn;
        low_than_zero_check = postfix_calculator_multiple(&(postfixed->postfix_notation), a)*postfix_calculator_multiple(&(postfixed->postfix_notation), xn_pre);
        ++counter;
    }
}

void solve_equation(postfix_tokenised* postfixed, double start_interval, double stop_interval, double base_epsilon, double epsilon, unsigned int robust)
{
    queue_s q;
    init_queue(&q);
    separate_roots(&q, postfixed, start_interval, stop_interval, base_epsilon, epsilon, robust);

    interval_s interval;
    double root;
    printf("\nRoot finding with %f tolerance\n", epsilon);
    while(!is_queue_empty(&q))
    {
        interval = take_from_ab_queue(&q);
        root = hordy(&interval, postfixed, epsilon, robust);
        //if (((root - root_pre) > epsilon) && ((root - root_pre) < -epsilon))
        printf("Found root=%f with eps=%f, f(x)=%f at interval a=%f, b=%f\n", root, epsilon, postfix_calculator_multiple(&(postfixed->postfix_notation), root), interval.a, interval.b);
    }
}
