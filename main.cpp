#include "Server.hpp"
#include "ServerBuilder.hpp"
#include "ServerPoll.hpp"

int main() {
    Server s1 =
            ServerBuilder().set_port(8080).set_host("127.0.0.1").build();
    Server s2 = ServerBuilder().set_port(8080).set_host("0.0.0.0").build();
    ServerPoll().add_server(s1).add_server(s2).run_servers();
}
