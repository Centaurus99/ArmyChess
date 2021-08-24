#include "network.h"
#include <exception>
#include <string>

Network::Network() {
    connect(this, &Network::package_get, this, &Network::test_package_get);
}

void Network::SendPackage(const QByteArray& data) {
    qDebug() << "[Post]";
    QByteArray package;
    QDataStream stream(&package, QIODevice::ReadWrite);
    stream << (SIZE_TYPE)(0) << data;
    stream.device()->seek(0);
    stream << (SIZE_TYPE)(package.size() - sizeof(SIZE_TYPE));
    socket_->write(package);
}

void Network::ReadPackage() {
    qDebug() << "[Get]Received One Package";
    while (true) {
        if (package_size_ == 0) {
            if (socket_->bytesAvailable() < sizeof(SIZE_TYPE))
                break;
            QByteArray size;
            size.resize(sizeof(SIZE_TYPE));
            socket_->read(size.data(), sizeof(SIZE_TYPE));
            QDataStream size_stream(size);
            size_stream >> package_size_;
            qDebug() << "[Get]size: " << package_size_;
        } else {
            if (socket_->bytesAvailable() < package_size_)
                break;
            QDataStream stream(socket_);
            QByteArray data;
            stream >> data;
            emit package_get(data);
            package_size_ = 0;
        }
    }
    qDebug() << "[Get]End with " << socket_->bytesAvailable()
             << "bytes remaining";
    qDebug() << "[Get]Now package_size_: " << package_size_;
}

void Server::SetSocket() {
    qDebug() << "[SetSocket]TryConnect";
    if (is_connected_) {
        qDebug() << "[SetSocket]Reconnected!";
        return;
    }
    socket_ = tcp_server_->nextPendingConnection();
    tcp_server_->close();
    if (socket_ == nullptr)
        qDebug() << "[SetSocket]Null!!!";
    is_connected_ = 1;
    connect(socket_, &QTcpSocket::connected, this, &Server::OnConnected);
    connect(socket_, &QTcpSocket::readyRead, this, &Network::ReadPackage);
    connect(socket_, &QTcpSocket::disconnected, this, &Server::OnDisconnected);
}

Server::Server() {
    tcp_server_ = new QTcpServer(this);
    tcp_server_->setMaxPendingConnections(1);
    if (!tcp_server_->listen(QHostAddress::AnyIPv4, 7070)) {
        qDebug() << "[Server]" << tcp_server_->errorString();
        std::string error = tcp_server_->errorString().toStdString();
        delete tcp_server_;
        tcp_server_ = nullptr;
        throw std::runtime_error(error);
    }
    connect(tcp_server_, &QTcpServer::newConnection, this, &Server::SetSocket);
}

Server::~Server() { delete tcp_server_; }

std::vector<QString> Server::GetIP() {
    QString localHostName = QHostInfo::localHostName();
    QHostInfo info = QHostInfo::fromName(localHostName);
    std::vector<QString> servIP;
    foreach (QHostAddress address, info.addresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol) {
            qDebug() << "[GetIP]" << address;
            servIP.push_back(address.toString());
        }
    }
    return servIP;
}

Client::Client(const QString& ip, const qint16& port) {
    socket_ = new QTcpSocket(this);
    connect(socket_, &QTcpSocket::connected, this, &Client::OnConnected);
    connect(socket_, &QTcpSocket::readyRead, this, &Network::ReadPackage);
    connect(socket_, &QTcpSocket::disconnected, this, &Client::OnDisconnected);
    qDebug() << "[Client]"
             << "BeginConnect.";
    socket_->connectToHost(ip, port);
}
