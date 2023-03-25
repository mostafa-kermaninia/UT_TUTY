#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "mainFuncs.h"
#include "updateFiles.h"
#include "addStructsNode.h"
#include "findUser.h"

#define TRUE 1
#define FALSE 0

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

    // *Save new user info in accounts File
    updateAccountsFile(usersHead);

    printf("*** You signed up successfully! ***\n\n");
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
int post(users *currentUser, users *usersHead)
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

    // *Update user info(posts count) in accounts File and update post info in posts File
    updateAccountsFile(usersHead);
    updatePostsFile(usersHead);

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

                    // *Update post info(likes count) in posts File
                    updatePostsFile(usersHead);

                    printf("*** You like this post successfuly! ***\n\n");
                    return 0;
                }
                postsNode = postsNode->nextPost;
            }
        }
        usersNode = usersNode->nextUser;
    }
    printf("We couldn't find any post with inputted postId from inputted user or username is wrong!\n");
    return -1;
}
void logout()
{
    printf("*** You logged out successfully! ***\n\n");
}
int deletePost(int postId, users *currentUser, users *usersHead)
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

            // *Update user info(posts count) in accounts File and update post info in posts File
            updateAccountsFile(usersHead);
            updatePostsFile(usersHead);

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
