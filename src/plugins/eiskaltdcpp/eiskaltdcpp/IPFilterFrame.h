#ifndef DCIPFILTERFRAME_H
#define DCIPFILTERFRAME_H

#include <QDialog>
#include <QTreeWidgetItem>
#include <QCloseEvent>
#include <QRadioButton>
#include <QList>

#include "IPFilter.h"
#include "IPFilterModel.h"

#include "ui_UIIPFilter.h"

class IPFilterFrame : public QDialog, private Ui::UIIPFilter {
    Q_OBJECT

public:
    /** */
    IPFilterFrame(QWidget *parent = 0);
    /** */
    virtual ~IPFilterFrame();

private:
    /** */
    void InitDocument();
    /** */
    void loadItems();

    /** */
    IPFilterModel *model;
protected:
    virtual void closeEvent(QCloseEvent *e);

private slots:
    /** */
    void slotCheckBoxClick();

    /** */
    void slotRuleAdded(QString, eDIRECTION);

    /** */
    void slotTreeViewContextMenu(QPoint);

    /** */
    void slotExport();
    /** */
    void slotImport();

    /** */
    void slotUpDownClick();

    /** */
    void slotAddRule();
};

#endif