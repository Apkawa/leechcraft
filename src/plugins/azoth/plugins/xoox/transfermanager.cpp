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

#include "transfermanager.h"
#include <QXmppTransferManager.h>
#include "transferjob.h"

namespace LeechCraft
{
namespace Azoth
{
namespace Xoox
{
	TransferManager::TransferManager (QXmppTransferManager *manager)
	: QObject (manager)
	, Manager_ (manager)
	{
		connect (Manager_,
				SIGNAL (fileReceived (QXmppTransferJob*)),
				this,
				SLOT (handleFileReceived (QXmppTransferJob*)));
	}

	QObject* TransferManager::SendFile (const QString& id,
			const QString& var, const QString& name)
	{
		return new TransferJob (Manager_->sendFile (id + "/" + var, name));
	}

	void TransferManager::handleFileReceived (QXmppTransferJob *job)
	{
		emit fileOffered (new TransferJob (job));
	}
}
}
}
