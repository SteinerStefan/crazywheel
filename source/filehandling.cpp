#include "filehandling.h"
#include <QFile>
#include <QString>
#include <QIODevice>
#include <QtGui>
#include <qfiledialog.h>
#include "wheel.h"
//---------------------------------------------------------------
// Name     :  FileHandling
// Funktion :  CTor
//---------------------------------------------------------------
FileHandling::FileHandling(Wheel* pWheel, QWidget *parent) :
wheel(pWheel), QWidget(parent)
{
    searchFilesInDir();
    locElimDuplicate();
    locTest();
}
//---------------------------------------------------------------
// Name     :  getANewFile
// Funktion :
//---------------------------------------------------------------
void FileHandling::getANewFile()
{
    QString location;
    location = QFileDialog::getOpenFileName(this,
    tr("Open TextFile"), "/", tr("TextFiles (*.txt)"));

    if(location != 0)
    {
        locTextFiles.resize(locTextFiles.size()+1);
        locTextFiles[locTextFiles.size()-1] = location;
    }
}
//---------------------------------------------------------------
// Name     :  loadFile
// Funktion :  ladet neues File von location in wholeFile
//---------------------------------------------------------------
void FileHandling::loadFile(int pfad)
{
    QFile textFile(locTextFiles[pfad]);//Pfad mit file
    textFile.open(QFile::ReadOnly);
    wholeFile = "";
    wholeFile.append(textFile.readAll());
    textFile.close();
}

//---------------------------------------------------------------
// Name     :  parseText
// Funktion :  hol Text aus File
//---------------------------------------------------------------
int FileHandling::parseText()
{
    QString umbruch = "\n";// bei Linux \n, bei Linux  \r\n
    short choplength = 1;
    int maxSector = 32;
    //if(linux)
    //if(windows)
//alle Zusätzlichen umbrüche entfernen
    while(wholeFile.endsWith(umbruch))
    {
        wholeFile.chop(choplength); // 2 bei windows(da \r\n) 1 bei linux
    }
//Anzahl Sektoren
    sectorAnzahl = wholeFile.count(umbruch)+1;//anzahl Sektoren

    if(sectorAnzahl > maxSector || sectorAnzahl < 2)
    {
       return 1;
    }
    wheelText.resize(sectorAnzahl);
//Strings herauslesen
    for(int x = sectorAnzahl-1; x>=0;--x)
    {
       if(x != 0)
       {
            int position = wholeFile.lastIndexOf(umbruch)+choplength;
            int laenge = wholeFile.length()-wholeFile.lastIndexOf(umbruch)-choplength;
            wheelText[x] = wholeFile.mid(position, laenge);
            laenge = wholeFile.lastIndexOf(umbruch);
            wholeFile.resize(laenge);
       }
       else if(x == 0){  wheelText[0] = wholeFile;  }
    }
    return 0;
}
//---------------------------------------------------------------
// Name     :  searchFilesInDir
// Funktion :  search local Dir
//---------------------------------------------------------------
void FileHandling::searchFilesInDir()
{
    QList<QString> fileInDir;
    QDir verzeichnis;

    fileInDir = verzeichnis.entryList() ;
    int zaehler =0;


    for(int x = 0; x<fileInDir.size();x++)
    {
        fileInDir[x] = verzeichnis.relativeFilePath(fileInDir[x]);

        if(fileInDir[x].endsWith(".txt"))
        {
            fileInDir[zaehler] = fileInDir[x];
            zaehler++;
        }
        else
        {
            fileInDir[x] = "";
        }
    }
    locTextFiles.resize(locTextFiles.size() + zaehler);
    for(int x = 0; x< zaehler; x++)
    {
        locTextFiles[locTextFiles.size()-1-x] = fileInDir[x];
    }
}
//---------------------------------------------------------------
// Name     :  programmLocation
// Funktion :  returns String with current Path
//---------------------------------------------------------------
QString* FileHandling::programmLocation(void)
{
    QString *string = new QString;
    *string = QDir::currentPath();
    return string;
}
//---------------------------------------------------------------
// Name     :  locElimDuplicate
// Funktion :  search and eliminate duplicate locations
//---------------------------------------------------------------
void FileHandling::locElimDuplicate(void)
{
    for(int x = 0; x< locTextFiles.size();x++)
    {
        for(int y = x+1; y<locTextFiles.size();y++)
        {
            if(locTextFiles[x] == locTextFiles[y])//if duplicate
            {
                for(int z = y; z<(locTextFiles.size()-1); z++)
                {
                    locTextFiles[z] = locTextFiles[z+1];
                }
                locTextFiles.resize(locTextFiles.size()-1);
                y--;
            }
        }
    }
}
//---------------------------------------------------------------
// Name     :  locTest
// Funktion :  tests all locations and deletes them if not valid
//---------------------------------------------------------------
void FileHandling::locTest(void)
{
    for(int x = 0; x < locTextFiles.size() ; x++)
    {
        loadFile(x);
        if( parseText() ) //Fehlerfall
        {
            deleteLoc(x);
            x--;
        }
    }
}
//---------------------------------------------------------------
// Name     :  deleteLoc(int pfad)
// Funktion :  löscht den Pfad
//---------------------------------------------------------------
void FileHandling::deleteLoc(int pfad)
{

    for(int x = pfad; x< (locTextFiles.size()-1);x++)
    {
        locTextFiles[x] = locTextFiles[x+1];
    }
    locTextFiles.resize(locTextFiles.size()-1);
}
//---------------------------------------------------------------
// Name     :  getLocations
// Funktion :  gibt ein Array von Pfaden zurück
//---------------------------------------------------------------
QVector<QString>* FileHandling::getLocations()
{
    return &locTextFiles;

}
//---------------------------------------------------------------
// Name     :  getLocationsList
// Funktion :  gibt eine Qlist von Pfaden zurück
//---------------------------------------------------------------
QList<QString> FileHandling::getLocationsList()
{
    QList<QString> lList;


    for(int x = 0; x < locTextFiles.size(); x++)
    {
        lList << locTextFiles[x];
    }
    return lList;
}
//---------------------------------------------------------------
// Name     :  setTextActive
// Funktion :  Slot, der ein gewisser Text aktiv setzt
//---------------------------------------------------------------
void FileHandling::setTextActive(int pfad)
{
    loadFile(pfad);//(pfad);
    parseText();
    wheel->setNewFile(sectorAnzahl, wheelText);
}
//---------------------------------------------------------------



