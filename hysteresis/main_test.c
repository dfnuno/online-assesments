#include <stdio.h>

/* Y = MX + B*/

#define N (4)              /*Discrete Levels*/
#define HYST (25)          /*Hystereis tolerance for analogue input 2.5*/
#define M (1000 / N)       /*Slope M = 25*/
#define B (1000 / (2 * N)) /*Interception B = 12.5*/
#define DEBUG_TEST
//#define DEBUG
#define TEST

/*Function Prototypes*/
unsigned int hysteresis(unsigned int input_percent);
unsigned int getDiscreteLevel(unsigned int prev, unsigned int curr, unsigned int prev_disc_level);

#ifdef TEST
unsigned int pass = 0;
unsigned int fail = 0;
unsigned int executed = 0;

static void test_result(const char *test_name, unsigned int level, unsigned int expected)
{
    if (level == expected)
    {
        pass++;
#ifdef DEBUG_TEST
        printf("Testcase %d - %s -> PASSED\n", executed, test_name);
#endif
    }
    else
    {
        fail++;
#ifdef DEBUG_TEST
        printf("Testcase %d - %s -> FAILED - ", executed, test_name);
        printf("returned level %d != expected %d.\n", level, expected);
#endif
    }
    executed++;
}
static void test_init(void)
{
    test_result("init = 0", hysteresis(0), 0);
}
static void test_up(void)
{
    test_result("0  -> 14 ", hysteresis(14), 0);
    test_result("14 -> 16 ", hysteresis(16), 1);
    test_result("16 -> 41 ", hysteresis(41), 2);
    test_result("41 -> 66 ", hysteresis(66), 3);
    test_result("66 -> 91 ", hysteresis(91), 4);
}
static void test_down(void)
{
    test_result("86 <- 91 ", hysteresis(86), 4);
    test_result("84 <- 86 ", hysteresis(84), 3);
    test_result("59 <- 84 ", hysteresis(59), 2);
    test_result("34 <- 59 ", hysteresis(34), 1);
    test_result(" 9 <- 34 ", hysteresis(9), 0);
}
static void test_thresholds(void)
{
    test_result("Threshold 15", hysteresis(15), 1);
    test_result("Threshold 40", hysteresis(40), 2);
    test_result("Threshold 65", hysteresis(65), 3);
    test_result("Threshold 90", hysteresis(90), 4);
    test_result("Threshold 85", hysteresis(85), 3);
    test_result("Threshold 60", hysteresis(60), 2);
    test_result("Threshold 35", hysteresis(35), 1);
    test_result("Threshold 10", hysteresis(10), 0);
}
static void test_same_level(void)
{
    test_result("set lvl 0", hysteresis(0), 0);
    test_result("same lvl 0  -> 14", hysteresis(14), 0);

    test_result("set lvl 1", hysteresis(16), 1);
    test_result("same lvl 16 -> 39", hysteresis(39), 1);

    test_result("set lvl 2", hysteresis(41), 2);
    test_result("same lvl 41 -> 64", hysteresis(64), 2);

    test_result("set lvl 3", hysteresis(66), 3);
    test_result("same lvl 66 -> 89", hysteresis(89), 3);

    test_result("set lvl 4", hysteresis(91), 4);
    test_result("same lvl 91 -> 100", hysteresis(100), 4);
}
static void test_jumps(void)
{
    (void)hysteresis(16);
    test_result("jump 16 -> 75 ", hysteresis(75), 3);
    test_result("jump  5 <- 75 ", hysteresis(5), 0);
    test_result("jump  5 -> 100", hysteresis(100), 4);
    test_result("jump 50 <- 100", hysteresis(50), 2);
    test_result("jump 50 -> 91 ", hysteresis(91), 4);
    test_result("jump 25 <- 91 ", hysteresis(25), 1);
}
void test_run_all(void)
{
    test_init();
    test_up();
    test_down();
    test_thresholds();
    test_same_level();
    test_jumps();

    printf("Tests Passed:    %d\n", pass);
    printf("Tests Failed:    %d\n", fail);
    printf("Tests Executed:  %d\n", executed);
}
#endif

int main()
{
    unsigned int i = 0;

    test_run_all();

    return 0;
}

unsigned int getDiscreteLevel(unsigned int prev, unsigned int curr, unsigned int prev_disc_level)
{
    unsigned int discrete_level = 0;
    if (prev < curr)
    {
#ifdef DEBUG
        printf("discrete level = (%i - %i + %i) / %i; ", curr, HYST, B, M);
#endif
        discrete_level = ((curr - HYST + B) / M);
    }
    else if (prev > curr)
    {
#ifdef DEBUG
        printf("discrete level = (%i + %i + %i) / %i; ", curr, HYST, B, M);
#endif
        discrete_level = ((curr + HYST + B) / M);
    }
    else
    {
        discrete_level = prev_disc_level;
    }
#ifdef DEBUG
    printf("discrete level = %i\n", discrete_level);
#endif
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

    /*Check if the value is in one of the hysteresis ranges*/
    for (i = 1; i < 2 * N && found_val == 0; i += 2)
    {
        /*To check if current input is in the hysteresis range*/
#ifdef DEBUG
        printf("y = %i; HYST LOW [%i - %i] HYST HIGH\n", y, i * B - HYST, i * B + HYST);
#endif
        if ((y > i * B - HYST) && (y < i * B + HYST))
        {
            /*To check if previous input is in the hysteresis range*/
            if ((prev_y > i * B - HYST) && (prev_y < i * B + HYST))
            {
                discrete_level = prev_discretelevel;
#ifdef DEBUG
                printf("discrete level (prev)= %i\n", discrete_level);
#endif
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
#ifdef DEBUG
        printf("discrete level = %i\n", discrete_level);
#endif
    }
    /*Save the historical values to compare in a new entry*/
    prev_y = y;
    prev_discretelevel = discrete_level;

    return (discrete_level);
}
