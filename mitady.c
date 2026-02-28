#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

void putnbr(int n)
{
    char c;
    if (n >= 10)
        putnbr(n / 10);
    c = (n % 10) + '0';
    write(1, &c, 1);
}

int main(int argc, char *argv[])
{
    int opt;
    int fd;
    char buf[1024];
    ssize_t n;

    while ((opt = getopt(argc, argv, "f:d:")) != -1)
    {
        if (optind >= argc)
        {
            write(1, "Missing search word\n", 20);
            return 1;
        }

        char *word = argv[optind];

        switch (opt)
        {
            case 'f':

                fd = open(optarg, O_RDONLY);
                if (fd == -1)
                {
                    write(1, "Error opening file\n", 19);
                    return 1;
                }

                int count = 0;

                while ((n = read(fd, buf, sizeof(buf) - 1)) > 0)
                {
                    buf[n] = '\0';

                    char *ptr = buf;

                    while ((ptr = strstr(ptr, word)) != NULL)
                    {
                        count++;
                        ptr++;
                    }
                }

                close(fd);

                write(1, "File: ", 6);
                write(1, optarg, strlen(optarg));
                write(1, " -> ", 4);
                putnbr(count);
                write(1, " occurrences\n", 13);

                break;

            case 'd':

                DIR *dir = opendir(optarg);
                if (!dir)
                {
                    write(1, "Error opening directory\n", 24);
                    return 1;
                }

                struct dirent *entry;

                while ((entry = readdir(dir)) != NULL)
                {
                    if (entry->d_type == DT_REG)
                    {
                        char path[1024];

                        strcpy(path, optarg);
                        strcat(path, "/");
                        strcat(path, entry->d_name);

                        fd = open(path, O_RDONLY);
                        if (fd == -1)
                            continue;

                        int count_d = 0;

                        while ((n = read(fd, buf, sizeof(buf) - 1)) > 0)
                        {
                            buf[n] = '\0';

                            char *ptr = buf;

                            while ((ptr = strstr(ptr, word)) != NULL)
                            {
                                count_d++;
                                ptr++;
                            }
                        }

                        close(fd);

                        if (count_d > 0)
                        {
                            write(1, "File: ", 6);
                            write(1, path, strlen(path));
                            write(1, " -> ", 4);
                            putnbr(count_d);
                            write(1, " occurrences\n", 13);
                        }
                    }
                }

                closedir(dir);

                break;

            default:
                write(1, "Invalid option\n", 15);
                return 1;
        }
    }

    return 0;
}
