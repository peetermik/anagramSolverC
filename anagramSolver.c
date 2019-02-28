#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/time.h>

#define maxWordLength 40
#define maxOutputLength 80

int wordVector(char input[]);

int main(int argc, char **argv)
{
    struct timeval timerStart, timerEnd;
    gettimeofday(&timerStart, NULL);

    char *fileName = argv[1];
    char *originalWord = malloc(maxWordLength);

    // Get the original search word(s) from arguments
    strcat(originalWord, argv[2]);
    if (argc > 3)
    {
        for (int i = 3; i < argc; i++)
        {
            strcat(originalWord, " ");
            strcat(originalWord, argv[i]);
        }
    }

    char *output = malloc(maxOutputLength);
    int originalWordLength = strlen(originalWord);
    int originalWordVector = wordVector(originalWord);

    // Let's read the entire anagram list file to a buffer, binary access for speed.
    char *fileBuffer = 0;
    long length;
    FILE *f = fopen(fileName, "rb");
    if (f)
    {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        fileBuffer = malloc(length);
        if (fileBuffer)
        {
            fread(fileBuffer, 1, length, f);
        }
        fclose(f);
    }

    if (fileBuffer) // If the buffer is ok then start.
    {
        int lineCount = 0;
        int charPosCount = 0;
        bool checkActive = true;
        char *currentLine = malloc(maxWordLength);

        for (int i = 0; i < length; i++) // Loop through the entire file buffer.
        {
            if (fileBuffer[i] == '\n') // When we get to a newline character, then...
            {
                currentLine[charPosCount] = '\0';                          // Mark current line as finished.
                if (charPosCount == originalWordLength && checkActive &&   // If lenghts of the original and current word are equal, and checking is active,
                    wordVector(currentLine) == originalWordVector && // and vector lenghts of those words are also equal,
                    strcmp(currentLine, originalWord))                     // and the words aren't the same, then we might have an anagram.
                {
                    // TODO: Extra check to be sure that we really have an anagram.
                    // At this point we just know that all the characters from the anagram candidate exist in the original word,
                    // words have the same length and vectors have the same value.
                    // So a small possibility of collisions does exist.
                    strcat(output, ",");
                    strcat(output, currentLine);
                }
                checkActive = true; // Enable anagram check again for the next word.
                charPosCount = 0;   // Start counting the next line's character position from 0.
                lineCount++;        // Counting the lines from input file, just for information.
            }
            else if (checkActive && fileBuffer[i] != '\r') // On all other characters (except the carriage return or when we don't care anymore).
            {
                currentLine[charPosCount] = fileBuffer[i];    // Add the current character from file buffer to current line.
                charPosCount++;                               // Increment the current line character position.
                if (strchr(originalWord, fileBuffer[i]) == 0) // If the current character doesn't exist in our original word,
                    checkActive = false;                      // then stop checking this line
            }
        }
    }

    gettimeofday(&timerEnd, NULL);                           // Stop the timer
    int timeSpent;                                           // and store the total time spent in microseconds..
    timeSpent = (timerEnd.tv_sec - timerStart.tv_sec) * 1e6; // Convert timer interval seconds to microseconds
    timeSpent += (timerEnd.tv_usec - timerStart.tv_usec);    // and add the microsecond count to get the total amount.
    printf("%d%s\n", timeSpent, output);
    return 0;
}

int wordVector(char input[]) // Calculate the word vector lenght, just add up all the values of each character squared.
{
    int temp = 0;
    for (int i = 0; i < strlen(input); i++)
    {
        temp += (int)input[i] * (int)input[i];
    }
    return temp;
}
