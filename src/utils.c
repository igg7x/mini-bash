#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "utils.h"

void trim_whitespace(char *str)
{
    char *end;
    while (*str == ' ')
        str++;
    if (*str == 0)
        return;
    end = str + strlen(str) - 1;
    while (end > str && *end == ' ')
        end--;
    *(end + 1) = 0;
}

void change_directory(char *path)
{
    if (path == NULL)
    {
        if (chdir(getenv("HOME")) != 0)
        {
            perror("Error al cambiar de directorio");
        }
    }
    else
    {
        if (chdir(path) != 0)
        {
            perror("Error al cambiar de directorio");
        }
    }
}

int find_token(char **args, const char *token)
{
    int i = 0;
    while (args[i] != NULL)
    {
        if (strcmp(args[i], token) == 0)
        {
            return i;
        }
        i++;
    }
    return -1;
}

int execute_redirect_file(char **args)
{
    int idx = find_token(args, ">");
    if (idx < 0)
        return 0;

    if (args[idx + 1] == NULL)
    {
        fprintf(stderr, "Error: No se especificó un archivo para redirección\n");
        return 1;
    }

    char *filename = args[idx + 1];
    args[idx] = NULL;
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("Error al crear el proceso hijo");
        return 1;
    }
    if (pid == 0)
    {
        int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0)
        {
            perror("Error al abrir el archivo para redirección");
            exit(EXIT_FAILURE);
        }
        if (dup2(fd, STDOUT_FILENO) < 0)
        {
            perror("Error al redirigir salida");
            close(fd);
            exit(EXIT_FAILURE);
        }
        close(fd);
        execvp(args[0], args);
        perror("Error en exec");
        exit(EXIT_FAILURE);
    }
    waitpid(pid, NULL, 0);
    return 1;
}

int execute_redirect_pipes(char **args)
{
    int idx = find_token(args, "|");
    if (idx < 0)
        return 0;

    args[idx] = NULL;
    char **left_args = args;
    char **right_args = &args[idx + 1];

    if (left_args[0] == NULL || right_args[0] == NULL)
    {
        fprintf(stderr, "Error: comando de tubería incompleto\n");
        return 1;
    }

    int pipefd[2];
    if (pipe(pipefd) == -1)
    {
        perror("Error al crear la tubería");
        return 1;
    }

    pid_t pid_child1 = fork();
    if (pid_child1 < 0)
    {
        perror("Error al crear el primer proceso hijo");
        close(pipefd[0]);
        close(pipefd[1]);
        return 1;
    }

    if (pid_child1 == 0)
    {
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);
        execvp(left_args[0], left_args);
        perror("Comando no encontrado");
        exit(EXIT_FAILURE);
    }

    pid_t pid_child2 = fork();
    if (pid_child2 < 0)
    {
        perror("Error al crear el segundo proceso hijo");
        close(pipefd[0]);
        close(pipefd[1]);
        waitpid(pid_child1, NULL, 0);
        return 1;
    }

    if (pid_child2 == 0)
    {
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[1]);
        close(pipefd[0]);
        execvp(right_args[0], right_args);
        perror("Comando no encontrado");
        exit(EXIT_FAILURE);
    }

    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(pid_child1, NULL, 0);
    waitpid(pid_child2, NULL, 0);
    return 1;
}

int execute_simple_command(char **args)
{
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("Error al crear el proceso hijo");
        return 1;
    }
    if (pid == 0)
    {
        execvp(args[0], args);
        perror("Comando no encontrado");
        exit(EXIT_FAILURE);
    }

    int status;
    waitpid(pid, &status, 0);
    return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
}

int execute_command(char **args)
{
    if (args == NULL || args[0] == NULL)
        return 1;

    if (find_token(args, "|") >= 0)
        return execute_redirect_pipes(args);

    if (find_token(args, ">") >= 0)
        return execute_redirect_file(args);

    return execute_simple_command(args);
}
