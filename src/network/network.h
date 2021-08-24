#ifndef NETWORK_H
#define NETWORK_H

#include <QDataStream>
#include <QObject>
#include <QtNetWork>
#include <vector>

class Network : public QObject {
    typedef int SIZE_TYPE;
    Q_OBJECT

signals:
    // Emitted when a complete package received
    void package_get(const QByteArray&);

public slots:
    // Triggered when a new tcp packet is received
    void ReadPackage();

    void test_package_get(const QByteArray& package) {
        qDebug() << "[package_get]" << package;
    }

protected:
    QTcpSocket* socket_ = nullptr;
    SIZE_TYPE package_size_ = 0;

public:
    Network();
    virtual ~Network();

    // Send a QByteArray package
    void SendPackage(const QByteArray& data);
};

class Server : public Network {
    Q_OBJECT

private:
    QTcpServer* tcp_server_ = nullptr;
    bool is_connected_ = 0;

private slots:
    void OnConnected() { qDebug() << "[Server]Connected."; }
    void OnDisconnected() { qDebug() << "[Server]Disconnected."; }

    // Triggered when a new client connected
    void SetSocket();

public:
    Server();
    ~Server();

    // Get server IP
    std::vector<QString> GetIP();
    // Get server port
    qint16 GetPort() { return tcp_server_->serverPort(); }
};

class Client : public Network {
    Q_OBJECT

private slots:
    void OnConnected() { qDebug() << "[Client]Connected."; }
    void OnDisconnected() {
        qDebug() << "[Client]Disconnected.";
        qDebug() << "[ClientDisconnected]" << socket_->errorString();
    }

public:
    Client(const QString& ip, const qint16& port);
};

#endif // NETWORK_H
