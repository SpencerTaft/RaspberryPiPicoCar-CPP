#pragma once

#include "lwip/pbuf.h"
#include "lwip/tcp.h"

#define TCP_PORT 4242
#define DEBUG_printf printf
#define BUF_SIZE 2048
#define TEST_ITERATIONS 10
#define POLL_TIME_S 5

typedef struct TCP_SERVER_T_ {
    struct tcp_pcb *server_pcb;
    struct tcp_pcb *client_pcb;
    bool complete;
    uint8_t buffer_sent[BUF_SIZE];
    uint8_t buffer_recv[BUF_SIZE];
    int sent_len;
    int recv_len;
} TCP_SERVER_T;

class HttpServer {
    public:
        HttpServer() = default;
        ~HttpServer();
        void initAndRunServer(void);

    private:    
        static err_t closeServer();
        static err_t updateServerSent(void *arg, struct tcp_pcb *tpcb, u16_t len);
        static err_t receiveData(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
        static void serverError(void *arg, err_t err);
        static err_t onConnectionAccept(void *arg, struct tcp_pcb *client_pcb, err_t err);   
        static TCP_SERVER_T serverState;

        bool openServer();
        void runServer(void);

        
        
};
