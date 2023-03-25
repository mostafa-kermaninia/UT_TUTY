#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "findUser.h"

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