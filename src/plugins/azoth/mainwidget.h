/**********************************************************************
 * LeechCraft - modular cross-platform feature rich internet client.
 * Copyright (C) 2006-2011  Georg Rudoy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **********************************************************************/

#ifndef PLUGINS_AZOTH_MAINWIDGET_H
#define PLUGINS_AZOTH_MAINWIDGET_H
#include <QWidget>
#include "ui_mainwidget.h"

class QToolBar;
class QMenu;

namespace LeechCraft
{
namespace Azoth
{
	class SortFilterProxyModel;

	class MainWidget : public QWidget
	{
		Q_OBJECT

		Ui::MainWidget Ui_;

		QMenu *MenuGeneral_;
		QMenu *MenuView_;
		SortFilterProxyModel *ProxyModel_;

		QMenu *MenuChangeStatus_;
	public:
		MainWidget (QWidget* = 0);
		
		QList<QAction*> GetMenuActions ();
	private:
		void CreateMenu ();
	private slots:
		void on_CLTree__activated (const QModelIndex&);
		void on_CLTree__customContextMenuRequested (const QPoint&);
		void handleChangeStatusRequested ();

		void showAccountsList ();
		void handleAddContactRequested ();

		void handleShowOffline (bool);

		void handleRowsInserted (const QModelIndex&, int, int);
		void rebuildTreeExpansions ();
		void on_CLTree__expanded (const QModelIndex&);
		void on_CLTree__collapsed (const QModelIndex&);
	};
}
}

#endif
