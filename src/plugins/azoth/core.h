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

#ifndef PLUGINS_AZOTH_CORE_H
#define PLUGINS_AZOTH_CORE_H
#include <boost/function.hpp>
#include <QObject>
#include <QSet>
#include <QIcon>
#include <QDateTime>
#include "interfaces/iinfo.h"
#include "interfaces/azothcommon.h"
#include "interfaces/imucentry.h"
#include "interfaces/iprotocol.h"
#include "interfaces/iauthable.h"
#include "sourcetrackingmodel.h"

class QStandardItemModel;
class QStandardItem;

namespace LeechCraft
{
namespace Util
{
	class ResourceLoader;
}
namespace Azoth
{
	struct EntryStatus;
	class ICLEntry;
	class IAccount;
	class IMessage;
	class ISmileResourceSource;
	class IChatStyleResourceSource;

	class ChatTabsManager;
	class PluginManager;
	class ProxyObject;
	class TransferJobManager;

	class Core : public QObject
	{
		Q_OBJECT
		Q_ENUMS (CLRoles CLEntryType CLEntryActionArea)

		ICoreProxy_ptr Proxy_;

		QRegExp LinkRegexp_;
		QRegExp ImageRegexp_;

		QObjectList ProtocolPlugins_;
		QList<QAction*> AccountCreatorActions_;

		QStandardItemModel *CLModel_;
		ChatTabsManager *ChatTabsManager_;

		typedef QHash<QString, QStandardItem*> Category2Item_t;
		typedef QHash<QStandardItem*, Category2Item_t> Account2Category2Item_t;
		Account2Category2Item_t Account2Category2Item_;

		typedef QHash<ICLEntry*, QList<QStandardItem*> > Entry2Items_t;
		Entry2Items_t Entry2Items_;

		typedef QHash<const ICLEntry*, QHash<QByteArray, QAction*> > Entry2Actions_t;
		Entry2Actions_t Entry2Actions_;

		typedef QHash<QString, QObject*> ID2Entry_t;
		ID2Entry_t ID2Entry_;

		typedef QHash<ICLEntry*, QMap<QString, QIcon> > EntryClientIconCache_t;
		EntryClientIconCache_t EntryClientIconCache_;

		typedef QHash<ICLEntry*, QImage> Entry2SmoothAvatarCache_t;
		Entry2SmoothAvatarCache_t Entry2SmoothAvatarCache_;

		boost::shared_ptr<Util::ResourceLoader> StatusIconLoader_;
		boost::shared_ptr<Util::ResourceLoader> ClientIconLoader_;
		boost::shared_ptr<SourceTrackingModel<ISmileResourceSource> > SmilesOptionsModel_;
		boost::shared_ptr<SourceTrackingModel<IChatStyleResourceSource> > ChatStylesOptionsModel_;

		boost::shared_ptr<PluginManager> PluginManager_;
		boost::shared_ptr<ProxyObject> PluginProxyObject_;

		boost::shared_ptr<TransferJobManager> XferJobManager_;

		Core ();
	public:
		enum CLRoles
		{
			CLRAccountObject = Qt::UserRole + 1,
			CLREntryObject,
			CLREntryType,
			CLREntryCategory,
			CLRUnreadMsgCount
		};

		enum CLEntryType
		{
			/** Self account.
				*/
			CLETAccount,
			/** Category (under self account).
				*/
			CLETCategory,
			/** Remote contact.
				*/
			CLETContact
		};

		enum ResourceLoaderType
		{
			RLTStatusIconLoader,
			RLTClientIconLoader
		};

		enum CLEntryActionArea
		{
			CLEAATabCtxtMenu,
			CLEAAContactListCtxtMenu,
			CLEAAApplicationMenu
		};
	private:
		typedef QHash<const QAction*, QList<CLEntryActionArea> > Action2Areas_t;
		Action2Areas_t Action2Areas_;
	public:
		static Core& Instance ();
		void Release ();

		void SetProxy (ICoreProxy_ptr);
		ICoreProxy_ptr GetProxy () const;

		Util::ResourceLoader* GetResourceLoader (ResourceLoaderType) const;
		QAbstractItemModel* GetSmilesOptionsModel () const;
		QAbstractItemModel* GetChatStylesOptionsModel ();

		QSet<QByteArray> GetExpectedPluginClasses () const;
		void AddPlugin (QObject*);
		void RegisterHookable (QObject*);

		const QObjectList& GetProtocolPlugins () const;

		QList<QAction*> GetAccountCreatorActions () const;
		QAbstractItemModel* GetCLModel () const;
		ChatTabsManager* GetChatTabsManager () const;
		QList<IAccount*> GetAccounts () const;
		QList<IProtocol*> GetProtocols () const;

		/** Returns the list of all groups of all chat entries.
		 */
		QStringList GetChatGroups () const;

		void SendEntity (const Entity&);

		/** Returns contact list entry with the given id. The id is the
		 * same as returned by ICLEntry::GetEntryID(). If no such entry
		 * entry could be found, NULL is returned.
		 */
		QObject* GetEntry (const QString& id) const;

		/** Opens chat with the remote contact identified by index
		 * (which is from GetCLModel() model). If the index identifies
		 * account or category, this function does nothing.
		 */
		void OpenChat (const QModelIndex& index);

		/** Handles the given transfer job, taking ownership of it and
		 * handling various events from it.
		 *
		 * @param[in] job The transfer job to handle, should implement
		 * ITransferJob.
		 *
		 * @sa ITransferJob
		 */
		void HandleTransferJob (QObject *job);

		TransferJobManager* GetTransferJobManager () const;

		/** Whether the given from the given entry should be counted as
		 * unread message. For example, messages in currently visible
		 * chat session or status messages shouldn't be counted as
		 * unread.
		 */
		bool ShouldCountUnread (const ICLEntry *entry,
				const IMessage *message);

		/** Whether this message should be considered as a the one that
		 * highlights the participant.
		 */
		bool IsHighlightMessage (const IMessage*);

		/** Returns an icon from the current iconset for the given
		 * contact list entry state.
		 */
		QIcon GetIconForState (State state) const;

		/** @brief Returns icons for the given CL entry.
		 *
		 * This function returns an icon for each variant of the entry,
		 * since different variants may have different clients. If the
		 * protocol which the entry belongs doesn't support variants,
		 * the map would have only one key/value pair of null QString
		 * and corresponding icon.
		 *
		 * This function returns the icons from the currently selected
		 * (in settings) iconset.
		 *
		 * @param[in] entry Entry for which to return the icons.
		 * @return Map from entity variant to corresponding
		 * client icon.
		 */
		QMap<QString, QIcon> GetClientIconForEntry (ICLEntry *entry);

		/** @brief Returns the avatar for the given CL entry scaled to
		 * the given size.
		 *
		 * The scale is performed using SmoothTransform and keeping the
		 * aspect ratio.
		 *
		 * @param[in] entry Entry for which to get the avatar.
		 * @return Entry's avatar scaled to the given size.
		 */
		QImage GetAvatar (ICLEntry *entry, int size);

		/** Returns an up-to-date list of actions suitable for the given
		 * entry.
		 */
		QList<QAction*> GetEntryActions (ICLEntry *entry);

		/** Returns the list of preferred areas for the given action.
		 *
		 * The action should be the one returned from GetEntryActions(),
		 * otherwise an empty list would be returned.
		 */
		QList<CLEntryActionArea> GetAreasForAction (const QAction *action) const;
		
		QString GetSelectedChatTemplate () const;
		
		bool AppendMessageByTemplate (QWebFrame*, QObject*, const QString&,
				bool, bool);
		
		void FrameFocused (QWebFrame*);
		
		// Theming stuff
		QList<QColor> GenerateColors (const QString& coloringScheme) const;
		
		QString GetNickColor (const QString& nick, const QList<QColor>& colors) const;
		
		QString FormatDate (QDateTime, IMessage*);
		QString FormatNickname (QString, IMessage*, const QString& color);
		QString FormatBody (QString body, IMessage *msg);
	private:
		/** Adds the protocol object. The object must implement
		 * IProtocolPlugin interface.
		 *
		 * Creates an entry in the contact list for accounts from the
		 * protocol plugin and creates the actions for adding a new
		 * account in this protocol or joining groupchats.
		 */
		void AddProtocolPlugin (QObject *object);
		
		/** Adds the resource source object. Currently only smile
		 * resources are supported.
		 */
		void AddResourceSourcePlugin (QObject *object);
		void AddSmileResourceSource (ISmileResourceSource*);
		void AddChatStyleResourceSource (IChatStyleResourceSource*);

		/** Adds the given contact list entry to the given account and
		 * performs common initialization tasks.
		 */
		void AddCLEntry (ICLEntry *entry, QStandardItem *accItem);

		/** Returns the list of category items for the given account and
		 * categories list. Creates the items if needed. The returned
		 * items are children of account item.
		 *
		 * Categories could be, for example, tags/groups in XMPP client
		 * and such.
		 */
		QList<QStandardItem*> GetCategoriesItems (QStringList categories, QStandardItem *account);

		/** Returns the QStandardItem for the given account.
		 */
		QStandardItem* GetAccountItem (const QObject *accountObj);

		/** Returns the QStandardItem for the given account and adds it
		 * into accountItemCache.
		 */
		QStandardItem* GetAccountItem (const QObject *accountObj,
				QMap<const QObject*, QStandardItem*>& accountItemCache);

		/** Handles the event of status changes in a contact list entry.
		 */
		void HandleStatusChanged (const EntryStatus& status,
				ICLEntry *entry, const QString& variant);
		
		/** Checks whether icon representing incoming file should be
		 * drawn for the entry with the given id.
		 */
		void CheckFileIcon (const QString& id);
		
		/** This function increases the number of unread messages by
		 * the given amount, which may be negative.
		 */
		void IncreaseUnreadCount (ICLEntry *entry, int amount = 1);

		/** This functions calculates new value of number of unread
		 * items for the chain of parents of the given item.
		 */
		void RecalculateUnreadForParents (QStandardItem*);

		void CreateActionsForEntry (ICLEntry*);
		void UpdateActionsForEntry (ICLEntry*);

		/** Asks user for reason for the given action, possibly showing
		 * the given text. The id may be used to distinguish between
		 * different reason contexts (like kick/ban and authentication
		 * request), for example, to keep history of reasons and to
		 * allow the user to choose one.
		 */
		QString GetReason (const QString& id, const QString& text);

		void NotifyWithReason (QObject*, const QString&,
				const char*, const QString&, const QString&);

		/** Calls the given func on the sending entry, asking for reason
		 * for the action, if it should. The text may contain %1, in
		 * which case it'd be replaced with the result if
		 * ICLEntry::GetEntryName().
		 */
		void ManipulateAuth (const QString& id, const QString& text,
				boost::function<void (IAuthable*, const QString&)> func);

		/** Removes one item representing the given CL entry.
		 */
		void RemoveCLItem (QStandardItem*);

		/** Adds the given entry to the given category item.
		 */
		void AddEntryTo (ICLEntry*, QStandardItem*);
	public slots:
		/** Initiates account registration process.
		 */
		void handleAccountCreatorTriggered ();

		/** Initiates MUC join by calling the corresponding protocol
		 * plugin's IProtocol::InitiateMUCJoin() function.
		 */
		void handleMucJoinRequested ();
	private slots:
		/** Handles a new account. This account may be both a new one
		 * (added as a result of user's actions) and already existing
		 * one (in case it was just read from settings, for example).
		 *
		 * account is expected to implement IAccount interface.
		 */
		void addAccount (QObject *account);

		/** Handles account removal. Basically, just removes it and its
		 * children from the contact list.
		 *
		 * account is expected to implement IAccount interface.
		 */
		void handleAccountRemoved (QObject *account);

		/** Handles newly added contact list items. Each item is
		 * expected to implement ICLEntry. This slot appends the items
		 * to already existing ones, so only really new ones (during the
		 * session lifetime) should be in the items list.
		 */
		void handleGotCLItems (const QList<QObject*>& items);

		/** Handles removal of items previously added to the contact
		 * list. Each item is expected to implement the ICLEntry
		 * interface.
		 *
		 * This slot removes the model items corresponding to the items
		 * removed and also removes those categories that became empty
		 * because of items removal, if any.
		 */
		void handleRemovedCLItems (const QList<QObject*>& items);

		/** Handles the status change of an account to new status.
		 */
		void handleAccountStatusChanged (const EntryStatus& status);

		/** Handles the status change of a CL entry to new status.
		 */
		void handleStatusChanged (const EntryStatus& status, const QString& variant);

		/** Handles the event of name changes in plugin.
		 */
		void handleEntryNameChanged (const QString& newName);

		/** Handles the event of groups change in plugin.
		 */
		void handleEntryGroupsChanged (QStringList);

		/** Handles the message receival from contact list entries.
		 */
		void handleEntryGotMessage (QObject *msg);

		/** Handles the authorization requests from accounts.
		 */
		void handleAuthorizationRequested (QObject*, const QString&);

		void handleItemSubscribed (QObject*, const QString&);
		void handleItemUnsubscribed (QObject*, const QString&);
		void handleItemUnsubscribed (const QString&, const QString&);
		void handleItemCancelledSubscription (QObject*, const QString&);
		void handleItemGrantedSubscription (QObject*, const QString&);

		/** Is registered in the XmlSettingsManager as handler for
		 * changes of the "StatusIcons" property.
		 */
		void updateStatusIconset ();

		/** This slot is used to update the model item which is
		 * corresponding to the sender() which is expected to be a
		 * ICLEntry.
		 */
		void updateItem ();

		/** Asks the corresponding CL entry to show its dialog with
		 * information about the user.
		 */
		void showVCard ();

		/** Handles the number of unread messages for the given contact
		 * list entry identified by object. Object should implement
		 * ICLEntry, obviously.
		 */
		void handleClearUnreadMsgCount (QObject *object);
		
		void handleFileOffered (QObject*);
		void handleJobDeoffered (QObject*);

		/** Removes the entries in the client icon cache for the sender,
		 * if obj is null, or for obj, if it is not null.
		 *
		 * If the object can't be casted to ICLEntry, this function does
		 * nothing.
		 */
		void invalidateClientsIconCache (QObject *obj = 0);
		void invalidateClientsIconCache (ICLEntry*);

		void invalidateSmoothAvatarCache ();

		void handleActionRenameTriggered ();
		void handleActionChangeGroupsTriggered ();
		void handleActionRemoveTriggered ();
		void handleActionRevokeAuthTriggered ();
		void handleActionUnsubscribeTriggered ();
		void handleActionRerequestTriggered ();
		void handleActionVCardTriggered ();

		void handleActionOpenChatTriggered ();
		void handleActionLeaveTriggered ();
		void handleActionAuthorizeTriggered ();
		void handleActionDenyAuthTriggered ();

		void handleActionRoleTriggered ();
		void handleActionAffTriggered ();
	signals:
		void gotEntity (const LeechCraft::Entity&);
		void delegateEntity (const LeechCraft::Entity&, int*, QObject**);

		/** Emitted after some new account creation actions have been
		 * received from a plugin and thus should be shown in the UI.
		 */
		void accountCreatorActionsAdded (const QList<QAction*>&);

		/** Convenient signal for rethrowing the event of an account
		 * being added.
		 */
		void accountAdded (IAccount*);

		/** Convenient signal for rethrowing the event of an account
		 * being removed.
		 */
		void accountRemoved (IAccount*);

		// Plugin API
		void hookFormatDateTime (LeechCraft::IHookProxy_ptr proxy,
				QObject *chatTab,
				QDateTime dateTime,
				QObject *message);
		void hookFormatNickname (LeechCraft::IHookProxy_ptr proxy,
				QObject *chatTab,
				QString nick,
				QObject *message);
		void hookFormatBodyBegin (LeechCraft::IHookProxy_ptr proxy,
				QObject *chatTab,
				QString body,
				QObject *message);
		void hookFormatBodyEnd (LeechCraft::IHookProxy_ptr proxy,
				QObject *chatTab,
				QString body,
				QObject *message);
		void hookGotMessage (LeechCraft::IHookProxy_ptr proxy,
				QObject *message);
	};
}
}

Q_DECLARE_METATYPE (LeechCraft::Azoth::Core::CLEntryType);
Q_DECLARE_METATYPE (LeechCraft::Azoth::Core::CLEntryActionArea);
Q_DECLARE_METATYPE (LeechCraft::Azoth::ICLEntry*);

#endif

