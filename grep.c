#include <stddef.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]){
    int fd =  open(argv[1], O_RDONLY, 0666);

    if(argc < 2){
        char msgerror[] = "error: couldn't argumante";
        size_t size = sizeof(msgerror);
        write(1,msgerror,  size);
        return 1;
    }
    
   


    return 0;
}