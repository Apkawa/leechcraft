#ifndef DCANTISPAMFRAME_H
#define DCANTISPAMFRAME_H

#include <QDialog>
#include <QWidget>
#include <QTreeWidget>

#include "ui_UIAntiSpam.h"
#include "Antispam.h"

class AntiSpamFrame : public QDialog, private Ui::UIAntiSpam {
    Q_OBJECT

public:

    AntiSpamFrame(QWidget *parent = 0);
    virtual ~AntiSpamFrame();

private:
    void InitDocument();
    void loadGUIData();

    void loadWhiteList();
    void loadBlackList();
    void loadGrayList();
    void loadList(QTreeWidget *, QList<QString>&);

    void addItemToTree(QTreeWidget *, QString);
    void remItemFromTree(QTreeWidget *, QString);

    void clearTreeWidget(QTreeWidget*);

    bool addToList(AntiSpamObjectState state, QString);

private slots:
    void slotAntiSpamSwitch();
    void slotAsFilter();

    void slotAddToWhite();
    void slotAddToBlack();
    void slotAddToGray();
    void slotRemFromWhite();
    void slotRemFromBlack();
    void slotRemFromGray();
    void slotClearWhite();
    void slotClearBlack();
    void slotClearGray();

    void slotAccept();

    void slotWToG(); //White -> Gray
    void slotWToB(); //White -> Black
    void slotBToW(); //Black -> White
    void slotBToG(); //Black -> Gray
    void slotGToB(); //Gray -> Black
    void slotGToW(); //Gray -> White

};

#endif
