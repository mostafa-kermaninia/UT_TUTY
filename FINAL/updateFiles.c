#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "updateFiles.h"

#define ACCOUNTS_FILE_ADDRESS "accounts.txt"
#define POSTS_FILE_ADDRESS "posts.txt"

int updateAccountsFile(users *usersHead)
{
    FILE *accountsFile = fopen(ACCOUNTS_FILE_ADDRESS, "w");
    if (accountsFile == NULL)
    {
        printf("Users file can't be opened!\n");
        exit(1);
    }
    fprintf(accountsFile, "*** Here is the format of lines : <username> <password> <number_of_posts> ***\n");

    users *usersNode = usersHead->nextUser; // we passed dummy node of usersList
    posts *postsNode;

    while (usersNode)
    {
        postsNode = usersNode->postsHead;
        // calculate count of user's posts
        int postsCount = -1; // we start from -1 because we don't count dummy node
        while (postsNode)
        {
            postsCount++;
            postsNode = postsNode->nextPost;
        }

        fprintf(accountsFile, "%s %s %d\n", usersNode->username, usersNode->password, postsCount);
        usersNode = usersNode->nextUser;
    }
    fclose(accountsFile);

    return 0;
}
int updatePostsFile(users *usersHead)
{
    FILE *postsFile = fopen(POSTS_FILE_ADDRESS, "w");
    if (postsFile == NULL)
    {
        printf("Posts file can't be opened!\n\n");
        exit(1);
    }

    fprintf(postsFile, "*** Here is the format of lines : <post> <author> <number_of_likes> ***\n");

    users *usersNode = usersHead->nextUser; // we passed dummy node of usersList
    posts *postsNode;
    likers *likesNode;

    while (usersNode)
    {
        postsNode = usersNode->postsHead->nextPost; // we passed dummy node of postsList
        // calculate count of user's posts
        while (postsNode)
        {
            // calculate count of post's likes
            likesNode = postsNode->likersHead;
            int likesCount = -1; // we start from -1 because we don't count dummy node

            while (likesNode)
            {
                likesCount++;
                likesNode = likesNode->nextLiker;
            }

            fprintf(postsFile, "%s %s %d\n", postsNode->content, usersNode->username, likesCount);
            postsNode = postsNode->nextPost;
        }
        fprintf(postsFile, "\n");
        usersNode = usersNode->nextUser;
    }
    fclose(postsFile);

    return 0;
}
