#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define N 12

extern char **environ;

char *allowed[N] = {"cp","touch","mkdir","ls","pwd","cat","grep","chmod","diff","cd","exit","help"};

int getCommandIndex(const char*cmd);

int spawn(char* cmd[]);

int isAllowed(const char*cmd) {
	return getCommandIndex(cmd)!=-1;
}

int main() {

    // TODO
    // Add variables as needed

    char line[256];

    while (1) {

		fprintf(stderr,"rsh>");

		if (fgets(line,256,stdin)==NULL) continue;

		if (strcmp(line,"\n")==0) continue;

		line[strlen(line)-1]='\0';

		char* commandTokens[20]; 
		int tokenLength=-1;
		

		for (int i=0; i<20; i++){
			commandTokens[i] = strtok(i==0? line : NULL, " ");
			if (commandTokens[i]==NULL) {
				tokenLength=i;
				//printf("token: %i", tokenLength);
				break;
			}
		}



		//saftey check
		if (tokenLength==-1){
			printf("To many command line arguments were provided. Please limit it too 20\n");
			return 1;
		}
		//printf("cmd:%s\n", commandTokens[0]);
		int cmdIndex = getCommandIndex(commandTokens[0]);
		//printf("Index: %d\n", cmdIndex);

		if (cmdIndex>=0 && cmdIndex<9){
			spawn(commandTokens);
		}
		else if (cmdIndex==9){
			//cd
			if (tokenLength!=2){
				printf("-rsh: cd: too many arguments\n");
				continue;
			}

			chdir(commandTokens[1]);
		}

		else if (cmdIndex==10){
			break;
		}

		else if (cmdIndex==11){
			//help
			printf("The allowed commands are:\n");
			for (int i=0; i<N; i++){
				printf("%d: %s\n", i+1, allowed[i]);
			}
		}

		else{
			printf("NOT ALLOWED!\n");
		}

		// TODO
		// Add code to spawn processes for the first 9 commands
		// And add code to execute cd, exit, help commands
		// Use the example provided in myspawn.c

    }
    return 0;
}


int getCommandIndex(const char*cmd){
	for (int i=0; i<N; i++){
		//printf("CMP: %s -> %s \n", cmd, allowed[i]);
		if (strcmp(cmd, allowed[i])==0){
			return i;
		}
	}
	return -1;
}

int spawn(char* cmd[]){
	pid_t pid;
    //char *argv[] = {"echo", "Hello from the spawned process!", NULL};
    int status;
    posix_spawnattr_t attr;

    // Initialize spawn attributes
    posix_spawnattr_init(&attr);

    // Set flags if needed, for example, to specify the scheduling policy
    // posix_spawnattr_setflags(&attr, POSIX_SPAWN_SETSCHEDULER);

    // Spawn a new process
    if (posix_spawnp(&pid, cmd[0], NULL, &attr, cmd, environ) != 0) {
        perror("spawn failed");
        exit(EXIT_FAILURE);
    }

    // Wait for the spawned process to terminate
    if (waitpid(pid, &status, 0) == -1) {
        perror("waitpid failed");
        exit(EXIT_FAILURE);
    }

    // if (WIFEXITED(status)) {
    //     printf("Spawned process exited with status %d\n", WEXITSTATUS(status));
    // }

    // Destroy spawn attributes
    posix_spawnattr_destroy(&attr);

    return EXIT_SUCCESS;
}