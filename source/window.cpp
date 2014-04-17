#include "window.h"
#include "wheel.h"
#include "filehandling.h"
#include <QGridLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QListView>
#include <QStringListModel>
#include <QFile>

//---------------------------------------------------------------
// Name     :
// Funktion :
//---------------------------------------------------------------
Window::Window(QWidget *parent) :
    QWidget(parent)
{
   //Wheel Widget anlegen
    crazyWheel = new Wheel(this, handler);
    handler = new FileHandling(crazyWheel);

   //Main layout allozieren
    windowLayout = new QGridLayout;



    createButtonLayout();

    initListModel();
    updateListModel();


    initWindowLayout();
    this->setLayout(windowLayout);
    setMainStyle();



}
//---------------------------------------------------------------
// Name     :
// Funktion :
//---------------------------------------------------------------
Window::~Window(void)
{
    delete crazyWheel;
    delete goCrazy;
    delete readFileButton;

    delete windowLayout;

}
//---------------------------------------------------------------
// Name     :
// Funktion :
//---------------------------------------------------------------
void Window::fertigDrehen(void)
{
    goCrazy->setEnabled(true);
    readFileButton->setEnabled(true);
    locView->setEnabled(true);
}
//---------------------------------------------------------------
// Name     :
// Funktion :
//---------------------------------------------------------------
void Window::startDrehen(void)
{
    goCrazy->setEnabled(false);
    readFileButton->setEnabled(false);
    locView->setEnabled(false);
}
//---------------------------------------------------------------
// Name     :
// Funktion :
//---------------------------------------------------------------
void Window::createButtonLayout(void)
{
//hbox
    goCrazy = new QPushButton("GO CRAZY");
    connect(goCrazy, SIGNAL(released()), crazyWheel, SLOT(wheelDrehen()));
    goCrazy->setEnabled(false);

//vbox
    readFileButton = new QPushButton("new File");
    connect(readFileButton, SIGNAL(released()), this, SLOT(addOtherFile()) );


    locDelete = new QPushButton("Delete");
    connect(locDelete, SIGNAL(released() ), this, SLOT(deleteFile() ) );

    locSaveSet = new QPushButton("Save locations");
    //connect

    stylechanged = new QPushButton("change style");
    connect(stylechanged, SIGNAL(released()),
            this, SLOT(setMainStyle()));

    fullScrn = new QPushButton("Fullscreen");
    connect(fullScrn, SIGNAL(released()),
            this, SLOT(showfullScrn()));


    hBox = new QHBoxLayout;
    hBox->addStretch(4);
    hBox->addWidget(goCrazy);
    hBox->addStretch(4);

    vBox = new QVBoxLayout;
    vBox->addWidget(readFileButton);
    vBox->addWidget(locDelete);
    //vBox->addWidget(locSaveSet);
    vBox->addWidget(stylechanged);
    vBox->addWidget(fullScrn);

}
//---------------------------------------------------------------
// Name     :  setMainStyle
// Funktion :
//---------------------------------------------------------------
void Window::setMainStyle(void)
{
    this->setObjectName("fenster");
   crazyWheel->setObjectName("crazyWheel");

   locDelete->setObjectName("locDelete");
   locSaveSet->setObjectName("locSaveSet");
   vBox->setObjectName("vBox");

   stylechanged->setObjectName("stylechanged");
   goCrazy->setObjectName("goCrazy");
   readFileButton->setObjectName("readFileButton");
   hBox->setObjectName("hBox");

   QFile file("stylesheet.qss");
       if(file.open(QIODevice::ReadOnly | QIODevice::Text))
       {
           while (!file.atEnd())
           {
                QByteArray line = file.readAll();
                this->setStyleSheet(line);
                while(line.indexOf("\r\n") != -1)
                {
                    line.insert(line.indexOf("\r\n"), "  ");
                }
           }
           file.close();
       }
}
//---------------------------------------------------------------
// Name     : updateListModel
// Funktion :
//---------------------------------------------------------------
void Window::updateListModel(void)
{
   locModel->setStringList(handler->getLocationsList() );

   locView->setModel(locModel);
}
//---------------------------------------------------------------
// Name     : initListModel
// Funktion :
//---------------------------------------------------------------
void Window::initListModel(void)
{
    locModel = new QStringListModel((handler->getLocationsList() ));
    locView = new QListView;
    locView->setEditTriggers( QAbstractItemView::DoubleClicked);
    locView->setSelectionMode(QAbstractItemView::SingleSelection);
    locView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    vBox->insertWidget(0, locView);

    connect(locView, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT( setNewText() )  );

    updateListModel();
}
//---------------------------------------------------------------
// Name     : setNewText
// Funktion :
//---------------------------------------------------------------
void Window::setNewText()
{
    int index = locView->currentIndex().row();

    handler->setTextActive(index);
}
//---------------------------------------------------------------
// Name     : addOtherFile
// Funktion :
//---------------------------------------------------------------
void Window::addOtherFile()
{
    handler->getANewFile();
    handler->locElimDuplicate();
    handler->locTest();
    updateListModel();
}
//---------------------------------------------------------------
// Name     : deleteFile
// Funktion :
//---------------------------------------------------------------
void Window::deleteFile(void)
{
    int index = locView->currentIndex().row();
    if(index !=-1)
    {
        handler->deleteLoc(index);
        updateListModel();
        if(locView->currentIndex().row() != -1)
            handler->setTextActive(index);
        else
            crazyWheel->wheelInitPosition();
    }
}
//---------------------------------------------------------------
// Name     : fullScrn
// Funktion :
//---------------------------------------------------------------
void Window::showfullScrn(void)
{

    if(this->isFullScreen())
    {
        this->setWindowState(Qt::WindowMaximized);
        this->setWindowLayout();
    }
    else
    {
        this->setWindowState(Qt::WindowFullScreen);
        this->setFullScrnLayout();
    }
    show();
}
//---------------------------------------------------------------
// Name     : initWindowLayout
// Funktion :
//---------------------------------------------------------------
void Window::initWindowLayout(void)
{

    //Main Layout zusammenstellen
    windowLayout->addWidget(crazyWheel, 0, 0);
    windowLayout->addLayout(hBox,1,0);
    windowLayout->addLayout(vBox,0,1);

    windowLayout->setColumnStretch(0, 6);
    windowLayout->setColumnStretch(1,1);
    windowLayout->setRowMinimumHeight(0, 500);
    windowLayout->setRowMinimumHeight(1, 40);

    setWindowLayout();
}
//---------------------------------------------------------------
// Name     : setWindowLayout
// Funktion :
//---------------------------------------------------------------
void Window::setWindowLayout(void)
{
    setWindowTitle(tr("Crazy Wheel"));
    resize( 1200, 600 );

}
//---------------------------------------------------------------
// Name     : initFullScrnLayout
// Funktion :
//---------------------------------------------------------------
void Window::initFullScrnLayout(void)
{
    fullScrnLayout->addWidget(crazyWheel, 0, 0);
    fullScrnLayout->addLayout(hBox,1,0);

}

//---------------------------------------------------------------
// Name     : setullScrnLayout
// Funktion :
//---------------------------------------------------------------
void Window::setFullScrnLayout(void)
{



}
//---------------------------------------------------------------


