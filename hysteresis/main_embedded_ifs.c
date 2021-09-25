#include <stdio.h>

#define N (4)              /*Discrete Levels*/
#define HYST (25)          /*Hystereis tolerance for analogue input 2.5*/
#define M (1000 / N)       /*Slope M = 25*/
#define B (1000 / (2 * N)) /*Interception B = 12.5*/

#define HYST_TRIGG_LOW_1 (1*B - HYST)   /*10*/
#define HYST_TRIGG_LOW_2 (3*B - HYST)   /*35*/
#define HYST_TRIGG_LOW_3 (5*B - HYST)   /*60*/
#define HYST_TRIGG_LOW_4 (7*B - HYST)   /*85*/

#define HYST_TRIGG_HIGH_1 (1*B + HYST)   /*15*/
#define HYST_TRIGG_HIGH_2 (3*B + HYST)   /*40*/
#define HYST_TRIGG_HIGH_3 (5*B + HYST)   /*65*/
#define HYST_TRIGG_HIGH_4 (7*B + HYST)   /*90*/

unsigned int getDiscreteLevel(unsigned int prev, unsigned int curr, unsigned int prev_disc_level)
{
    unsigned int discrete_level = 0;
    if (prev < curr)
    {
        discrete_level = ((curr - HYST + B) / M);
    }
    else if (prev > curr)
    {
        discrete_level = ((curr + HYST + B) / M);
    }
    else
    {
        discrete_level = prev_disc_level;
    }
    return discrete_level;
}

unsigned int hysteresis(unsigned int input_percent)
{
    unsigned int discrete_level = 0;
    unsigned int y = input_percent * 10; /*To work in hundreds and preserve the decimal val*/
    static unsigned int prev_y = 0;
    static unsigned int prev_discretelevel = 0;

    /*Check if the value is in one of the hysteresis ranges*/
    if ((y > HYST_TRIGG_LOW_1) && (y < HYST_TRIGG_HIGH_1))
    {
        if ((prev_y > HYST_TRIGG_LOW_1) && (prev_y < HYST_TRIGG_HIGH_1))
        {
            discrete_level = prev_discretelevel;
        }
        else
        {
            discrete_level = getDiscreteLevel(prev_y, y, prev_discretelevel);
        }
    }
    else if ((y > HYST_TRIGG_LOW_2) && (y < HYST_TRIGG_HIGH_2))
    {
        if ((prev_y > HYST_TRIGG_LOW_2) && (prev_y < HYST_TRIGG_HIGH_2))
        {
            discrete_level = prev_discretelevel;
        }
        else
        {
            discrete_level = getDiscreteLevel(prev_y, y, prev_discretelevel);
        }
    }
    else if ((y > HYST_TRIGG_LOW_3) && (y < HYST_TRIGG_HIGH_3))
    {
        if ((prev_y > HYST_TRIGG_LOW_3) && (prev_y < HYST_TRIGG_HIGH_3))
        {
            discrete_level = prev_discretelevel;
        }
        else
        {
            discrete_level = getDiscreteLevel(prev_y, y, prev_discretelevel);
        }
    }
    else if ((y > HYST_TRIGG_LOW_4) && (y < HYST_TRIGG_HIGH_4))
    {
        if ((prev_y > HYST_TRIGG_LOW_4) && (prev_y < HYST_TRIGG_HIGH_4))
        {
            discrete_level = prev_discretelevel;
        }
        else
        {
            discrete_level = getDiscreteLevel(prev_y, y, prev_discretelevel);
        }
    }
    else
    {
        /*Calculate the discrete value from the linear graph*/
        discrete_level = ((y + B) / M);
    }
    /*Save the historical values to compare in a new entry*/
    prev_y = y;
    prev_discretelevel = discrete_level;

    return (discrete_level);
}

int main()
{
    unsigned int i = 0;
    for(i = 0; i <= 100; i++)
        printf("%d %d\n", i ,hysteresis(i));

    for(i = 100; i > 0; i--)
        printf("%d %d\n", i ,hysteresis(i));

    // printf("%d\n", HYST_TRIGG_LOW_1);
    // printf("%d\n", HYST_TRIGG_HIGH_1);
    // printf("%d\n", HYST_TRIGG_LOW_2);
    // printf("%d\n", HYST_TRIGG_HIGH_2);
    // printf("%d\n", HYST_TRIGG_LOW_3);
    // printf("%d\n", HYST_TRIGG_HIGH_3);
    // printf("%d\n", HYST_TRIGG_LOW_4);
    // printf("%d\n", HYST_TRIGG_HIGH_4);

    // // Case 1
    // printf("%d %d\n", 9, hysteresis(9));
    // printf("%d %d\n", 10, hysteresis(10));
    // printf("%d %d\n", 12, hysteresis(12));
    // printf("%d %d\n", 15, hysteresis(15));
    // printf("%d %d\n", 11, hysteresis(11));
    // printf("%d %d\n", 10, hysteresis(10));

    // // Case 2
    // printf("%d %d\n", 34, hysteresis(34));
    // printf("%d %d\n", 35, hysteresis(35));
    // printf("%d %d\n", 37, hysteresis(37));
    // printf("%d %d\n", 40, hysteresis(40));
    // printf("%d %d\n", 36, hysteresis(36));
    // printf("%d %d\n", 35, hysteresis(35));

    // // Case 3
    // printf("%d %d\n", 59, hysteresis(59));
    // printf("%d %d\n", 60, hysteresis(60));
    // printf("%d %d\n", 62, hysteresis(62));
    // printf("%d %d\n", 65, hysteresis(65));
    // printf("%d %d\n", 61, hysteresis(61));
    // printf("%d %d\n", 60, hysteresis(60));

    // // Case 4
    // printf("%d %d\n", 84, hysteresis(84));
    // printf("%d %d\n", 85, hysteresis(85));
    // printf("%d %d\n", 87, hysteresis(87));
    // printf("%d %d\n", 90, hysteresis(90));
    // printf("%d %d\n", 86, hysteresis(86));
    // printf("%d %d\n", 85, hysteresis(85));

    // //Case 5
    // printf("%d %d\n", 5, hysteresis(5));
    // printf("%d %d\n", 25, hysteresis(25));
    // printf("%d %d\n", 50, hysteresis(50));
    // printf("%d %d\n", 75, hysteresis(75));
    // printf("%d %d\n", 100, hysteresis(100));

    // // Case 6
    // printf("%d %d\n", 100, hysteresis(100));
    // printf("%d %d\n", 90, hysteresis(90));
    // printf("%d %d\n", 87, hysteresis(87));
    // printf("%d %d\n", 85, hysteresis(84));
    // printf("%d %d\n", 84, hysteresis(84));
    // printf("%d %d\n", 87, hysteresis(87));

    // // Case 7
    // printf("%d %d\n", 66, hysteresis(66));
    // printf("%d %d\n", 65, hysteresis(65));
    // printf("%d %d\n", 61, hysteresis(61));
    // printf("%d %d\n", 60, hysteresis(60));
    // printf("%d %d\n", 59, hysteresis(59));
    // printf("%d %d\n", 62, hysteresis(62));

    // // Case 7
    // printf("%d %d\n", 66, hysteresis(66));
    // printf("%d %d\n", 65, hysteresis(65));
    // printf("%d %d\n", 61, hysteresis(61));
    // printf("%d %d\n", 60, hysteresis(60));
    // printf("%d %d\n", 59, hysteresis(59));
    // printf("%d %d\n", 62, hysteresis(62));

    // // Case 8
    // printf("%d %d\n", 41, hysteresis(41));
    // printf("%d %d\n", 40, hysteresis(40));
    // printf("%d %d\n", 37, hysteresis(37));
    // printf("%d %d\n", 35, hysteresis(35));
    // printf("%d %d\n", 34, hysteresis(34));
    // printf("%d %d\n", 37, hysteresis(37));

    // // Case 8
    // printf("%d %d\n", 16, hysteresis(16));
    // printf("%d %d\n", 15, hysteresis(15));
    // printf("%d %d\n", 12, hysteresis(12));
    // printf("%d %d\n", 10, hysteresis(10));
    // printf("%d %d\n", 9, hysteresis(9));
    // printf("%d %d\n", 12, hysteresis(12));
    


    return 0;
}