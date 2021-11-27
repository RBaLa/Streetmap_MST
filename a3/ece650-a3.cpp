#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

int main(int argc, char** argv){
    
    pid_t child_rgen,child_a1,child_a2;
    int pipe1[2],pipe2[2];
    pipe(pipe1);
    pipe(pipe2);
    
    child_rgen = fork();
    if (child_rgen==0){
        int dup2check1 = dup2(pipe1[1],STDOUT_FILENO);
        cerr<<"dup2 check rgen:"<<dup2check1<<endl;
        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[0]);
        close(pipe2[1]);
        execv("./rgen",argv);
    }
    else{
        child_a1 = fork();
        if (child_a1==0){
            int dup2check2 = dup2(pipe1[0],STDIN_FILENO);
            cerr<<"dup2 check a1 read:"<<dup2check2<<endl;
            close(pipe1[1]);
            close(pipe1[0]);
            int dup2check3 = dup2(pipe2[1],STDOUT_FILENO);
            cerr<<"dup2 check a1 write:"<<dup2check3<<endl;
            close(pipe2[0]);
            close(pipe2[1]);
            execlp("python","python","ece650-a1.py",(char*)NULL);
        }
        else{
            child_a2 = fork();
            if (child_a2==0){
                int dup2check4 = dup2(pipe2[0],STDIN_FILENO);
                cerr<<"dup2 check a2:"<<dup2check4<endl;
                close(pipe2[1]);
                close(pipe2[0]);
                close(pipe1[1]);
                close(pipe1[0]);
                execlp("./ece650-a2",(char*)NULL);
            }
            else{
                dup2(pipe2[1],STDOUT_FILENO);
                close(pipe1[0]);
                close(pipe1[1]);
                close(pipe2[0]);
                close(pipe2[1]);
                string line;
                while (!cin.eof()){
                    getline(cin,line);
                    cout<<line<<endl;
                    if (cin.eof())
                        break;
                }
                kill(child_a1,SIGTERM);
                kill(child_a2,SIGTERM);
                kill(child_rgen,SIGTERM);
            }
        }
    }
    return 0;
}
