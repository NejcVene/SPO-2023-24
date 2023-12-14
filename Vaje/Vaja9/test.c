#include <stdio.h>

int main(void)
{
        FILE* p = popen("grep -n int server.c | cut -d: -f1", "r");
        if (!p) return 1;

        char buff[1024];
        while (fgets(buff, sizeof(buff), p)) {
                printf("%s", buff);
        }
        pclose(p);

        return 0;
}