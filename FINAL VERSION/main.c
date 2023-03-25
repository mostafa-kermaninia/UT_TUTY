// gcc main.c addStructsNode.c mainFuncs.c recieveInput.c updateFiles.c findUser.c

#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "addStructsNode.h"
#include "findUser.h"
#include "mainFuncs.h"
#include "recieveInput.h"
#include "updateFiles.h"

#define TRUE 1
#define FALSE 0
#define POSTSIGN -2

int main()
{
    // create dummy node(head) of users list
    users *usersHead = addUsersNode(" ", " ", 0);

    int currentUserId;
    int loggedIn = FALSE;
    while (TRUE)
    {
        char *splittedCommand[3] = {NULL, NULL, NULL};
        // this function returns count of commands that it recognized,
        // and returns -1 if written command has more than 3 parts
        int commandsCount = inputCommand(splittedCommand);
        int badCommand = TRUE;

        switch (commandsCount)
        {
        case 1:
        {
            /* LOGOUT part */
            /* splittedCommand will be like this : {"logout"} */
            if (!strcmp(splittedCommand[0], "logout"))
            {
                if (loggedIn)
                {
                    logout();
                    loggedIn = FALSE;
                }
                else
                    printf("You can't logout when you aren't in your account!\n");
                badCommand = FALSE;
            }

            /* INFO part */
            /* splittedCommand will be like this : {"info"} */
            else if (!strcmp(splittedCommand[0], "info"))
            {
                if (loggedIn)
                {
                    info(currentUserId, usersHead);
                }
                else
                {
                    printf("You can't see info when you aren't in your account!\n");
                }
                badCommand = FALSE;
            }

            break;
        }
        case 2:
        {
            /* DELETE part */
            /* splittedCommand will be like this : {"delete", <post_id>} */
            if (!strcmp(splittedCommand[0], "delete"))
            {
                if (loggedIn)

                    deletePost(atoi(splittedCommand[1]), findUser(currentUserId, usersHead), usersHead);
                else
                    printf("You can't delete when you aren't in your account!\n");
                badCommand = FALSE;
            }

            /* FIND_USER part */
            /* splittedCommand will be like this : {"find_user", <username>} */
            else if (!strcmp(splittedCommand[0], "find_user"))
            {
                // this function doesn't need to login or logout
                find_user(splittedCommand[1], usersHead);
                badCommand = FALSE;
            }

            break;
        }
        case 3:
        {
            /* SIGNUP part */
            /* splittedCommand will be like this : {"signup", <user name>, <password>} */
            if (!strcmp(splittedCommand[0], "signup"))
            {
                if (loggedIn)
                {
                    printf("You can't signup when you are in your account!\n");
                }
                else
                {
                    signUp(splittedCommand[1], splittedCommand[2], usersHead);
                }
                badCommand = FALSE;
            }

            /* LOGIN part */
            /* splittedCommand will be like this : {"login", <user name>, <password>} */
            else if (!strcmp(splittedCommand[0], "login"))
            {
                if (loggedIn)
                {
                    printf("You can't login when you are in your account!\n");
                }
                else
                {
                    // if login was successful, change user's login status
                    currentUserId = logIn(splittedCommand[1], splittedCommand[2], usersHead);
                    if (currentUserId)
                    {
                        loggedIn = TRUE;
                    }
                }
                badCommand = FALSE;
            }

            /* LIKE part */
            /* splittedCommand will be like this : {"like", <user name>, <postId>} */
            else if (!strcmp(splittedCommand[0], "like"))
            {
                if (loggedIn)
                {
                    like(splittedCommand[1], atoi(splittedCommand[2]), usersHead, currentUserId);
                }
                else
                {
                    printf("You can't like when you aren't in your account!\n");
                }
                badCommand = FALSE;
            }

            break;
        }
        case POSTSIGN:
        {
            /* POST part */
            /* splittedCommand will be like this : {"post"} */
            if (loggedIn)
            {
                post(findUser(currentUserId, usersHead), usersHead);
            }
            else
            {
                printf("You can't post when you aren't in your account!\n");
                fflush(stdin);
            }
            badCommand = FALSE;

            break;
        }
        case -1:
        {
            printf("Your command is too big, try again\n");
            // We have not bad command, its just too big!
            badCommand = FALSE;

            break;
        }
        }

        if (badCommand)
        {
            printf("Your command is wrong, try again\n");
        }
    }

    return 0;
}
