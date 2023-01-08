#include <stdio.h>
#include <malloc.h>
#include <string.h>

// include headerfiles

// define consts
#define TRUE 1
#define FALSE 0
#define postSign -2

// structures
typedef struct likers
{
    int likerId;
    struct likers *nextLiker;
} likers;

typedef struct posts
{
    char *ownerUsername;
    int postId;
    int likes;
    char *content;
    likers *likersHead;

    struct posts *nextPost;
} posts;

typedef struct users
{
    char *username;
    char *password;
    int userId;
    posts *postsHead;

    struct users *nextUser;
} users;

// declarations
users *addUsersNode(char *username, char *password, int userId);
posts *addPostsNode(char *ownerUsername, int postId, char *content);
likers *addLikersNode(int userId);

// Auxiliary functions
int inputCommand(char *splittedCommand[3]);
users *findUser(int userId, users *usersHead);

// main functions
int signUp(char *username, char *password, users *usersHead);
int logIn(char *username, char *password, users *usersHead);
int post(users *currentUser);
int like(char *postOwnerusername, int postId, users *usersHead, int currentUserId);
void logout();
int delete(int postId, users *currentUser);
void info(int Id, users *usersHead);
int find_user(char *username, users *usersHead);

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

                    delete (atoi(splittedCommand[1]), findUser(currentUserId, usersHead));
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
        case postSign:
        {
            /* POST part */
            /* splittedCommand will be like this : {"post"} */
            if (loggedIn)
            {
                post(findUser(currentUserId, usersHead));
            }
            else
            {
                printf("You can't post when you aren't in your account!\n");
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

users *addUsersNode(char *username, char *password, int userId)
{
    // this function build a linked list with just one node,we can append it to the tail of main list
    users *node;
    if ((node = (users *)malloc(sizeof(users))))
    {
        node->username = username;
        node->password = password;
        node->userId = userId;
        // for each user we should make a post list , create dummy node(head) of posts list first
        node->postsHead = addPostsNode(username, 0, NULL);
        node->nextUser = NULL;

        return node;
    }
    else
    {
        printf("We couldn't allocate memmory!\n");
        exit(1);
    }
}

posts *addPostsNode(char *ownerUsername, int postId, char *content)
{
    // this function build a linked list with just one node,we can append it to the tail of main list
    posts *node;
    if ((node = (posts *)malloc(sizeof(posts))))
    {
        node->ownerUsername = ownerUsername;
        node->postId = postId;
        node->likes = 0;
        node->content = content;
        // for each post we should make a likers list , create dummy node(head) of likers list first
        node->likersHead = addLikersNode(0);
        node->nextPost = NULL;

        return node;
    }
    else
    {
        printf("We couldn't allocate memmory!\n");
        exit(1);
    }
}

likers *addLikersNode(int userId)
{
    // this function build a linked list with just one node,we can append it to the tail of main list
    likers *node;
    if ((node = (likers *)malloc(sizeof(likers))))
    {
        // we dont need to define username, password or postsHead for liker,id is enough
        node->likerId = userId;
        node->nextLiker = NULL;

        return node;
    }
    else
    {
        printf("We couldn't allocate memmory!\n");
        exit(1);
    }
}

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
                    return postSign;
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

int signUp(char *username, char *password, users *usersHead)
{
    // check validity of inputs
    if (!strcmp(username, "signup") || !strcmp(username, "logout") || !strcmp(username, "login") ||
        !strcmp(username, "like") || !strcmp(username, "post") || !strcmp(username, "find_user") ||
        !strcmp(username, "info") || !strcmp(username, "delete"))
    {
        printf("you cant use command_names for your username! try again\n");
        return 1;
    }

    // iterate all users in list
    users *node = usersHead;
    users *prev;
    while (node)
    {
        // duplicated username error
        if (!strcmp(node->username, username))
        {
            printf("this username used before! try again\n");
            return 1;
        }
        prev = node; // we will use prev after the loop
        node = node->nextUser;
    }
    // now prev is the tail, and we should add new signedup user after it
    users *new = addUsersNode(username, password, (prev->userId) + 1);
    prev->nextUser = new;

    printf("*** You signed up successfully!*** \n\n");
    return 0;
}

int logIn(char *username, char *password, users *usersHead)
{
    // check validity of inputs and iterate all users in list
    users *node = usersHead;
    while (node)
    {
        // search for a user with input username and password
        if (!strcmp(node->username, username) && !strcmp(node->password, password))
        {
            printf("*** You logged in successfully! ***\n\n");
            return node->userId;
        }
        node = node->nextUser;
    }

    printf("We couldn't find any user with this username and password!\n");
    return 0;
}

int post(users *currentUser)
{
    char inputChar;
    char *postText = NULL;
    int charCount = 0;
    int isEmpty = TRUE;

    while ((inputChar = getchar()) != '\n')
    {
        // check that the post isn't empty
        if (inputChar != ' ')
        {
            isEmpty = FALSE;
        }

        charCount++;
        if ((postText = realloc(postText, (charCount + 1) * sizeof(char))))
        {
            *(postText + (charCount - 1)) = inputChar;
        }
        else
        {
            printf("We couldn't allocate memmory!\n");
            exit(1);
        }
    }
    // put null at the end of string
    if (isEmpty == FALSE)
    {
        *(postText + charCount) = '\0';
    }
    else
    {
        printf("Your post can't be empty!");
        return -1;
    }

    // find last post of user
    posts *node = currentUser->postsHead;
    while (node->nextPost)
    {
        node = node->nextPost;
    }
    // add new post to tail
    posts *new = addPostsNode(currentUser->username, node->postId + 1, postText);
    node->nextPost = new;

    printf("*** Your post aploaded successfully! ***\n\n");
    return 0;
}

int like(char *postOwnerusername, int postId, users *usersHead, int currentUserId)
{
    // check validity of inputs and iterate all users in list(we passinged dummy node)
    users *usersNode = usersHead;
    while (usersNode)
    {
        // search for a user with inputted username
        if (!strcmp(usersNode->username, postOwnerusername))
        {
            // iterate all posts of selected user (we passinged dummy node)
            posts *postsNode = usersNode->postsHead->nextPost;
            while (postsNode)
            {
                // search for a post with inputted id(if inputted postId is equal to a post's postId, its valid)
                if (postsNode->postId == postId)
                {
                    // check that its the first time that user likes this post or not
                    // iterate all likers of selected post
                    likers *likersNode = postsNode->likersHead;
                    likers *prevLiker;
                    while (likersNode)
                    {
                        // if user is in likers list, he can't like this post
                        if (likersNode->likerId == currentUserId)
                        {
                            printf("You've liked this post before so you can't like it!\n");
                            return 1;
                        }
                        prevLiker = likersNode;
                        likersNode = likersNode->nextLiker;
                    }
                    // now we need add user informations in likers list and increase likes count of post
                    postsNode->likes = postsNode->likes + 1;
                    // now prevliker is the tail, and we should add new new liker user after it
                    likers *new = addLikersNode(currentUserId);
                    prevLiker->nextLiker = new;

                    printf("*** You like this post successfuly! ***\n\n");
                    return 0;
                }
                postsNode = postsNode->nextPost;
            }
        }
        usersNode = usersNode->nextUser;
    }
    printf("We couldn't find any post with inputted postId from inputted user or username is wrong!\n");
}

void logout()
{
    printf("*** You logged out successfully! ***\n\n");
}

int delete(int postId, users *currentUser)
{
    // check validity of inputs and iterate all posts of currenc user
    posts *node = currentUser->postsHead;
    posts *prev;
    while (node->nextPost)
    {
        prev = node;
        node = node->nextPost;

        // search for a post with inputted id
        if (node->postId == postId)
        {
            // delete and free selected post
            prev->nextPost = node->nextPost;
            free(node);

            printf("*** We deleted selected post successfully! ***\n\n");
            return 0;
        }
    }
    printf("We couldn't find selected post \n\n");
    return 1;
}

void info(int Id, users *usersHead)
{
    users *user = findUser(Id, usersHead);
    // print account info
    printf("* Here is your account information *\nYour username is %s\nYour password is %s\n* And Here is your posts information *\n", user->username, user->password);

    // iterate posts and print their info
    posts *postsNode = user->postsHead->nextPost; // we go to the second node because its first node is dummy node
    while (postsNode)
    {
        printf("%s\n likes : %d\n*********\n", postsNode->content, postsNode->likes);
        postsNode = postsNode->nextPost;
    }
}

int find_user(char *username, users *usersHead)
{
    // check validity of input and iterate all users in list
    users *usersNode = usersHead;
    while (usersNode)
    {
        // search for a user with input username
        if (!strcmp(usersNode->username, username))
        {
            // print all posts with their informations
            printf("*** Here is selected user's posts information ***\n");
            posts *postsNode = usersNode->postsHead->nextPost;
            while (postsNode)
            {
                printf("postOwner : %s\npostId : %d\ncontent : %s\nlikesCount : %d\n* * * * * * * *\n", usersNode->username, postsNode->postId, postsNode->content, postsNode->likes);
                postsNode = postsNode->nextPost;
            }
            return 0;
        }
        usersNode = usersNode->nextUser;
    }
    printf("We don't have any user with selected username\n");
    return 1;
}

users *findUser(int userId, users *usersHead)
{
    users *node = usersHead;
    while (node)
    {
        if (node->userId == userId)
        {
            return node;
        }
        node = node->nextUser;
    }
    return NULL;
}