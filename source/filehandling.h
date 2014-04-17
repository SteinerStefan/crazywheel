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
    explicit FileHandling(Window* pWindow,Wheel* pWheel,QWidget *parent = 0 );
    void getANewFile();
    int  parseText();
    void searchFilesInDir();
    QString* programmLocation(void);
    void locElimDuplicate(void);
    void locTest(void);
    void loadFile(int pfad);
    void deleteLoc(int pfad);
    QVector<QString>* getLocations();
    QList<QString> getLocationsList();
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
