#include <stdio.h>

#define MAX 10

typedef struct
{
    int id;
    char grade;
} Student;

union Data
{
    int i;
    float f;
    char c;
};

enum Day
{
    MON, TUE, WED, THU, FRI
};

const int value = 100;
volatile int flag = 0;

extern int global;

int main(void)
{
    auto int a = 10;
    static int count = 0;
    register int i;
    short s = 5;
    long l = 100000;
    signed int x = -20;
    unsigned int y = 20;

    float f = 3.14;
    double d = 12.3456;

    Student st;
    union Data data;

    data.i = 50;

    st.id = 1;
    st.grade = 'A';

    // print size of student
    printf("Size of Student = %lu\n", sizeof(Student));

    /* Check for positive
       check for negative */

    if (a > 0)
    {
        printf("Positive\n");
    }
    else
    {
        printf("Negative\n");
    }

    switch (st.grade)
    {
        case 'A':
            printf("Excellent\n");
            break;

        case 'B':
            printf("Good\n");
            break;

        default:
            printf("Average\n");
    }

    for (i = 0; i < 5; i++)
    {
        if (i == 3)
            continue;

        printf("%d ", i);
    }

    printf("\n");

    while (a > 0)
    {
        a--;

        if (a == 5)
            goto END;
    }

    do
    {
        count++;
    } while (count < 2);

END:
    printf("End of Program\n");

    return 0;
}