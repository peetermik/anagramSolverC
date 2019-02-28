#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/time.h>

#define maxLineLength 40
#define maxOutputLength 80
int wordVector(char input[]);

int main(int argc, char **argv)
{
    struct timeval timerStart, timerEnd;
    gettimeofday(&timerStart, NULL);

    char *fileName = argv[1];
    char *originalWord = argv[2];
    char *output = malloc(maxOutputLength);
    char originalWordLength = strlen(originalWord);

    //Let's read the anagram list from the specified file to a buffer, binary access for speed.
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
        fclose(f);
    }

    if (fileBuffer) // If the buffer is ok, then let's start.
    {
        int lineCount = 0;
        int charPosCount = 0;
        bool checkActive = true;
        char *currentLine = malloc(maxLineLength);

        for (int i = 0; i < length; i++) // Loop through the entire file buffer.
        {
            if (fileBuffer[i] == '\n') // When we get to a newline character.
            {
                currentLine[charPosCount] = '\0';                          // Mark current line as finished.
                if (charPosCount == originalWordLength && checkActive &&   // If lenghts of the original and current word are equal, and checking is active,
                    wordVector(currentLine) == wordVector(originalWord) && // and vector lenghts of those words are also equal,
                    strcmp(currentLine, originalWord))                     // and the words aren't the same, then we might have an anagram.
                {
                    strcat(output, ",");
                    strcat(output, currentLine);
                }
                checkActive = true; // Enable anagram check again for the next word.
                charPosCount = 0;   // Start counting the next line's character position from 0.
                lineCount++;        // Counting the lines from input file, just for information.
            }
            else if (checkActive && fileBuffer[i] != '\r') // On all other cases (except the carriage return or when we don't care anymore).
            {
                currentLine[charPosCount] = fileBuffer[i];    // Then add the current character from file buffer to current line.
                charPosCount++;                               // Increment the current line character position.
                if (strchr(originalWord, fileBuffer[i]) == 0) // If the current character doesn't exist in our original word,
                    checkActive = false;                      // then stop checking this line
            }
        }
    }

    gettimeofday(&timerEnd, NULL);
    int timeSpent;
    timeSpent = (timerEnd.tv_sec - timerStart.tv_sec) * 1e6;
    timeSpent += (timerEnd.tv_usec - timerStart.tv_usec);
    printf("%d%s\n", timeSpent, output);
    return 0;
}

int wordVector(char input[])
{
    int temp = 0;
    for (int i = 0; i < strlen(input); i++)
    {
        temp += (int)input[i] * (int)input[i];
    }
    return temp;
}
