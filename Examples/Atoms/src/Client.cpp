//
// Created by Bogdan on 19.08.2024.
//

#include "Client.hpp"
#include <iostream>

namespace Nest {

    void Client::onAttach(const ServerData& serverData) {
        if (enet_initialize() != 0) {
            fprintf(stderr, "An error occurred while initializing ENet.\n");
            return;
        }

        m_client = nullptr;
        m_server = nullptr;
        m_client = enet_host_create(nullptr /* create a client host */,
                                  1 /* only allow 1 outgoing connection */,
                                  2 /* allow up 2 channels to be used, 0 and 1 */,
                                  0 /* assume any amount of incoming bandwidth */,
                                  0 /* assume any amount of outgoing bandwidth */);
        if (m_client == nullptr) {
            fprintf(stderr,
                    "An error occurred while trying to create an ENet client host.\n");
            exit(EXIT_FAILURE);
        }
        m_event.channelID = 0;

        ENetAddress address = { 0 };

        enet_address_set_host(&address, serverData.ip.c_str());
        address.port = serverData.port;

        /* Initiate the connection, allocating the two channels 0 and 1. */
        m_server = enet_host_connect(m_client, &address, 2, 0);
        if (m_server == nullptr) {
            fprintf(stderr,
                    "No available peers for initiating an ENet connection.\n");
            exit(EXIT_FAILURE);
        }
    }

    void Client::onUpdate() {
        while (enet_host_service(m_client, &m_event, 10) > 0) {
            if (m_event.type == ENET_EVENT_TYPE_CONNECT) {
                m_connected = true;
                std::cout << "Connected to server!" << std::endl;
                enet_host_flush(m_client);
            } else if (m_event.type == ENET_EVENT_TYPE_RECEIVE) {
                auto worldData = (PlayersData*)m_event.packet->data;
                memcpy(&m_worldData, worldData, sizeof(worldData->players) + sizeof(worldData->currentIndex));
                enet_packet_destroy(m_event.packet);
            } else if (m_event.type == ENET_EVENT_TYPE_DISCONNECT) {
                puts("Disconnection succeeded.");
                m_connected = false;
            }
        }
        if (!m_connected || m_worldData.players.size() == 0) {
            return;
        }
        auto &currPlayer = m_worldData.players[m_worldData.currentIndex];
        currPlayer.position += 0.1;
        printf("CLIENT: X: %f, Y: %f, Z: %f\n", currPlayer.position.x, currPlayer.position.y, currPlayer.position.z);
        sendData(&currPlayer, sizeof(currPlayer));
    }

    void Client::onDetach() {
        if (!m_connected) {
            enet_peer_reset(m_server);
        }
        enet_host_destroy(m_client);
        enet_deinitialize();
    }

    bool Client::serverValid() {
        if (m_connected) {
            return true;
        }
        static int timeoutServer = 100;
        return enet_host_service(m_client, &m_event, timeoutServer ) > 0;
    }

    void Client::sendData(const void* data, size_t size) {
        ENetPacket *packet = enet_packet_create(data, size, ENET_PACKET_FLAG_RELIABLE);

        enet_peer_send(m_server, 0, packet);
        enet_host_flush(m_server->host);
    }
}