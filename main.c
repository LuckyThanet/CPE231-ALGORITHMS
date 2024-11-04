#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ATTEMPTS 32768
#define CODE_LENGTH 5
#define MAX_COLOR 8
// #define MAX_ATTEMPTS 1296
// #define CODE_LENGTH 4
// #define MAX_COLOR 6
#define MIN_COLOR 1
#define MAX_POPULATION 40
#define MAX_ANSWER 1000
#define MAX_GUESS 1000
#define MAX_ROUND 3
void generateSecretCode(int code[], int length, int min, int max)
{
    for (int i = 0; i < length; i++)
    {
        code[i] = (rand() % (max - min + 1)) + min;
    }
}

void check_guess(int secret_code[], int guess[], int *colored_key_pegs, int *white_key_pegs)
{
    *colored_key_pegs = 0;
    *white_key_pegs = 0;

    // Check for colored key pegs (correct number and position)
    for (int i = 0; i < CODE_LENGTH; i++)
    {
        if (guess[i] == secret_code[i])
        {
            (*colored_key_pegs)++;
        }
    }

    // Check for white key pegs (correct number but wrong position)
    for (int i = 1; i <= MAX_COLOR; i++)
    {
        int secret_code_count = 0;
        int guess_count = 0;

        // Count occurrences of the number in the secret code and guess
        for (int j = 0; j < CODE_LENGTH; j++)
        {
            if (secret_code[j] == i)
            {
                secret_code_count++;
            }
            if (guess[j] == i)
            {
                guess_count++;
            }
        }

        // Calculate white key pegs
        *white_key_pegs += (secret_code_count < guess_count) ? secret_code_count : guess_count;
    }

    // Adjust white key pegs by subtracting the colored key pegs
    *white_key_pegs -= *colored_key_pegs;
}

void inpurfromfile(int *combinationsCode, char *buffer)
{
    for (int i = 0; i < CODE_LENGTH; i++)
    {
        sscanf(buffer, "%d ", &combinationsCode[i]);
        // Move the buffer pointer to the next integer (skip whitespace)
        while (*buffer != '\0' && (*buffer == ' ' || *buffer == '\t' || *buffer == '\n'))
        {
            buffer++;
        }
        // Move the buffer pointer to the next non-whitespace character
        while (*buffer != '\0' && (*buffer != ' ' && *buffer != '\t' && *buffer != '\n'))
        {
            buffer++;
        }
    }
}

void readcombinationsFromFile(int combinationsCode[MAX_ATTEMPTS][CODE_LENGTH])
{
    FILE *file = (CODE_LENGTH == 4) ? fopen("combinations.txt", "r") : fopen("combinations2.txt", "r");
    char buffer[50];
    int count = 0;
    if (file == NULL)
    {
        exit(1);
    }

    while (fgets(buffer, 50, file) != NULL)
    {
        inpurfromfile(combinationsCode[count], buffer);
        count++;
    }

    fclose(file);
}

int isDuplicate(int arr[], int num, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (arr[i] == num)
        {
            return 1; // Found a duplicate
        }
    }
    return 0; // No duplicate found
}

int *generateRandombox(int size, int amount)
{
    int *Randombox = (int *)calloc(size, sizeof(int));
    int generated = 0;
    while (generated < size)
    {
        int randomNumber = rand() % amount;

        if (!isDuplicate(Randombox, randomNumber, generated))
        {
            Randombox[generated] = randomNumber;
            generated++;
        }
    }
    return Randombox;
}

void randomParent(int randomPa[MAX_POPULATION][CODE_LENGTH], int combinationsCode[MAX_ATTEMPTS][CODE_LENGTH])
{
    int *randomNumbers = generateRandombox(MAX_POPULATION, MAX_ATTEMPTS);
    for (int i = 0; i < MAX_POPULATION; i++)
    {
        // for (int j = 0; j < CODE_LENGTH; j++)
        // {
        //     randomPa[i][j] = combinationsCode[randomNumbers[i]][j];
        // }
        memcpy(randomPa[i], combinationsCode[randomNumbers[i]], CODE_LENGTH * sizeof(int));
    }
    free(randomNumbers);
}

void PMX(int *parent1, int *parent2, int *child1, int *child2, int point1, int point2)
{
    int mapA[10], mapB[10];
    memset(mapA, 0, sizeof mapA);
    memset(mapB, 0, sizeof mapB);
    for (int i = point1; i < point2; i++)
    {
        child2[i] = parent1[i];
        mapA[parent1[i]] = parent2[i];
        child1[i] = parent2[i];
        mapB[parent2[i]] = parent1[i];
    }
    for (int i = 0; i < point1; i++)
    {
        child2[i] = mapA[parent2[i]] ? mapA[parent2[i]] : parent2[i];
        child1[i] = mapB[parent1[i]] ? mapB[parent1[i]] : parent1[i];
    }
    for (int i = point2; i < CODE_LENGTH; i++)
    {
        child2[i] = mapA[parent2[i]] ? mapA[parent2[i]] : parent2[i];
        child1[i] = mapB[parent1[i]] ? mapB[parent1[i]] : parent1[i];
    }
}

void UniformCrossover(int *parent1, int *parent2, int *child1, int *child2)
{
    for (int i = 0; i < CODE_LENGTH; i++)
    {
        if (rand() % 2 == 0)
        {
            child1[i] = parent1[i];
            child2[i] = parent2[i];
        }
        else
        {
            child1[i] = parent2[i];
            child2[i] = parent1[i];
        }
    }
}

void Mutation(int amount, int **childs)
{
    int mutationAmount = (amount * 12 / 100);
    int *randomNumbers = generateRandombox(mutationAmount, MAX_POPULATION);
    for (int i = 0; i < mutationAmount; i++)
    {
        int randomindex = rand() % CODE_LENGTH;
        int tmp = (rand() % MAX_COLOR) + 1;
        while (tmp == childs[randomNumbers[i]][randomindex])
        {
            tmp = (rand() % MAX_COLOR) + 1;
        }
        childs[randomNumbers[i]][randomindex] = tmp;
    }
    free(randomNumbers);
}

int **RandomCrossoverParent(int parents[MAX_POPULATION][CODE_LENGTH])
{
    int point1 = rand() % CODE_LENGTH;
    int point2 = rand() % CODE_LENGTH;
    int **offsprings = (int **)calloc(MAX_POPULATION, sizeof(int *));
    int *randomNumbers = generateRandombox(MAX_POPULATION, MAX_POPULATION);
    for (int i = 0; i < MAX_POPULATION; i++)
    {
        offsprings[i] = (int *)calloc(CODE_LENGTH, sizeof(int));
    }
    while (point1 > point2)
    {
        point1 = rand() % CODE_LENGTH;
        point2 = rand() % CODE_LENGTH;
    }
    for (int i = 0; i < MAX_POPULATION / 2; i++)
    {
        PMX(parents[randomNumbers[i]], parents[randomNumbers[i + MAX_POPULATION / 2]], offsprings[i], offsprings[i + MAX_POPULATION / 2], point1, point2);
        // UniformCrossover(parents[randomNumbers[i]], parents[randomNumbers[i + MAX_POPULATION / 2]], offsprings[i], offsprings[i + MAX_POPULATION / 2]);
    }
    free(randomNumbers);
    return offsprings;
}

void copychildtoparent(int parents[MAX_POPULATION][CODE_LENGTH], int **childs)
{
    // Assume the size is MAX_POPULATION x CODE_LENGTH
    for (int i = 0; i < MAX_POPULATION; i++)
    {
        memcpy(parents[i], childs[i], CODE_LENGTH * sizeof(int));
    }
}

void printcode(int *code)
{
    printf("code: ");
    for (int i = 0; i < CODE_LENGTH; i++)
    {
        printf("%d ", code[i]);
    }
    printf("\n");
}

void printGuesses(int *code)
{
    printf("Guesses: ");
    for (int i = 0; i < CODE_LENGTH; i++)
    {
        printf("%d ", code[i]);
    }
    printf("\n");
}

int *calculatefitness(int guess[MAX_GUESS][2], int answer_code[MAX_ANSWER][CODE_LENGTH], int **offsprings, int k, int i, int *runner)
{
    int correct_positionGen = 0;
    int correct_valueGen = 0;
    int sum = 0;
    int *result = (int *)malloc(sizeof(int) * 1000);
    int count = 0;
    for (int l = 0; l < k; l++)
    {
        sum = 0;

        if (count >= 100)
        {
            break;
        }
        for (int m = 0; m < i; m++)
        {
            correct_positionGen = correct_valueGen = 0;
            check_guess(answer_code[m], offsprings[l], &correct_positionGen, &correct_valueGen);
            sum += abs(guess[m][0] - correct_positionGen) + abs(guess[m][1] - correct_valueGen);
        }
        if (sum == 0)
        {
            result[count++] = l;
        }
    }
    (*runner) = count;
    return result;
}

int compareIntArrays(int arr1[], int arr2[], int size) 
{
    for (int i = 0; i < size; i++) 
    {
        if (arr1[i] < arr2[i]) 
        {
            return -1;
        } else if (arr1[i] > arr2[i]) 
        {
            return 1;
        }
    }
    return 0; 
}

int main()
{
    srand(time(0) + 1);
    int a[3] = {0};
    double sum = 0;
    for (int i = 0; i < MAX_ROUND; i++)
    {
        int secret_code[CODE_LENGTH];
        int guess[MAX_GUESS][2];
        int combinationsCode[MAX_ATTEMPTS][CODE_LENGTH];
        int randomParents[MAX_POPULATION][CODE_LENGTH] = {0};
        int answer_code[MAX_ANSWER][CODE_LENGTH]; 
        int fitness_zero[MAX_ANSWER][CODE_LENGTH];
        int turn = 0;
        int runner = 0;
        if (CODE_LENGTH == 4)
        {
            int initialGuess[] = {1, 1, 2, 3};
            memcpy(answer_code[0], initialGuess, sizeof(initialGuess));
        }
        else
        {
            int initialGuess[] = {1, 1, 2, 3, 4};
            memcpy(answer_code[0], initialGuess, sizeof(initialGuess));
        }
        generateSecretCode(secret_code, CODE_LENGTH, MIN_COLOR, MAX_COLOR);
        readcombinationsFromFile(combinationsCode);
        check_guess(secret_code, answer_code[0], &guess[0][0], &guess[0][1]);
        while (guess[turn][0] != CODE_LENGTH)
        {
            turn++;
            int count = 0;
            randomParent(randomParents, combinationsCode);
            while (1)
            {
                int **offsprings = RandomCrossoverParent(randomParents);
                runner = 0;
                Mutation(MAX_POPULATION, offsprings);
                int *resultindexfitness = calculatefitness(guess, answer_code, offsprings, MAX_POPULATION, turn, &runner);
                copychildtoparent(randomParents, offsprings);
                if (runner > 0)
                {
                    for (int i = 0; i < runner; i++)
                    {
                        memcpy(fitness_zero[i], offsprings[resultindexfitness[i]], CODE_LENGTH * sizeof(int));
                        printcode(fitness_zero[i]);
                    }
                    memcpy(answer_code[turn], fitness_zero[runner - 1], CODE_LENGTH * sizeof(int));
                    break;
                }
            }
            printf("Answer %d = ", turn);
            printcode(answer_code[turn]);
            check_guess(secret_code, answer_code[turn], &guess[turn][0], &guess[turn][1]);
            printf("B = %d W = %d\n", guess[turn][0], guess[turn][1]);
            for (int i = 0; i < turn; i++)
            {
                printGuesses(answer_code[i]);
            }
            printf("\n");
        }
        printf("Secret = ");
        printcode(secret_code);
        printf("--------------------------------------------\n");
        sum += turn;
        a[i] = turn;
    }
    printf("mean = %.2lf ROUND ", sum / MAX_ROUND);
    for(int i = 0 ; i < 3 ; i++)
    {
      printf("%d ",a[i]);
    }
    return 0;
}

// "Code: 1 2 3 4, fitness: 0"