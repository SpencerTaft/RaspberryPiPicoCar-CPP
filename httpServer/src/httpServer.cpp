#include "httpServer.hpp"

// /**
//  * Copyright (c) 2022 Raspberry Pi (Trading) Ltd.
//  *
//  * SPDX-License-Identifier: BSD-3-Clause
//  */

#include <string.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

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

static err_t tcp_server_close(void *arg) {
    printf("TCP_SERVER_CLOSE\n");
    TCP_SERVER_T *state = (TCP_SERVER_T*)arg;
    err_t err = ERR_OK;
    if (state->client_pcb != NULL) {
        tcp_arg(state->client_pcb, NULL);
        tcp_poll(state->client_pcb, NULL, 0);
        tcp_sent(state->client_pcb, NULL);
        tcp_recv(state->client_pcb, NULL);
        tcp_err(state->client_pcb, NULL);
        err = tcp_close(state->client_pcb);
        if (err != ERR_OK) {
            printf("close failed %d, calling abort\n", err);
            tcp_abort(state->client_pcb);
            err = ERR_ABRT;
        }
        state->client_pcb = NULL;
    }
    if (state->server_pcb) {
        tcp_arg(state->server_pcb, NULL);
        tcp_close(state->server_pcb);
        state->server_pcb = NULL;
    }
    return err;
}

static err_t tcp_server_sent(void *arg, struct tcp_pcb *tpcb, u16_t len) {
    printf("TCP_SERVER_SENT\n");
    TCP_SERVER_T *state = (TCP_SERVER_T*)arg;
    printf("tcp_server_sent %u\n", len);
    state->sent_len += len;

    if (state->sent_len >= BUF_SIZE) {

        // We should get the data back from the client
        state->recv_len = 0;
        printf("Waiting for buffer from client\n");
    }

    return ERR_OK;
}

//Omitted for now, until I can validate data received from client
// err_t tcp_server_send_data(void *arg, struct tcp_pcb *tpcb)
// {
//     printf("TCP_SERVER_SEND_DATA\n");
//     TCP_SERVER_T *state = (TCP_SERVER_T*)arg;
//     for(int i=0; i< BUF_SIZE; i++) {
//         state->buffer_sent[i] = rand();
//     }

//     state->sent_len = 0;
//     printf("Writing %ld bytes to client\n", BUF_SIZE);
//     // this method is callback from lwIP, so cyw43_arch_lwip_begin is not required, however you
//     // can use this method to cause an assertion in debug mode, if this method is called when
//     // cyw43_arch_lwip_begin IS needed
//     cyw43_arch_lwip_check();
//     err_t err = tcp_write(tpcb, state->buffer_sent, BUF_SIZE, TCP_WRITE_FLAG_COPY);
//     if (err != ERR_OK) {
//         printf("Failed to write data %d\n", err);
//         return tcp_server_result(arg, -1);
//     }
//     return ERR_OK;
// }

err_t tcp_server_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    printf("TCP_SERVER_RECV\n");
    TCP_SERVER_T *state = (TCP_SERVER_T*)arg;
    if (!p) {
        printf("TCP server received empty buffer\n");
        return ERR_ABRT;
    }
    // this method is callback from lwIP, so cyw43_arch_lwip_begin is not required, however you
    // can use this method to cause an assertion in debug mode, if this method is called when
    // cyw43_arch_lwip_begin IS needed
    cyw43_arch_lwip_check();
    if (p->tot_len > 0) {
        printf("tcp_server_recv %d/%d err %d\n", p->tot_len, state->recv_len, err);

        ////>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        struct pbuf *current_pbuf = p;
        while (current_pbuf != NULL) {
            printf("received data:");
            for (int i = 0; i < current_pbuf->len; i++) {
                printf("%c", ((uint8_t *)current_pbuf->payload)[i]);
            }
            current_pbuf = current_pbuf->next;
        }
        ///<<<<<<<<<<<<<<<<<<<<<<<<<<

        // Receive the buffer
        const uint16_t buffer_left = BUF_SIZE - state->recv_len;
        state->recv_len += pbuf_copy_partial(p, state->buffer_recv + state->recv_len,
                                             p->tot_len > buffer_left ? buffer_left : p->tot_len, 0);
        tcp_recved(tpcb, p->tot_len);
    }
    pbuf_free(p);

    // Have we have received the whole buffer
    if (state->recv_len > 0) {
        printf("tcp_server_recv buffer ok\n");
        //todo send data to other core
    }
    else{
        printf("TCP server failed to receive data\n");
        //todo do not return ERR_OK
    }
    return ERR_OK;
}

static void tcp_server_err(void *arg, err_t err) {
    printf("TCP server error, closing server %d\n", err);
    tcp_server_close(arg);
}

static err_t tcp_server_accept(void *arg, struct tcp_pcb *client_pcb, err_t err) {
    printf("TCP_SERVER_ACCEPT\n");
    TCP_SERVER_T *state = (TCP_SERVER_T*)arg;
    if (err != ERR_OK || client_pcb == NULL) {
        printf("Failure in accept\n");
        return ERR_VAL;
    }
    printf("Client connected\n");

    state->client_pcb = client_pcb;
    tcp_arg(client_pcb, state);
    tcp_sent(client_pcb, tcp_server_sent);
    tcp_recv(client_pcb, tcp_server_recv);
    tcp_err(client_pcb, tcp_server_err);

    return ERR_OK; //tcp_server_send_data(arg, state->client_pcb);
}

//this opens a TCP server on port 4242
static bool tcp_server_open(TCP_SERVER_T* state) {
    printf("TCP_SERVER_OPEN\n");
    printf("Starting server at %s on port %u\n", ip4addr_ntoa(netif_ip4_addr(netif_list)), TCP_PORT);

    //tcp_pcb (TCP protocol control block) manages state and control for a TCP connection
    struct tcp_pcb *pcb = tcp_new_ip_type(IPADDR_TYPE_ANY);
    if (!pcb) {
        printf("failed to create pcb\n");
        return false;
    }

    err_t err = tcp_bind(pcb, NULL, TCP_PORT); //binds the pcb to a local IP address and port
    if (err) {
        printf("failed to bind to port %u\n", TCP_PORT);
        return false;
    }

    //assigning new pcb to the server state
    state->server_pcb = tcp_listen_with_backlog(pcb, 1); //listen for incoming connections, allow 1 pending
    if (!state->server_pcb) {
        printf("failed to listen\n");
        if (pcb) {
            tcp_close(pcb);
        }
        return false;
    }

    tcp_arg(state->server_pcb, state); //state is arg to be passed to callbacks
    tcp_accept(state->server_pcb, tcp_server_accept); //sets function pointer to be called when a new connection is accepted

    return true;
}

void run_tcp_server(void) {
    printf("RUN_TCP_SERVER\n");
    TCP_SERVER_T *serverState = (TCP_SERVER_T *)calloc(1, sizeof(TCP_SERVER_T));
    if (!serverState) {
        printf("failed to allocate state\n");
        return;
    }

    if (!tcp_server_open(serverState)) {
        printf("failed to open server\n");
        free(serverState); //todo use a smart pointer for better memory management
        return;
    }
    while(!serverState->complete) {
        sleep_ms(1000); //this can be blocking work that is being done
    }
    free(serverState);
}

void testWifi() {
    if (cyw43_arch_init()) {
        printf("failed to initialise\n");
        return;
    }

    cyw43_arch_enable_sta_mode();

    printf("Connecting to Wi-Fi...\n");
    if (cyw43_arch_wifi_connect_timeout_ms("Cactus transplants", PICOHTTPS_WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("failed to connect.\n");
        return;
    } else {
        printf("Connected.\n");
    }
    run_tcp_server();
    cyw43_arch_deinit();
}