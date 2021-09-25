/*
 * @name: ADC hysterisis
 * @author: Diego Nuno
 * @date: xx-xx-2021
*/
#include <stdio.h>

/* Y = MX + B*/

#define N (4)              /*Discrete Levels*/
#define HYST (25)          /*Hystereis tolerance for analogue input 2.5*/
#define M (1000 / N)       /*Slope M = 25*/
#define B (1000 / (2 * N)) /*Interception B = 12.5*/
#define TEST

/*Data types*/
typedef struct
{
    unsigned int rising_edge;
    unsigned int falling_edge;
} stThreshold;

/*Function Prototypes*/
unsigned int hysteresis(unsigned int input_percent);
unsigned int getDiscreteLevel(unsigned int prev, unsigned int curr, unsigned int prev_disc_level);
void setHysteresisThresholds(void);

/*Global Variables*/
stThreshold astHystereis[N];
#ifdef TEST
unsigned int passed;
unsigned int failed;
unsigned int tests;
#endif

/*Unit test code*/
#ifdef TEST
void test_result(const char *testcase_name, unsigned int discrete_level, unsigned int expected_level)
{
    if (discrete_level == expected_level)
    {
        printf("Testcase %d - %s => PASSED\n", ++tests, testcase_name);
        passed++;
    }
    else
    {
        printf("Testcase %d - %s => FAILED : returned level %d != expected %d.\n", ++tests, testcase_name, discrete_level, expected_level);
        failed++;
    }
}
void test_init(void)
{
    passed = 0;
    failed = 0;
    tests = 0;
    test_result("init = 0", hysteresis(0), 0);
}
void test_show_theshold_limits(void)
{
    unsigned int i = 0;
    unsigned int low = 0;
    unsigned int high = 0;

    printf("0\t");
    for (i = 1; i < (2 * N); i += 2)
    {
        low = i * B - HYST;
        high = i * B + HYST;
        printf("[%d, %d]\t", low / 10, high / 10);
    }
    printf("100\n");
}
void test_analogue_input_up(void)
{
    test_result("0  -> 14 ", hysteresis(14), 0);
    test_result("14 -> 16 ", hysteresis(16), 1);
    test_result("16 -> 41 ", hysteresis(41), 2);
    test_result("41 -> 66 ", hysteresis(66), 3);
    test_result("66 -> 91 ", hysteresis(91), 4);
}
void test_analogue_input_down(void)
{
    test_result("86 <- 91 ", hysteresis(86), 4);
    test_result("84 <- 86 ", hysteresis(84), 3);
    test_result("59 <- 84 ", hysteresis(59), 2);
    test_result("34 <- 59 ", hysteresis(34), 1);
    test_result(" 9 <- 34 ", hysteresis(9), 0);
}
void test_analogue_input_threshold(void)
{
    /*rising edge thresholds*/
    test_result("Rising Edge Trigger 15", hysteresis(15), 1);
    test_result("Rising Edge Trigger 40", hysteresis(40), 2);
    test_result("Rising Edge Trigger 65", hysteresis(65), 3);
    test_result("Rising Edge Trigger 90", hysteresis(90), 4);
    /*faling edge thresholds*/
    test_result("Falling Edge Trigger 85", hysteresis(85), 3);
    test_result("Falling Edge Trigger 60", hysteresis(60), 2);
    test_result("Falling Edge Trigger 35", hysteresis(35), 1);
    test_result("Falling Edge Trigger 10", hysteresis(10), 0);
}
void test_memory_effect(void)
{
    /*Up*/
    test_result("discrete lvl 0", hysteresis(0), 0);
    test_result("discrete lvl 0,  0  -> 14", hysteresis(14), 0);
    test_result("discrete lvl 1", hysteresis(15), 1);
    test_result("discrete lvl keep lvl 1", hysteresis(16), 1);
    test_result("discrete lvl 1,  16 -> 39", hysteresis(39), 1);
    test_result("discrete lvl 2", hysteresis(40), 2);
    test_result("discrete lvl keep lvl 2", hysteresis(41), 2);
    test_result("discrete lvl 2,  41 -> 64", hysteresis(64), 2);
    test_result("discrete lvl 3", hysteresis(65), 3);
    test_result("discrete lvl keep lvl 3", hysteresis(66), 3);
    test_result("discrete lvl 3,  66 -> 89", hysteresis(89), 3);
    test_result("discrete lvl 4", hysteresis(90), 4);
    test_result("discrete lvl keep lvl 4", hysteresis(91), 4);
    test_result("discrete lvl 4,  91 -> 100", hysteresis(100), 4);

    /*Down*/
    test_result("discrete lvl 3", hysteresis(85), 3);
    test_result("discrete lvl keep lvl 3", hysteresis(84), 3);
    test_result("discrete lvl 3,  61 <- 84", hysteresis(61), 3);
    test_result("discrete lvl 2", hysteresis(60), 2);
    test_result("discrete lvl keep lvl 2", hysteresis(59), 2);
    test_result("discrete lvl 2,  36 <- 59", hysteresis(36), 2);
    test_result("discrete lvl 1", hysteresis(35), 1);
    test_result("discrete lvl keep lvl 1", hysteresis(34), 1);
    test_result("discrete lvl 1,  11 <- 34", hysteresis(11), 1);
    test_result("discrete lvl 0", hysteresis(10), 0);
    test_result("discrete lvl keep lvl 0", hysteresis(9), 0);
    test_result("discrete lvl 0,   0 <- 9", hysteresis(0), 0);
}
void test_random_values(void)
{
    /*jump to discrete values (not necessarily step by step)*/
    test_result("0  -> 95 ", hysteresis(95), 4);
    test_result("38 <- 95 ", hysteresis(38), 2);
    test_result("9  <- 38", hysteresis(9), 0);
    test_result("9  -> 81", hysteresis(81), 3);
    test_result("81 -> 99 ", hysteresis(91), 4);
    test_result("20 <- 99 ", hysteresis(20), 1);
}
void test_summary(void)
{
    printf("----------------------------\n");
    printf("Tests Executed: %d\n", tests);
    printf("Tests Passed:   %d\n", passed);
    printf("Tests Failed:   %d\n", failed);
    
}
void test_run_all(void)
{
    test_show_theshold_limits();
    test_init();
    test_analogue_input_up();
    test_analogue_input_down();
    test_analogue_input_threshold();
    test_memory_effect();
    test_random_values();
    test_summary();
}
#endif

int main()
{
#ifdef TEST
    test_run_all();
#endif
    return 0;
}

void setHysteresisThresholds(void)
{
    unsigned int i = 0;
    unsigned int j = 0;

    for (j = 1, i = 0; i < N; j += 2, i++)
    {
        astHystereis[i].falling_edge = (j * B) - HYST;
        astHystereis[i].rising_edge = (j * B) + HYST;
    }
}

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
    unsigned int i = 0;
    unsigned int found_val = 0;
    unsigned int discrete_level = 0;
    unsigned int y = input_percent * 10; /*To work in hundreds and preserve the decimal val*/
    static unsigned int prev_y = 0;
    static unsigned int prev_discretelevel = 0;
    static unsigned int init = 0;

    /*Set hysteresis thresholds*/
    if(init == 0)
    {
        setHysteresisThresholds();
        init = 1;
    }
    /*Check if the value is in one of the hysteresis ranges*/
    for (i = 0; ((i < N) && (found_val == 0)); i++)
    {
        /*To check if current input is in the hysteresis range*/
        if ((y > astHystereis[i].falling_edge) && (y < astHystereis[i].rising_edge))
        {
            /*To check if previous input is in the hysteresis range*/
            if ((prev_y > astHystereis[i].falling_edge) && (prev_y < astHystereis[i].rising_edge))
            {
                discrete_level = prev_discretelevel;
            }
            else
            {
                discrete_level = getDiscreteLevel(prev_y, y, prev_discretelevel);
            }
            /*Exit loop, value found, value encoded*/
            found_val = 1;
        }
    }
    /*If the value was not encoded yet*/
    if (!found_val)
    {
        /*Calculate the discrete value from the linear graph*/
        discrete_level = ((y + B) / M);
    }
    /*Save the historical values to compare in a new entry*/
    prev_y = y;
    prev_discretelevel = discrete_level;

    return (discrete_level);
}
