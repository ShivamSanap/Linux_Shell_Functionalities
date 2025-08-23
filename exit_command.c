#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

char **tokenize(char *line)
{
	char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
	char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
	int i, tokenIndex = 0, tokenNo = 0;

	for(i =0; i < strlen(line); i++)
	{
		char readChar = line[i];
		if (readChar == ' ' || readChar == '\n' || readChar == '\t')
		{
			token[tokenIndex] = '\0';
			if (tokenIndex != 0)
			{
				tokens[tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
				strcpy(tokens[tokenNo++], token);
				tokenIndex = 0; 
			}
		} 
		else 
		{
			token[tokenIndex++] = readChar;
		}
	}
	
	free(token);
	tokens[tokenNo] = NULL ;
	return tokens;
}

int main(int argc, char* argv[]) 
{
	char  line[MAX_INPUT_SIZE];            
	char  **tokens;              
	int i;

	while(1) 
	{			
		bzero(line, sizeof(line));

		char cwd[256];
		if (getcwd(cwd, sizeof(cwd)) != NULL) {
    		printf("%s $ ", cwd);
		} else {
			perror("getcwd error");
			printf("$ ");
		}

		scanf("%[^\n]", line);
		getchar();

		printf("Command entered: %s (remove this debug output later)\n", line);

		line[strlen(line)] = '\n'; 
		tokens = tokenize(line);

        while(waitpid(-1, NULL, WNOHANG)>0)
        {
            printf("bg process reaped");
        }

		int count=0;

        for(i=0;tokens[i]!=NULL;i++)
        {
            printf("found token %s (remove this debug output later)\n", tokens[i]);
            count+=1;
        }
        printf("%d\n", count);

        if(strcmp(tokens[0],"exit")==0)
        {
            break;
        }
   
		if(strcmp((tokens[count-1]),"&")==0)
        {
            tokens[count-1]=NULL;
            if (tokens[0] != NULL && strcmp(tokens[0], "cd") == 0)
            {
                if (tokens[1] == NULL)
                {
                    fprintf(stderr, "cd: missing argument\n");
                }
                else if (tokens[2] != NULL)
                {
                    fprintf(stderr, "cd: too many arguments\n");
                }
                else
                {
                    if (chdir(tokens[1]) != 0)
                    {
                        perror("cd failed");
                    }
                }
                continue;
            }
            int pid = fork();
            if (pid < 0)
            {
                perror("Fork failed");
                exit(1);
            }

            else if(pid == 0)
            {
                execvp(tokens[0], tokens);
                perror("exec failed");
                exit(1);
            }
        }
        else
        {
            if (tokens[0] != NULL && strcmp(tokens[0], "cd") == 0)
            {
                if (tokens[1] == NULL)
                {
                    fprintf(stderr, "cd: missing argument\n");
                }
                else if (tokens[2] != NULL)
                {
                    fprintf(stderr, "cd: too many arguments\n");
                }
                else
                {
                    if (chdir(tokens[1]) != 0)
                    {
                        perror("cd failed");
                    }
                }
                continue;
            }
           
            int pid = fork();

            if (pid < 0)
            {
                perror("Fork failed");
                exit(1);
            }

            else if(pid == 0)
            {
                execvp(tokens[0], tokens);
                perror("exec failed");
                exit(1);
            }

            else
            {
                int status;
                wait(&status);
                if (WIFEXITED(status))
                {
                    int exit_code = WEXITSTATUS(status);
                        if (exit_code == 1)
                        {
                            printf("EXITSTATUS: %d\n", exit_code);
                        }
                }
            }
        }
		
		for(i=0;tokens[i]!=NULL;i++)
		{
			free(tokens[i]);
		}
		free(tokens);
	}
	return 0;
}