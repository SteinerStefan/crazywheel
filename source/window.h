#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
class Wheel;
class QPushButton;
class QGridLayout;
class QHBoxLayout;
class QVBoxLayout;
class QFile;
class FileHandling;
class QListView;
class QStringListModel;


class Window : public QWidget
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = 0);
    virtual ~Window();
    void startDrehen(void);
    void fertigDrehen(void);


signals:

public slots:
    void setNewText();
    void addOtherFile();
    void setMainStyle(void);
    void deleteFile(void);
    void showfullScrn(void);

private:
    void createButtonLayout();
    void initListModel();
    void updateListModel();
    void initWindowLayout();
    void setWindowLayout();
    void initFullScrnLayout();
    void setFullScrnLayout();


    Wheel       *crazyWheel;
    FileHandling *handler;

    QPushButton *readFileButton;
    QPushButton *fullScrn;
    QPushButton *stylechanged;
    QPushButton *locDelete;
    QPushButton *locSaveSet;

    QVBoxLayout *vBox;

    QPushButton *goCrazy;
    QHBoxLayout *hBox;


    QGridLayout *windowLayout;
    QGridLayout *fullScrnLayout;





    QStringListModel *locModel;
    QListView        *locView;

};

#endif // WINDOW_H
