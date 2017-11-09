#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>

void input(char *cmd);
void child(char *cmd);
int args(char *,char *,char *);
void pipe(char *,char *);
void in_redir(char *,char *,char *);
void out_redir(char *,char *,char *);

#define SIZE = 10;
int main()
{
        char buf[SIZE],cmd1[SIZE],cmd2[SIZE];
        int i,op;
        while(1)
        {
                input(buf);
                op = args(buf, cmd1, cmd2);
                switch(op)
                {
                        case '\0': child(cmd1);
                                   break;
                        case '|': pipe(cmd1, cmd2);
                                  break;
                        case '<': in_redir(buf, cmd1, cmd2);
                                  break;
                        case '>': out_redir(buf, cmd1, cmd2);
                                  break;
                }
        }
        return 0;
}       


void input(char* buf)
{
	int c;
	while((c = getchar())!='\n') 
			*buf++ = c;
	*buf = '\0';
}


int args(char *buf, char *cmd1, char *cmd2)
{
	int c ;
	int op;
	while((c = *buf++) != '|' && c != '>' && c != '<' && c != ' ' && c != '\0')
		*cmd1++ = c;
	*cmd1 = '\0';
    
    if(c == '\0') 
    {
		op = '\0';
		return op;
	}
    else if(c == ' ')
    {
		while((c = *buf++) == ' ');
    }
    
	if(c == '|' || c == '>' || c == '<') 
		op = c;
    
    while((c = *buf++) == ' ');
    while(c  != '\0') 
    {
		*cmd2++ = c;
		c = *buf++;
	}
	*cmd2 = '\0';
	return op;
}


void child(char *cmd)
{
    pid_t pid;
    int status;
	pid = fork();
	if(pid == 0)
		execlp(cmd, cmd, 0);
	else
		wait(&status);
}


void pipe(char *cmd1, char *cmd2)
{
	pid_t pid1, pid2;
	int fd[2],status;
	pipe(fd);
    if(pid1 == 0) 
    {
		close(1);
		dup(fd[1]);
		execlp(cmd1, cmd1, 0);
	}
    else
    {
		wait(0);
		close(fd[1]);
		pid2 = fork();
        if(pid2 == 0) 
        {
			close(0);
			dup(fd[0]);
			execlp(cmd2, cmd2, 0);
		}
		else
			wait(&status);
	}
}


void in_redir(char *buf, char *cmd1, char *cmd2)
{
	pid_t pid;
	int fd,status;
    if(pid == 0) 
    {
		fd = open(cmd2, O_RDONLY);
		close(0);
		dup(fd);
		execlp(cmd1, cmd1, 0);
	}
	else
		wait(&status);
}


void out_redir(char *buf, char *cmd1, char *cmd2)
{
	pid_t pid;
	int fd,status;
    if(pid == 0) 
    {
		fd = open(cmd2, O_WRONLY|O_CREAT|O_TRUNC, 0644);
		close(1);
		dup(fd);
		execlp(cmd1, cmd1, 0);
	}
	else
		wait(&status);
}




