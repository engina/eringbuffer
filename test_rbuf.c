#include <stdio.h>
#include <string.h>

#include "e_ringbuffer.h"

int main(void)
{
    FILE* test_cmds = fopen("test_commands.txt","rb");
    if(!test_cmds)
    {
        perror("fopen");
        return 1;
    }
    FILE* output = fopen("c_output.txt", "wb");    
    if(!output)
    {
        perror("fopen");
        return 1;
    }
    char cmd;
    int param;
    char test[5] = {0};
    fscanf(test_cmds, "%c:%d:%s\n", &cmd, &param, test);
    if(cmd != 'i')
    {
        perror("expecting i as first cmd\n");
        return 1;
    }
    char buf[(size_t)(param*1.5f+1)];
    char rbuf[(size_t)(param*1.5f+1)];
    ERingBuffer rb;
    printf("initing rb %d\n", param);
    e_ringbuffer_init(&rb, rbuf, param);
    memset(buf, 0, sizeof(buf));
    while(fscanf(test_cmds, "%c:%d:%s\n", &cmd, &param, buf) != EOF)
    {
        switch(cmd)
        {
            case 'r':
                if(1){}
                memset(buf,0,sizeof(buf));
                size_t n = e_ringbuffer_read(&rb, buf, param);
                fprintf(output, "r:%d:%s\n", n, buf);
            break;
            case 'w':
                fprintf(output, "w:%d:%s\n", e_ringbuffer_write(&rb, buf, param), buf);
            break;
            default:
                perror("unknown command\n");
                return 1;
        }
        memset(buf, 0, sizeof(buf));
    }
    return 0;
}
