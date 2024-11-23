#pragma once

class StartMenu {
public:
    void showStartMenu();
    inline bool isClient() const {
        return m_isClient;
    }
    inline bool isServer() const {
        return m_isServer;
    }

private:
    bool m_isServer, m_isClient;
};
