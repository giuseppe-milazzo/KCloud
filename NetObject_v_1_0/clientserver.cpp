#include "clientserver.h"
#include <QDebug>

ClientServer::ClientServer(QObject *parent) : QThread(parent), channel(new QTcpSocket(this)), res(new KCloud::Resource(this)){

}

MainServer::MainServer(QObject *parent) : QTcpServer(parent){
}

void MainServer::incomingConnection(qintptr sock){

	qDebug() << "Nuova connessione accettata!";
	threads << new WorkerServer(sock, this);
	threads.last()->start();
}

WorkerServer::WorkerServer(int fd, QObject *parent) : ClientServer(parent){

	channel->setSocketDescriptor(fd);
	qDebug() << "Thread costruito!";
	res->setZipDir("/home/giuseppe/Scrivania");
	res->setZipName("testTrasmissione1");
	res->prepareForRecv();
	res->receiveFrom(channel);
	connect(channel, SIGNAL(readyRead()), this, SLOT(notificaBytes()));
	connect(res, SIGNAL(objectReceived()), this, SLOT(notificaRicezione()));
	qDebug() << "Connect eseguite!";
}

void WorkerServer::notificaBytes(){
	qDebug() << "Ricevuti " << channel->bytesAvailable();
}

void WorkerServer::notificaRicezione(){
	qDebug() << "******* RICEVUTO *******";
}

Client::Client(const QString &host, const quint16 &port, QObject *parent) : ClientServer(parent){

	this->host = host;
	this->port = port;
}

void Client::init(){

	if(channel){
		connect(channel, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(handleStateChange(QAbstractSocket::SocketState)));
		channel->connectToHost(host, port);
	}
}

void Client::sendData(){

	if(res){
		qDebug() << "Creo la risorsa";
		res->setZipDir("/Users/Danilo/Desktop");
		res->setZipName("pallini69");
		res->setResourcePath("/Users/Danilo/Desktop/pa_gapps-modular-mini-5");
		res->prepareForSend();
		connect(res, SIGNAL(objectSended()), this, SLOT(finish()), Qt::UniqueConnection);
		qDebug() << "invio la risorsa";
		res->sendThrough(channel);
	}
}

void Client::handleStateChange(QAbstractSocket::SocketState state){

	switch (state) {
		case QAbstractSocket::ConnectedState:
			qDebug() << "Connessione stabilita";
			break;
		case QAbstractSocket::UnconnectedState:
			qDebug() << "Socket non connessa";
		default:
			break;
	}
}

void Client::finish(){

	qDebug() << "Risorsa inviata!!!";
	if(res->removeZipFile()){
		qDebug() << "File zip temporaneo eliminato con successo";
	}else{
		qDebug() << "File zip non eliminato!!!";
	}
	channel->close();
}

void Client::run(){

	qDebug() << "Thread client vivo";
	exec();
}
