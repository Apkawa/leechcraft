/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QUICKCONNECT_H
#define QUICKCONNECT_H

#include <QWidget>
#include <QDialog>

#include "ui_UIQuickConnect.h"

class QuickConnect : public QDialog,
                     private Ui::UIQuickConnect
{
    Q_OBJECT

public:
    QuickConnect(QWidget *parent = 0);
    ~QuickConnect();

public slots:
    void slotAccept();

};


#endif
