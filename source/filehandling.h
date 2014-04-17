#ifndef FILEHANDLING_H
#define FILEHANDLING_H

#include <QWidget>
#include <QVector>

class Wheel;
class QString;
class Window;

class FileHandling : public QWidget
{
    Q_OBJECT
public:
    explicit FileHandling(Window* pWindow,QWidget *parent = 0 );
    void setWheelPointer(Wheel* pWheel);
//location
    void getANewFile();
    void deleteLoc(int pfad);
    void searchFilesInDir();
    QString* programmLocation(void);
    void locElimDuplicate(void);
    QList<QString> getLocationsList();

//text in file
    int  parseText();
    void locTest(void);
    void loadFile(int pfad);


    //data wheel
    int getSectorAnzahl();
    int setSectorAnzahl(int anzahl);
    QVector<QString> wheelText;

signals:

public slots:
        void setTextActive(int pfad);

private:
    Wheel* wheel;
    Window* window;

    int sectorAnzahl;
    QVector<QString> locTextFiles;
    QString wholeFile;
};

#endif // FILEHANDLING_H
