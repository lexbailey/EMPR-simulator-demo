#include <LPC17xx.h>

char *test_socket_file = "./.lpc17_testing_socket";

void test_socket_send(char *str){
    ssize_t len = strlen(str);
    ssize_t written = 0;
    while (written < len){
        written += write(test_socket_fd, str+written, len-written);
        if (written < 0){
            perror("Socket failure");
            exit(1);
        }
    }
}

char next_test_socket_char(){
    char c;
    if (read(test_socket_fd, &c, 1)==0){
        fprintf(stderr, "End of data\n");
        exit(1);
    }
    return c;
}

char *test_socket_recv(){
    const int chunk_size = 20;
    char *reply = malloc(chunk_size);
    char *reply_pos = reply;
    size_t reply_len = chunk_size;
    int done = 0;
    while (!done){
        char next = next_test_socket_char();
        if (next == '\n'){
            next = '\0';
            done = 1;
        }
        *reply_pos++ = next;
        if (reply_pos > reply + reply_len){
            size_t new_len = reply_len + chunk_size;
            reply = realloc(reply, new_len);
            reply_pos = reply + reply_len;
            reply_len = new_len;
        }
    }
    return reply;
}

void init_test_env(){
    printf("Init test env...\n");
    char *env_socket_name = getenv("LPC17XX_TESTING_SOCKET_NAME");
    if (env_socket_name == NULL){
        printf("Using default socket file name: %s\n", test_socket_file);
    }
    else{
        test_socket_file = env_socket_name;
        printf("Using environment socket file name: %s\n", test_socket_file);
    }

    if (*test_socket_file == '\0'){
        fprintf(stderr, "Socket file name invalid");
    }


    if ((test_socket_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1){
        perror("Socket error");
        exit(1);
    }
  
    memset(&test_socket_addr, 0, sizeof(test_socket_addr));
    test_socket_addr.sun_family = AF_UNIX;
    strncpy(test_socket_addr.sun_path, test_socket_file, sizeof(test_socket_addr.sun_path)-1);

    if (connect(test_socket_fd, (struct sockaddr*)&test_socket_addr, sizeof(test_socket_addr)) == -1){
        perror("Connect error");
        exit(1);
    }
  
    test_socket_send("START\n");

    printf("Done env init\n\n");
}
