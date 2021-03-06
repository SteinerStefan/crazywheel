#include "filehandling.h"
#include <QFile>
#include <QString>
#include <QIODevice>
#include <QtGui>
#include <qfiledialog.h>
#include "wheel.h"
#include "window.h"
//---------------------------------------------------------------
// Name     :  FileHandling
// Funktion :  CTor
//---------------------------------------------------------------
FileHandling::FileHandling(Window* pWindow, QWidget *parent) :
window(pWindow) , QWidget(parent)
{
    longestString = 0;
    searchFilesInDir();
}
//---------------------------------------------------------------
// Name     :  setWheelPointer
// Funktion :
//---------------------------------------------------------------
void FileHandling::setWheelPointer(Wheel* pWheel)
{
    wheel = pWheel;
}
//---------------------------------------------------------------
// Name     :  getANewFile
// Funktion :
//---------------------------------------------------------------
void FileHandling::getANewFile()
{
    QString location;
    location = QFileDialog::getOpenFileName(this,
    tr("Open TextFile"), "/home/stefan", tr("Text files (*.txt);;All files (*)"));

    if(location != 0)
    {
        locTextFiles.resize(locTextFiles.size()+1);
        locTextFiles[locTextFiles.size()-1] = location;
    }
    locElimDuplicate();
    locTest();
}
//---------------------------------------------------------------
// Name     :  loadFile
// Funktion :  ladet neues File von location in wholeFile
//---------------------------------------------------------------
void FileHandling::loadFile(int pfad)
{
    wholeFile = "";

    QFile file(locTextFiles[pfad]); //Pfad mit file
    QTextStream in(&file);
    in.setCodec("UTF-8"); // change the file codec to UTF-8.
    if (file.open(QFile::ReadOnly))
    {
        while(!in.atEnd())
        {
            QString line = in.readLine();
            wholeFile.append( line.toUtf8() ); // convert to locale multi-byte string
            wholeFile.append("\n");
        }
    }
    file.close();
}

//---------------------------------------------------------------
// Name     :  parseText
// Funktion :  hol Text aus File
//---------------------------------------------------------------
int FileHandling::parseText()
{
    QString umbruch = "\n";// bei Linux \n, bei Windows  \r\n
    short choplength = 1;  // bei Linux  1, bei Windows 2
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
            if(laenge > longestString) longestString = laenge;
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
    locElimDuplicate();
    locTest();
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
// Name     :  getLocationsList
// Funktion :  gibt eine Qlist von Pfaden zurück
//---------------------------------------------------------------
QList<QString> FileHandling::getLocationsList()
{
    QList<QString> lList;

    for(int x = 0; x < locTextFiles.size(); x++)
    {
        QString shorted = locTextFiles[x];
        shorted = shorted.mid(shorted.lastIndexOf('/') + 1, shorted.length()-shorted.lastIndexOf('/')-1);
        if(shorted.indexOf(".txt") != -1)
        {
           shorted.chop(4); //da ".txt" vier zeichen lang ist.
        }
        lList << shorted;
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
    wheel->setOffsetWinkel(0);
    wheel->update();
    window->fertigDrehen();

}
//---------------------------------------------------------------
// Name     :  setSectoraAnzahl
// Funktion :
//---------------------------------------------------------------
int FileHandling::getSectorAnzahl(void)
{
    return sectorAnzahl;
}

//---------------------------------------------------------------
// Name     :  setSectoraAnzahl
// Funktion :
//---------------------------------------------------------------
int FileHandling::setSectorAnzahl(int anzahl)
{
    sectorAnzahl = anzahl;
}
//---------------------------------------------------------------
// Name     :  getLongestString
// Funktion :
//---------------------------------------------------------------
int FileHandling::getLongestString()
{
    return longestString;
}

//---------------------------------------------------------------
