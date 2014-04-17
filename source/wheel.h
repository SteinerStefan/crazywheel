#ifndef WHEEL_H
#define WHEEL_H

//---------------------------------------------------------------
// Bibliothek
//---------------------------------------------------------------
#include <QWidget>
#include <QVector>
//---------------------------------------------------------------
//Vordeklaration
//---------------------------------------------------------------
class QTimer;
class Window;
class Qstring;
class FileHandling;
//---------------------------------------------------------------
// class Wheel
//---------------------------------------------------------------
class Wheel : public QWidget
{
    Q_OBJECT
public:
    Wheel(Window* pHauptFenster, FileHandling* pfileHandling, QWidget *parent = 0);
	virtual ~Wheel();
    int randNumber();
    void wheelInitPosition();
protected:
    void paintEvent(QPaintEvent *event);
signals:

public slots:
    void wheelDrehen(void);
    void drehprozess(void);

private:
    //
    QTimer *timer;
    Window *hauptFenster;
    FileHandling *handler;
    //Wheel information
    //int sectorAnzahl;
    int offsetWinkel;

    double zeitkonst;
    double expkonst;
    double endtime;
    double anfangsgeschw;

    int  gewinnerString;


};

#endif // WHEEL_H
//---------------------------------------------------------------
