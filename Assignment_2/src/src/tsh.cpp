#include "tsh.h"
#include <sys/wait.h>
#define DELIM " \t\r\n\a"


using namespace std;

void simple_shell::parse_command(char* cmd, char** cmdTokens) {
  // TODO: tokenize the command string into arguments
  int buffer_size, index;
	char *token;

  buffer_size = 64;
  index = 0;

	token = strtok(cmd, DELIM);
	while(token != NULL){
		cmdTokens[index] = token;
		index++;
		token = strtok(NULL, DELIM);

		if(index >= buffer_size){
			buffer_size += 64;
			cmdTokens = (char **) realloc(cmdTokens, buffer_size * sizeof(char*));
		}
	}

	cmdTokens[index] = NULL;
}

void simple_shell::exec_command(char **argv) {
	int status;
	pid_t pid;
	pid = fork();
	if (pid < 0){
		fprintf(stderr, "fork failed\n");
		exit(1);
	} else if (pid == 0) {
		execvp(argv[0], argv);
		sleep(1);
	} else {
    pid_t waitPID = waitpid(pid, &status, WUNTRACED);
    while (!WIFEXITED(status) && !WIFSIGNALED(status)) {
      waitPID = waitpid(pid, &status, WUNTRACED);
    }
	}
}

bool simple_shell::isQuit(char *cmd){
  // TODO: check for the command "quit" that terminates the shell
	if(strcmp(cmd, "quit") == 0) {
		return true;
    }
	return false;
}
