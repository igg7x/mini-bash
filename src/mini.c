#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include "utils.h"

int main()
{
    char line[MAX_LINE];
    char *args[MAX_ARGS];

    while (1)
    {
        char cwd[PATH_MAX];

        if (getcwd(cwd, sizeof(cwd)) == NULL)
            strcpy(cwd, "?");

        printf("\033[1;32m@%s\033[0m mini-bash> ", cwd);

        if (fgets(line, sizeof(line), stdin) == NULL)
            break;

        trim_whitespace(line);
        line[strcspn(line, "\n")] = 0;
        if (strlen(line) == 0)
            continue;

        int i = 0;
        args[i] = strtok(line, " ");
        while (args[i] != NULL && i < MAX_ARGS - 1)
        {
            i++;
            args[i] = strtok(NULL, " ");
        }

        if (args[0] == NULL)
            continue;

        if (strcmp(args[0], "exit") == 0)
            break;

        if (strcmp(args[0], "cd") == 0)
        {
            change_directory(args[1]);
            continue;
        }

        execute_command(args);
    }

    printf("Saliendo de mini-bash...\n");
    return 0;
}
