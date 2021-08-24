#ifndef NETWORK_H
#define NETWORK_H

#include <QDataStream>
#include <QObject>
#include <QTimer>
#include <QtNetWork>
#include <vector>

class Network : public QObject {
    typedef int SIZE_TYPE;
    Q_OBJECT

signals:
    // Successfully connected
    void SuccessConnection();
    // Disconnect
    void Disconnect();
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
    void OnDisconnected() {
        qDebug() << "[Server]Disconnected.";
        emit Disconnect();
    }

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
        qDebug() << "[Client Disconnected]" << socket_->errorString();
        emit Disconnect();
    }

    // Check whether connection is ok to emit SuccessConnection
    void CheckConnect(const QByteArray& package);

public:
    // Connection time limit
    int timeout_;
    QTimer* timer;

    explicit Client(const int& timeout = 5000);
    ~Client();

    // Try to connect
    void TryConnect(const QString& ip, const qint16& port);
};

#endif // NETWORK_H
