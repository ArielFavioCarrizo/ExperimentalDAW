#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_EsferixisExperimentalDAWGUI.h"

class EsferixisExperimentalDAWGUI : public QMainWindow
{
	Q_OBJECT

public:
	EsferixisExperimentalDAWGUI(QWidget *parent = Q_NULLPTR);

private:
	Ui::EsferixisExperimentalDAWGUIClass ui;
};
