//---------------------------------------------------------------
// Bibliothek
//---------------------------------------------------------------
#include "wheel.h"
#include "window.h"
#include "filehandling.h"
#include <QtGui>
#include <math.h>
#include <QTime>
//---------------------------------------------------------------
// Name     : Wheel()
// Funktion : CTor
//---------------------------------------------------------------
Wheel::Wheel(Window *phauptFenster,FileHandling* pFileHandling, QWidget *parent):
QWidget(parent)
{
    hauptFenster = phauptFenster;
    handler = pFileHandling;
    timer = new QTimer(this);    

    wheelInitPosition();

    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    connect(timer, SIGNAL(timeout()), this, SLOT(drehprozess()));
}
//---------------------------------------------------------------
// Name     : ~Wheel()
// Funktion : DTor
//---------------------------------------------------------------
Wheel::~Wheel()
{
    delete timer;
}
//---------------------------------------------------------------
// Name     : paintEvent
// Funktion : zeichnet das Drehrad
//---------------------------------------------------------------
void Wheel::paintEvent(QPaintEvent *event)
{
    int sectorAnzahl = handler->getSectorAnzahl();
    double sectorWinkel = 360.0/sectorAnzahl;
//Farben
    QColor colSectorDunkel(166,219,41);
    QColor colSectorMitte(39,166,17);
    QColor colSectorHell(248,248,255);
    QColor colPfeil(75,75,75);
    QColor colMittelPunkt(25,175,24);
    QColor colText(0,0,0);
    //bestimmen, welche Seite kleiner ist
    int side = qMin(width(), height());

    //Ausgangspunkt setzen
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(width()/2, height()/2);
    painter.scale(side / 400.0, side / 400.0);
//Sektoren zeichnen
    if(sectorAnzahl < 35 && sectorAnzahl>0)//mehr geht nicht
    {
        QRectF rectangle(-200,-200,400, 400);//x,y, width, height

        if(sectorAnzahl%2 == 1)//für ungerade anzahl sektoren
        {
            for(int x = 0; x<sectorAnzahl;x++)
            {
                if(x%3 == 0)     { painter.setBrush(colSectorDunkel);  }
                else if(x%3 == 1){ painter.setBrush(colSectorMitte);   }
                else             { painter.setBrush(colSectorHell); }
                painter.drawPie(rectangle,   16*sectorWinkel* x + offsetWinkel*16    ,    16*sectorWinkel);
            }
        }
        else //für gerade Anzahl Sektoren
        {
            for(int x = 0; x<sectorAnzahl;x++)
            {
                if(x%2 == 0)  { painter.setBrush(colSectorDunkel);  }
                else          { painter.setBrush(colSectorHell);   }
                painter.drawPie(rectangle,   16*sectorWinkel* x +offsetWinkel*16   ,    16*sectorWinkel);
            }
        }
    }
//Text zeichnen
    painter.setBrush(colText);
    painter.setFont(QFont("Arial", 10));

        for(int x = 0; x<sectorAnzahl;x++)
        {
            int xpos = floor(50*cos(2*M_PI/sectorAnzahl * (x+0.5) - offsetWinkel/180.0*M_PI));
            int ypos = floor(50*sin(2*M_PI/sectorAnzahl * (x+0.5) - offsetWinkel/180.0*M_PI));

            painter.save();
            painter.translate(xpos,ypos);
            painter.rotate(sectorWinkel*(0.5+x) -offsetWinkel);

            painter.drawText(0, 0,handler->wheelText[x].toUtf8() );

            painter.restore();
        }

//Punkt in der Mitte zeichnen
        painter.setBrush(colMittelPunkt);
        painter.drawEllipse(-20,-20, 40, 40);

//Pfeil am Rand zeichnen
        painter.setBrush(colPfeil);

        static const QPointF points[] = {
             QPointF(203, 0),
             QPointF(240, 40.0),
             QPointF(240, 20.0),
             QPointF(340, 20.0),
             QPointF(330, 0),
             QPointF(340, -20),
             QPointF(240, -20.0),
             QPointF(240, -40.0)

         };
         painter.drawPolygon(points, 8 );

}
//---------------------------------------------------------------
// Name     : wheelDrehen
// Funktion : berechnet und bereitet alles vor für Drehprozess
//---------------------------------------------------------------
void Wheel::wheelDrehen(void)
{
    anfangsgeschw = 20;
    endtime = 1;
    zeitkonst = 10;

    hauptFenster->startDrehen();
    offsetWinkel = randNumber();
    expkonst = (zeitkonst*log(anfangsgeschw))/(endtime*1000);

    timer->start(zeitkonst);
}
//---------------------------------------------------------------
// Name     :  drehprozess
// Funktion :  Beschreibt was während des Drehens geschieht
//---------------------------------------------------------------
void Wheel::drehprozess()
{
    static int aufrufe = 1;
    offsetWinkel += anfangsgeschw - exp(aufrufe*expkonst);
    aufrufe++;

    if((aufrufe*zeitkonst)/1000 >= endtime)//Fertig gedreht
    {
            hauptFenster->fertigDrehen();
            timer->stop();
            aufrufe = 1;

            offsetWinkel%=360;
            if((offsetWinkel % (360/handler->getSectorAnzahl())) == 0 ) offsetWinkel +=10;
            //wenn wheel keinen Gewinner zieht
            gewinnerString = offsetWinkel / (360/offsetWinkel)-1;
    }
}
//---------------------------------------------------------------
// Name     :  randNumber
// Funktion :  Generiert Zufallszahl zwischen 1 und 359
//---------------------------------------------------------------
int Wheel::randNumber() //random number between 1, 359
{
    QTime currentmsec;
    static unsigned int x = currentmsec.msec() * 12345;
    static unsigned int y = 362436069;
    static unsigned int z = 521288629;
    static unsigned int w = 88675123;
    unsigned int t;

    t = x ^ (x << 11);
    x = y; y = z; z = w;
    return (w = w ^ (w >> 19) ^ (t ^ (t >> 8)))%359;
}
//---------------------------------------------------------------
// Name     :  wheelInitPosition
// Funktion :  setzt neuen Filesatz
//---------------------------------------------------------------
void Wheel::wheelInitPosition()
{
    offsetWinkel = 90;
    handler->setSectorAnzahl(2);


    handler->wheelText.resize(2);
    handler->wheelText[0] = "no File set";
    handler->wheelText[1] = "no File set";

    update();
}

//---------------------------------------------------------------
