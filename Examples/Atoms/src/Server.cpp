//
// Created by Bogdan on 19.08.2024.
//

#include "Server.hpp"
#include <iostream>

namespace Nest {
void Server::onAttach(const ServerCreateInfo &serverCreateInfo) {
    m_serverWorldData.players.assign(maxClients, {});
    m_clients.assign(maxClients, nullptr);

    if (enet_initialize() != 0) {
        printf("An error occurred while initializing ENet.\n");
        return;
    }

    ENetAddress address = {0};

    address.host = ENET_HOST_ANY;
    address.port = serverCreateInfo.port;

    auto ip = getLocalIPAddresses()[1];

    /* create a server */
    m_server = enet_host_create(&address, maxClients, 2, 0, 0);

    if (m_server == nullptr) {
        printf("An error occurred while trying to create an ENet server host.\n");
        return;
    }
    printf("Your IP: %s\n", ip.c_str());

    printf("Started a server...\n");
}

void Server::onUpdate() {
    ENetEvent event;
    while (enet_host_service(m_server, &event, 10) > 0) {
        if (event.type == ENET_EVENT_TYPE_CONNECT) {
            for (int i = 0; i < maxClients; ++i) {
                if (!m_serverWorldData.players[i].isValid()) {
                    m_serverWorldData.players[i].m_id = i;
                    m_serverWorldData.players[i].playerData.color = glm::vec3(1);
                    m_serverWorldData.players[i].playerData.position = glm::vec3(0);
                    m_clients[i] = event.peer;
                    break;
                }
            }
        } else if (event.type == ENET_EVENT_TYPE_RECEIVE) {
            for (int i = 0; i < maxClients; ++i) {
                if (m_clients[i] == event.peer) {
                    auto playerData = (PlayerData *)event.packet->data;
                    auto &clientData = m_serverWorldData.players[i];
                    memcpy(&clientData.playerData, playerData, sizeof(PlayerData));
                    enet_packet_destroy(event.packet);
                    break;
                }
            }
        } else if (event.type == ENET_EVENT_TYPE_DISCONNECT ||
                   event.type == ENET_EVENT_TYPE_DISCONNECT_TIMEOUT) {
            for (int i = 0; i < maxClients; ++i) {
                if (event.peer == m_clients[i]) {
                    m_clients[i] = nullptr;
                    m_serverWorldData.players[i].m_id = ClientData::invalidId;
                    break;
                }
            }
            event.peer->data = nullptr;
        } else if (event.type == ENET_EVENT_TYPE_NONE) {
            /// ...
        }
    }

    PlayersData players;
    players.players.reserve(maxClients);
    int cntPlayers = 0;
    for (int i = 0; i < maxClients; ++i) {
        if (m_serverWorldData.players[i].isValid()) {
            auto playerData = m_serverWorldData.players[i].playerData;
            players.players.emplace_back(playerData);
            cntPlayers++;
            printf(
                "SERVER: X: %f, Y: %f, Z: %f\n",
                playerData.position.x,
                playerData.position.y,
                playerData.position.z
            );
        }
    }
    printf("Count Players: %d\n", cntPlayers);
    for (int i = 0; i < maxClients; ++i) {
        if (m_serverWorldData.players[i].isValid()) {
            players.currentIndex = m_serverWorldData.players[i].m_id;
            sendData(&players, sizeof(players), m_clients[i]);
        }
    }
}

void Server::onDetach() {
    for (int i = 0; i < maxClients; ++i) {
        enet_peer_reset(m_clients[i]);
    }
    enet_host_destroy(m_server);
    enet_deinitialize();
}

void Server::sendData(const void *data, size_t size, ENetPeer *client) {
    ENetPacket *packet = enet_packet_create(data, size, ENET_PACKET_FLAG_RELIABLE);

    enet_peer_send(client, 0, packet);
    enet_host_flush(client->host);
}
} // namespace Nest