/*
 *
 * Darian Marvel
 * 4/17/2022
 * Writing a shell for CS321
 *
 * Started from Stephen Brennan's tutorial:
 * https://brennan.io/2015/01/16/write-a-shell-in-c/
 *
*/

#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int cd_command(char** args);
int help_command(char** args);
int exit_command(char** args);
int print_command(char** args);

char* builtin_str[] = {
  "cd",
  "help",
  "print",
  "exit"
};

int (*builtin_func[]) (char **) = {
  &cd_command,
  &help_command,
  &print_command,
  &exit_command
};

int num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

/*
  Builtin function implementations.
*/

int cd_command(char** args) {
  if (args[1] == NULL) {
    fprintf(stderr, "cash: expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("cash");
    }
  }
  return 1;
}

int help_command(char** args) {
  int i;
  printf("Darian Marvel's CASH\n");
  printf("Type commands and their arguments, and hit enter.\n");
  printf("The following are built in:\n");

  for (i = 0; i < num_builtins(); i++) {
    printf("  %s\n", builtin_str[i]);
  }

  printf("Use the man command for information on other programs.\n");
  return 1;
}

int exit_command(char** args) {
  return 0;
}

int print_command(char** args)  {
  printf("Shell pid: %d\n", getpid());
  return 1;
}

int launch(char** args) {
  pid_t pid;
  int status;

  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp(args[0], args) == -1) {
      perror("cash");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    perror("cash");
  } else {
    // Parent process
    do {
      waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

int execute(char** args)
{
  int i;

  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }

  for (i = 0; i < num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }

  return launch(args);
}

char* read_line(void)
{
#ifdef CASH_USE_STD_GETLINE
  char* line = NULL;
  ssize_t bufsize = 0; // have getline allocate a buffer for us
  if (getline(&line, &bufsize, stdin) == -1) {
    if (feof(stdin)) {
      exit(EXIT_SUCCESS);  // We received an EOF
    } else  {
      perror("cash: getline\n");
      exit(EXIT_FAILURE);
    }
  }
  return line;
#else
#define CASH_RL_BUFSIZE 1024
  int bufsize = CASH_RL_BUFSIZE;
  int position = 0;
  char* buffer = malloc(sizeof(char) * bufsize);
  int c;

  if (!buffer) {
    fprintf(stderr, "cash: allocation error\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    // Read a character
    c = getchar();

    if (c == EOF) {
      exit(EXIT_SUCCESS);
    } else if (c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++;

    // If we have exceeded the buffer, reallocate.
    if (position >= bufsize) {
      bufsize += CASH_RL_BUFSIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "cash: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
#endif
}

#define CASH_TOK_BUFSIZE 64
#define CASH_TOK_DELIM " \t\r\n\a"

char** split_line(char* line)
{
  int bufsize = CASH_TOK_BUFSIZE, position = 0;
  char** tokens = malloc(bufsize * sizeof(char*));
  char* token, **tokens_backup;

  if (!tokens) {
    fprintf(stderr, "cash: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, CASH_TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += CASH_TOK_BUFSIZE;
      tokens_backup = tokens;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
		free(tokens_backup);
        fprintf(stderr, "cash: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, CASH_TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}

void loop(void)
{
  char* line;
  char** args;
  int status;

  do {
    printf("> ");
    line = read_line();
    args = split_line(line);
    status = execute(args);

    free(line);
    free(args);
  } while (status);
}

int main(int argc, char** argv)
{
  // Load config files, if any.

  // Run command loop.
  loop();

  // Perform any shutdown/cleanup.

  return EXIT_SUCCESS;
}
