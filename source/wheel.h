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
    void setNewFile(int pSectorAnzahl, QVector<QString> pText);

private:
    int sectorAnzahl;
    int offsetWinkel;

    QTimer *timer;
    Window *hauptFenster;
    FileHandling *handler;

    double zeitkonst;
    double expkonst;
    double endtime;
    double anfangsgeschw;

    QVector<QString> text;
    int  gewinnerString;
};

#endif // WHEEL_H
//---------------------------------------------------------------
