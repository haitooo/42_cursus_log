#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define ENV_BUFFER_SIZE 8192  // バッファサイズ（必要なら増やす）

int main(void) {
    int fd = open("/proc/self/environ", O_RDONLY);
    if (fd == -1) {
        perror("open");
        return EXIT_FAILURE;
    }

    char buffer[ENV_BUFFER_SIZE];
    ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    close(fd);

    if (bytes_read <= 0) {
        perror("read");
        return EXIT_FAILURE;
    }

    buffer[bytes_read] = '\0';  // 安全のため末尾をヌル終端

    // `/proc/self/environ` は `\0` 区切りの文字列なので `strtok` を使用
    char *env = buffer;
    while (env < buffer + bytes_read) {
        if (strncmp(env, "PATH=", 5) == 0) {
            printf("PATH found: %s\n", env);
            return 0;
        }
        env += strlen(env) + 1;  // 次の環境変数へ移動（`+1` で `\0` をスキップ）
    }

    printf("PATH not found\n");
    return EXIT_FAILURE;
}
