#include "GuiClient.h"
#include "LoginForm.h"
#include "ui_GuiClient.h"
#include "FirstConfigForm.h"

GuiClient::GuiClient(QWidget *parent) : QMainWindow(parent), ui(new Ui::GuiClient){

	QSettings appSettings;
	client = new KCloud::Client(KCloud::Client::AsGuiThread, this);

	ui->setupUi(this);
	client->start();

	if(!appSettings.contains(T_STARTED)){
		FirstConfigForm f(client, this);
		f.exec();
		if(!f.getResult()){
			this->close();
		}
	}

	qint64 i = appSettings.value(T_STARTED).toLongLong();
	appSettings.setValue(T_STARTED, i++);

}

GuiClient::~GuiClient()
{
	delete ui;
}
