/*
BSD 3-Clause License

Copyright (c) 2018, Ariel Favio Carrizo
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of the copyright holder nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <esferixis/common/cps/cont.h>
#include "EsferixisQtApplication.h"
#include <QWindow>

struct Context {
	QWindow *window;
};

esferixis::cps::Cont setup(Context *context);

int main(int argc, char *argv[])
{
	esferixis::Qt::Application::run(argc, argv, esferixis::cps::Cont(setup, new Context()));
}

esferixis::cps::Cont setup(Context *context) {
	QWindow *window = new QWindow();

	context->window = window;

	window->resize(800, 600);
	window->show();
	window->setTitle("Test");

	esferixis::Qt::Application::unlockGUI();

	return esferixis::cps::Sched::exit();
}

/*
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	bool willQuit = false;

	//a.setQuitOnLastWindowClosed(false);

	QWindow window;

	window.resize(800, 600);
	window.show();
	window.setTitle("Test");

	while ( !willQuit ) {
		a.processEvents(QEventLoop::WaitForMoreEvents);
	}

	return a.exec();
}
*/