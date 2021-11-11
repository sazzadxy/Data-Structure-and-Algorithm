#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define SIZE 100

typedef struct{
    char item[SIZE];
    int top;
}OperatorStack;

typedef struct {
    double item[SIZE];
    int top;
}OperandStack;


void opr_push(OperatorStack *, char);
char opr_pop(OperatorStack *);
int isEmpty(OperatorStack *);
int prec(char left, char right);
void convert(char infix[], char postfix[]);
//void reverseString(char str[], char reverse[]);
void reverseStr(char str[]);

void push2(OperandStack *, double);
double pop2(OperandStack *);
int isEmpty2(OperandStack *);
double operate(double leftOpnd, double rightOpnd, char opr);
double evalPrefix(char preFix[]);

int isEmpty(OperatorStack *s)
{
    return s->top == -1;
}

void push(OperatorStack *s, char v)
{
    if(s->top == SIZE - 1){
        printf("Stack overflow, terminating program\n");
        exit(1);
    }
    s->item[++s->top] = v;
}

char pop(OperatorStack *s)
{
    if(isEmpty(s)){
        printf("Stack underflow, terminating program\n");
        exit(1);
    }
    return s->item[s->top--];
}

/*
    prcd(op, '(') = FALSE
    prcd('(', op) = FALSE
    prcd(op, ')') = TRUE, except op = '('
    prcd('(',')') = FALSE, instead of push will pop
*/
int prcd(char left, char right)
{
    if(left == ')' || right == ')')
        return 0;
    if(right == '(')
        return 1;

    if(left == '*' || left == '/'){
        if(right == '*' || right == '/' || right == '+' || right == '-')
            return 1;
        else
            return 0;
    }

    if(left == '+' || left == '-'){
        if(right == '+' || right == '-')
            return 1;
        else
            return 0;
    }

    if(left == '$')
        return 1;
    else
        return 0;
}

void convert(char infix[], char prefix[])
{
    OperatorStack stack;
    stack.top = -1;
    int i, j = 0;

    while(infix[i] != '\0'){
        char token = infix[i];
            // if token is a operand
            if(token >= '0' && token <= '9'){
                prefix[j++] = token;
            }
            // if token is a operator
            else if(token == '+' || token == '-' || token == '*' || token == '/' || token == '$' || token == '(' || token == ')'){
                while(stack.top != -1 && prcd(stack.item[stack.top], token)){
                    char top_opr = pop(&stack);
                    prefix[j++] = top_opr;
                }
                if(token == '(')
                    pop(&stack);
                else
                    push(&stack,token);
            }
            else{
                printf("invalid symbol %c encountered \n", token);
                exit(1);
            }
        i++;
    } // end of the outer while

    while(stack.top != -1){
        char top_opr = pop(&stack);
        prefix[j++] = top_opr;
    }
    prefix[j] = '\0';
}

void reverseStr(char str[])
{
    char reverse[strlen(str)+1];
    for(int i = strlen(str)-1, j = 0; i >= 0; i--, j++)
        reverse[j] = str[i];
    reverse[strlen(str)] = '\0';

    for(int i = 0; i < strlen(reverse); i++)
        str[i] = reverse[i];
}

/*void reverseString(char str[], char reverse[])
{
//    int j = 0;
//    for(int i = strlen(str)-1; i >= 0; i--)
//        reverse[j++] = str[i];


//    for(int i = strlen(str)-1, j = 0; i >= 0 ; i--, j++)
//        reverse[j] = str[i];
//    reverse[strlen(str)] = '\0';
} */


void push2(OperandStack *s, double value)
{
    if(s->top == SIZE - 1){
        printf("Stack overflow, terminating program\n");
        exit(1);
    }
    s->item[++s->top] = value;
}

double pop2(OperandStack *s)
{
    if(isEmpty2(s)){
        printf("Stack underflow, terminating program\n");
        exit(1);
    }
    return s->item[s->top--];
}

int isEmpty2(OperandStack *s)
{
    return s->top == -1;
}

double operate(double leftOpnd, double rightOpnd, char opr)
{
    double result = 0.0;

    switch(opr){
        case '+': result = rightOpnd + leftOpnd;
                  break;
        case '-': result = rightOpnd - leftOpnd;
                  break;
        case '*': result = rightOpnd * leftOpnd;
                  break;
        case '/': result = rightOpnd / leftOpnd ;
                  break;
        case '$': result = pow(rightOpnd,leftOpnd);
                  break;
        default: printf("%c is not a valid operator\n", opr);
                 exit(1);
    }
    return result;
}

double evalPrefix(char preFix[])
{
    OperandStack stack;
    stack.top = -1;
    int i = strlen(preFix) - 1;
    while(preFix[i] != 0){
        char token = preFix[i];

        if(token >= '0' && token <= '9'){
            int v = token - '0';
            push2(&stack, (double)v);
        }
        else if(token == '+' || token == '-' || token == '*' || token == '/' || token == '$'){
            double op1 = pop2(&stack);
            double op2 = pop2(&stack);
            double result = operate(op2,op1,token);
            push2(&stack,result);
        }
        else{
            printf("Invalid symbol encountered\n");
            exit(1);
        }
        --i;
    }
    return pop2(&stack);
}

int main()
{
    char infix[SIZE];
    char prefix[SIZE];

    printf("Input infix: ");
    scanf("%s",infix);
    reverseStr(infix);
    //strrev(infix);
    convert(infix,prefix);
    reverseStr(prefix);
    //strrev(prefix);

    double result = evalPrefix(prefix);

    printf("The equivalent prefix %s\n", prefix);
    printf("The value of the expression is %lf\n",result);

    return 0;
}
