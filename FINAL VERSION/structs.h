#ifndef STRUCTS
#define STRUCTS

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

#endif