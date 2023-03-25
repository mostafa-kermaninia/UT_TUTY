#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "addStructsNode.h"

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
