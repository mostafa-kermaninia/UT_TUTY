#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "recieveInput.h"

#define POSTSIGN -2

int inputCommand(char *splittedCommand[3])
{
    char inputChar;
    int charCount = 0;

    printf("Please enter your command :\n");

    int commandpart = 0;
    while (((inputChar = getchar()) != '\n'))
    {
        charCount++;

        // When we are at the end of a word in command
        if (inputChar == ' ')
        {
            // if we read 3 words before and there is still some words, we got wrong command
            if (commandpart < 2)
            {

                if ((splittedCommand[commandpart] = realloc(splittedCommand[commandpart], (charCount + 1) * sizeof(char))))
                {
                    *(splittedCommand[commandpart] + (charCount - 1)) = '\0';
                    charCount = 0;
                }
                else
                {
                    printf("We couldn't allocate memmory!\n");
                    exit(1);
                }

                // check that is it a post command or not
                if (commandpart == 0 && !strcmp(splittedCommand[0], "post"))
                {
                    return POSTSIGN;
                }

                commandpart++;
            }
            else
            {
                // if we write more than 3 words in command, we should clean buffer after exit
                fflush(stdin);
                return -1;
            }
        }

        // When we are in the middle of a word in command
        else
        {
            if ((splittedCommand[commandpart] = realloc(splittedCommand[commandpart], (charCount + 1) * sizeof(char))))
            {
                *(splittedCommand[commandpart] + (charCount - 1)) = inputChar;
            }
            else
            {
                printf("We couldn't allocate memmory!\n");
                exit(1);
            }
        }
    }

    // put null element for last command
    if (charCount > 0)
    {
        *(splittedCommand[commandpart] + charCount) = '\0';
    }

    return commandpart + 1;
}
