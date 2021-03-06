/**********************************************************************
 * LeechCraft - modular cross-platform feature rich internet client.
 * Copyright (C) 2006-2010  Georg Rudoy
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

#include "glooxprotocol.h"
#include <QInputDialog>
#include <QMainWindow>
#include <QSettings>
#include <QCoreApplication>
#include <QtDebug>
#include <interfaces/iprotocolplugin.h>
#include "glooxaccount.h"
#include "core.h"
#include "joingroupchatwidget.h"

namespace LeechCraft
{
namespace Azoth
{
namespace Xoox
{
	GlooxProtocol::GlooxProtocol (QObject *parent)
	: QObject (parent)
	, ParentProtocolPlugin_ (parent)
	, ProxyObject_ (0)
	{
	}

	GlooxProtocol::~GlooxProtocol ()
	{
	}

	void GlooxProtocol::Prepare ()
	{
		RestoreAccounts ();
	}

	QObject* GlooxProtocol::GetProxyObject () const
	{
		return ProxyObject_;
	}

	void GlooxProtocol::SetProxyObject (QObject *po)
	{
		ProxyObject_ = po;
	}

	QObject* GlooxProtocol::GetObject ()
	{
		return this;
	}

	IProtocol::ProtocolFeatures GlooxProtocol::GetFeatures() const
	{
		return PFSupportsMUCs | PFMUCsJoinable;
	}

	QList<QObject*> GlooxProtocol::GetRegisteredAccounts ()
	{
		QList<QObject*> result;
		Q_FOREACH (GlooxAccount *acc, Accounts_)
			result << acc;
		return result;
	}

	QObject* GlooxProtocol::GetParentProtocolPlugin () const
	{
		return ParentProtocolPlugin_;
	}

	QString GlooxProtocol::GetProtocolName () const
	{
		return "XMPP";
	}

	QByteArray GlooxProtocol::GetProtocolID () const
	{
		return "Xoox.Gloox.XMPP";
	}

	void GlooxProtocol::InitiateAccountRegistration ()
	{
		QString name = QInputDialog::getText (0,
				"LeechCraft",
				tr ("Enter new account name"));
		if (name.isEmpty ())
			return;

		GlooxAccount *account = new GlooxAccount (name, this);
		account->OpenConfigurationDialog ();

		connect (account,
				SIGNAL (accountSettingsChanged ()),
				this,
				SLOT (saveAccounts ()));

		Accounts_ << account;
		saveAccounts ();

		emit accountAdded (account);

		account->ChangeState (EntryStatus (SOnline, QString ()));
	}

	QWidget* GlooxProtocol::GetMUCJoinWidget ()
	{
		return new JoinGroupchatWidget ();
	}

	void GlooxProtocol::RemoveAccount (QObject *acc)
	{
		GlooxAccount *accObj = qobject_cast<GlooxAccount*> (acc);
		Accounts_.removeAll (accObj);
		emit accountRemoved (accObj);
		accObj->deleteLater ();
		saveAccounts ();
	}

	void GlooxProtocol::saveAccounts () const
	{
		QSettings settings (QSettings::IniFormat, QSettings::UserScope,
				QCoreApplication::organizationName (),
				QCoreApplication::applicationName () + "_Azoth_Xoox_Accounts");
		settings.beginWriteArray ("Accounts");
		for (int i = 0, size = Accounts_.size ();
				i < size; ++i)
		{
			settings.setArrayIndex (i);
			settings.setValue ("SerializedData", Accounts_.at (i)->Serialize ());
		}
		settings.endArray ();
		settings.sync ();
	}

	void GlooxProtocol::RestoreAccounts ()
	{
		QSettings settings (QSettings::IniFormat, QSettings::UserScope,
				QCoreApplication::organizationName (),
				QCoreApplication::applicationName () + "_Azoth_Xoox_Accounts");
		int size = settings.beginReadArray ("Accounts");
		for (int i = 0; i < size; ++i)
		{
			settings.setArrayIndex (i);
			QByteArray data = settings.value ("SerializedData").toByteArray ();
			GlooxAccount *acc = GlooxAccount::Deserialize (data, this);
			if (!acc)
			{
				qWarning () << Q_FUNC_INFO
						<< "unserializable acount"
						<< i;
				continue;
			}

			connect (acc,
					SIGNAL (accountSettingsChanged ()),
					this,
					SLOT (saveAccounts ()));

			Accounts_ << acc;

			emit accountAdded (acc);
		}
		settings.endArray ();
	}
}
}
}
