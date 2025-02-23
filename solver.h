#ifndef SOLVER_H_INCLUDED
#define SOLVER_H_INCLUDED
#include "main.h"

typedef struct interval_s
{
    double a;
    double b;
} interval_s;

typedef struct node_queue_s
{
    interval_s data;
    struct node_queue_s* next_node;
} node_queue_s;

typedef struct queue_s
{
    node_queue_s* head;
} queue_s;

void init_queue(queue_s* q);
void add_to_ab_queue(queue_s* q, double a, double b);
interval_s take_from_ab_queue(queue_s* q);
void free_queue_s(queue_s* q);
void print_queue(queue_s* q);
int is_queue_empty(queue_s* q);
void separate_roots(queue_s* q, postfix_tokenised* postfixed, double a, double b, double h, double eps, unsigned int robust);
double hordy(interval_s* interval, postfix_tokenised* postfixed, double epsilon, unsigned int robust);
void solve_equation(postfix_tokenised* postfixed, double start_interval, double stop_interval, double base_epsilon, double epsilon, unsigned int robust);


#endif // SOLVER_H_INCLUDED
