//Crazy Wheel Project... sollte rocken
//
//Stefan Steiner
//28.11.2013
//

#include "window.h"
#include <QApplication>
#include <QIcon>


int main(int argc, char *argv[])
{
   QApplication app(argc, argv);
   app.setWindowIcon(QIcon("./taskicon.png"));
   Window window;
   window.show();


  return app.exec();
}

//Todo:
//Farben bei ungeraden Zahlen nicht schön...
//
