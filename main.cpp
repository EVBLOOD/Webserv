#include "Server.hpp"
#include "ServerBuilder.hpp"
#include "ServerPoll.hpp"

int main() {
    Server server1 =
        ServerBuilder().set_port(8080).set_host(0x7f000001).build();
    Server server2 = ServerBuilder().set_port(8081).set_host(0x0).build();
    ServerPoll server_poll;
    server_poll.add_server(server1);
    server_poll.add_server(server2);
    server_poll.run_servers();
}
