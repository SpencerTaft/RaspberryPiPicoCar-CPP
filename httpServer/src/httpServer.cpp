#include "httpServer.hpp"

#include "pico/cyw43_arch.h"
#include "runtimeScheduler.hpp"

TCP_SERVER_T HttpServer::serverState = {};

HttpServer::~HttpServer()
{
    closeServer();
}

err_t HttpServer::closeServer() {
    printf("TCP_SERVER_CLOSE\n");
    err_t err = ERR_OK;
    if (serverState.client_pcb != NULL) {
        tcp_arg(serverState.client_pcb, NULL);
        tcp_poll(serverState.client_pcb, NULL, 0);
        tcp_sent(serverState.client_pcb, NULL);
        tcp_recv(serverState.client_pcb, NULL);
        tcp_err(serverState.client_pcb, NULL);
        err = tcp_close(serverState.client_pcb);
        if (err != ERR_OK) {
            printf("close failed %d, calling abort\n", err);
            tcp_abort(serverState.client_pcb);
            err = ERR_ABRT;
        }
        serverState.client_pcb = NULL;
    }
    if (serverState.server_pcb) {
        tcp_arg(serverState.server_pcb, NULL);
        tcp_close(serverState.server_pcb);
        serverState.server_pcb = NULL;
    }
    return err;
}

err_t HttpServer::updateServerSent(void *arg, struct tcp_pcb *tpcb, u16_t len) {
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

err_t HttpServer::receiveData(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    printf("TCP_SERVER_RECV\n");
    if (!p) {
        printf("TCP server received empty buffer\n");
        return ERR_ABRT;
    }
    // this method is callback from lwIP, so cyw43_arch_lwip_begin is not required, however you
    // can use this method to cause an assertion in debug mode, if this method is called when
    // cyw43_arch_lwip_begin IS needed
    cyw43_arch_lwip_check();
    if (p->tot_len > 0) {
        printf("tcp_server_recv %d/%d err %d\n", p->tot_len, serverState.recv_len, err);

        // Receive the buffer
        const uint16_t buffer_left = BUF_SIZE - serverState.recv_len;
        serverState.recv_len += pbuf_copy_partial(p, serverState.buffer_recv + serverState.recv_len,
                                             p->tot_len > buffer_left ? buffer_left : p->tot_len, 0);
        tcp_recved(tpcb, p->tot_len);
    }
    pbuf_free(p);

    // Have we have received the whole buffer
    if (serverState.recv_len > 0) {
        printf("tcp_server_recv buffer ok\n");
        
        //todo split data into ID and config
        Scheduler::updateConfig((char*)serverState.buffer_recv);
    }
    else{
        printf("TCP server failed to receive data\n");
        return ERR_ABRT;
    }
    return ERR_OK;
}

void HttpServer::serverError(void *arg, err_t err) {
    printf("TCP server error, closing server %d\n", err);
    closeServer();
}

err_t HttpServer::onConnectionAccept(void *arg, struct tcp_pcb *client_pcb, err_t err) {
    printf("TCP_SERVER_ACCEPT\n");

    serverState.client_pcb = client_pcb;
    //tcp_arg(client_pcb, state);
    tcp_sent(client_pcb, updateServerSent);
    tcp_recv(client_pcb, receiveData);
    tcp_err(client_pcb, serverError);

    return ERR_OK; //tcp_server_send_data(arg, state->client_pcb);
}

//this opens a TCP server on port 4242
bool HttpServer::openServer() {
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
    serverState.server_pcb = tcp_listen_with_backlog(pcb, 1); //listen for incoming connections, allow 1 pending
    if (!serverState.server_pcb) {
        printf("failed to listen\n");
        if (pcb) {
            tcp_close(pcb);
        }
        return false;
    }

    //tcp_arg(serverState.server_pcb); //state is arg to be passed to callbacks
    tcp_accept(serverState.server_pcb, onConnectionAccept); //sets function pointer to be called when a new connection is accepted

    return true;
}

void HttpServer::runServer(void) {
    printf("RUN_TCP_SERVER\n");

    if (!openServer()) {
        printf("failed to open server\n");
        return;
    }
    while(!serverState.complete) {
        sleep_ms(1000); //this can be blocking work that is being done
    }
}

void HttpServer::initAndRunServer() {
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
    runServer();
    cyw43_arch_deinit();
}