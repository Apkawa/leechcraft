#include "importopml.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDomDocument>
#include "opmlparser.h"

ImportOPML::ImportOPML (QWidget *parent)
: QDialog (parent)
{
	Ui_.setupUi (this);
	Ui_.ButtonBox_->button (QDialogButtonBox::Open)->setEnabled (false);
}

ImportOPML::~ImportOPML ()
{
}

QString ImportOPML::GetFilename () const
{
	return Ui_.File_->text ();
}

QString ImportOPML::GetTags () const
{
	return Ui_.AdditionalTags_->text ().trimmed ();
}

std::vector<bool> ImportOPML::GetMask () const
{
	std::vector<bool> result (Ui_.FeedsToImport_->topLevelItemCount ());

	for (int i = 0, items = Ui_.FeedsToImport_->topLevelItemCount ();
			i < items; ++i)
		result [i] = (Ui_.FeedsToImport_->topLevelItem (i)->
				data (0, Qt::CheckStateRole) == Qt::Checked);

	return result;
}

void ImportOPML::on_File__textEdited (const QString& newFilename)
{
	Reset ();

	if (QFile (newFilename).exists ())
		Ui_.ButtonBox_->button (QDialogButtonBox::Open)->
			setEnabled (HandleFile (newFilename));
	else
		Reset ();
}

void ImportOPML::on_Browse__released ()
{
	QString filename = QFileDialog::getOpenFileName (this,
			tr ("Select OPML file"),
			QDir::homePath (),
			tr ("OPML files (*.opml);;"
				"XML files (*.xml);;"
				"All files (*.*)"));

	if (filename.isEmpty ())
		return;

	Reset ();

	Ui_.File_->setText (filename);

	Ui_.ButtonBox_->button (QDialogButtonBox::Open)->
		setEnabled (HandleFile (filename));
}

bool ImportOPML::HandleFile (const QString& filename)
{
	QFile file (filename);
	if (!file.open (QIODevice::ReadOnly))
	{
		QMessageBox::critical (this,
				tr ("Error"),
				tr ("Could not open file %1 for reading.")
					.arg (filename));
		return false;
	}

	QByteArray data = file.readAll ();
	file.close ();

	QString errorMsg;
	int errorLine, errorColumn;
	QDomDocument document;
	if (!document.setContent (data,
				true,
				&errorMsg,
				&errorLine,
				&errorColumn))
	{
		QMessageBox::critical (this,
				tr ("Error"),
				tr ("XML error, file %1, line %2, column %3, error:<br />%4")
					.arg (filename)
					.arg (errorLine)
					.arg (errorColumn)
					.arg (errorMsg));
		return false;
	}

	OPMLParser parser (document);
	if (!parser.IsValid ())
	{
		QMessageBox::critical (this,
				tr ("Error"),
				tr ("OPML from file %1 is not valid.")
					.arg (filename));
		return false;
	}

	OPMLParser::OPMLinfo_t info = parser.GetInfo ();

	for (OPMLParser::OPMLinfo_t::const_iterator i = info.begin (),
			end = info.end (); i != end; ++i)
	{
		QString name = i.key ();
		QString value = i.value ();

		if (name == "title")
			Ui_.Title_->setText (value);
		else if (name == "dateCreated")
			Ui_.Created_->setText (value);
		else if (name == "dateModified")
			Ui_.Edited_->setText (value);
		else if (name == "ownerName")
			Ui_.Owner_->setText (value);
		else if (name == "ownerEmail")
			Ui_.OwnerEmail_->setText (value);
		else
		{
			QStringList strings;
			strings << name << value;
			new QTreeWidgetItem (Ui_.OtherFields_, strings);
		}
	}

	OPMLParser::items_container_t items = parser.Parse ();
	for (OPMLParser::items_container_t::const_iterator i = items.begin (),
			end = items.end (); i != end; ++i)
	{
		QStringList strings;
		strings << i->Title_ << i->URL_;
		QTreeWidgetItem *item =
			new QTreeWidgetItem (Ui_.FeedsToImport_, strings);
		item->setData (0, Qt::CheckStateRole, Qt::Checked);
	}

	return true;
}

void ImportOPML::Reset ()
{
	Ui_.Title_->setText ("");
	Ui_.Created_->setText ("");
	Ui_.Edited_->setText ("");
	Ui_.Owner_->setText ("");
	Ui_.OwnerEmail_->setText ("");
	Ui_.OtherFields_->clear ();
	Ui_.FeedsToImport_->clear ();

	Ui_.ButtonBox_->button (QDialogButtonBox::Open)->setEnabled (false);
}

