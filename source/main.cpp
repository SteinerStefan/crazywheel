//Crazy Wheel Project... sollte rocken
//
//Stefan Steiner
//28.11.2013
//

#include "window.h"
#include <QApplication>


int main(int argc, char *argv[])
{
   QApplication app(argc, argv);

   Window window;
   window.show();

  return app.exec();
}

//Todo:
//Farben bei ungeraden Zahlen nicht schön...
//
