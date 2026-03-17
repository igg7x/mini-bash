#ifndef UTILS_H
#define UTILS_H

#define MAX_ARGS 64
#define MAX_LINE 1024
#define PATH_MAX 4096

void trim_whitespace(char *str);
void change_directory(char *path);
int find_token(char **args, const char *token);
int execute_redirect_file(char **args);
int execute_redirect_pipes(char **args);
int execute_simple_command(char **args);
int execute_command(char **args);

#endif // UTILS_H
