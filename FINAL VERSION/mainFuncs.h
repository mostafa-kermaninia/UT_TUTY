#include "structs.h"

int signUp(char *username, char *password, users *usersHead);
int logIn(char *username, char *password, users *usersHead);
int post(users *currentUser, users *usersHead);
int like(char *postOwnerusername, int postId, users *usersHead, int currentUserId);
void logout();
int deletePost(int postId, users *currentUser, users *usersHead);
void info(int Id, users *usersHead);
int find_user(char *username, users *usersHead);
