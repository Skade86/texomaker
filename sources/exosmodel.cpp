#include "exosmodel.h"
#include "preferences.h"

ExosModel::ExosModel(const QList<QMap<QString,QString> > & exos, QObject * parent) : QAbstractTableModel(parent)
{
	QStringList curList=QStringList();
	QStringList emptyList=QStringList();
	QStringList headerList = Preferences::p_getMetaToView();
	
	QStringList tempList = Preferences::p_getMetaList();
	
	for (int i=0;i<tempList.size();i++)
	{
		if (!headerList.contains(tempList.at(i))) headerList << tempList.at(i);
		emptyList.insert(i,QString());
	}
	
	curList=emptyList;
	exosList.append(headerList);
	
	QMap<QString,QString> exoMap;
	
	// On crée la QList 
	for (int i=0;i<exos.size();i++)		// On itère sur les exos
		{
			exoMap=exos.at(i);
			QMapIterator<QString, QString> j(exoMap);
			j.toFront();
			
			while (j.hasNext())
			{
				j.next();
				if (curList.at(headerList.indexOf(j.key())).isEmpty())
				{
					curList.replace(headerList.indexOf(j.key()),j.value());
				}
				else curList.replace(headerList.indexOf(j.key()),curList.at(headerList.indexOf(j.key()))+";"+j.value());
			}
			
			exosList.append(curList);
			curList=emptyList; 
			exoMap.clear();
		}
}

int ExosModel::rowCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent);
	
	return exosList.count()-1;
}

int ExosModel::columnCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent);
	
	return exosList.at(0).count();
}

QVariant ExosModel::headerData(int section, Qt::Orientation orientation, int role) const 
{
	if (orientation == Qt::Horizontal)	{
		if (role == Qt::DisplayRole)	{
			return exosList.at(0).at(section);
			}
		}
	return QAbstractTableModel::headerData(section, orientation, role);
}

QVariant ExosModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid()) return QVariant();
	QStringList exoRecord = exosList.at(index.row()+1);
	
	if (role == Qt::DisplayRole)
		{
			return exoRecord.at(index.column());
		}
		
	if (role == Qt::ToolTipRole)
		{
			QString tip, key, value;
			tip = "<table>";
			int  maxLines = exoRecord.count();
			
			for (int i=0; i<maxLines; i++) 
				{
					key = headerData(i, Qt::Horizontal, Qt::DisplayRole).toString();
					value = exoRecord.at(i);
					if (!value.isEmpty()) tip += QString("<tr><td><b>%1</b>: %2</td></tr>").arg(key, value);
				}
	
			tip += "</table>";
			return tip;
		} 
		
	return QVariant();
}

bool ExosModel::setData(const QModelIndex &index,  const QVariant &value, int role)
{
	if (index.isValid() && role == Qt::DisplayRole)
		{
			exosList[index.row()+1][index.column()] = value.toString();
			emit dataChanged(index,index);
			return true;
		}
	return false;
}

bool ExosModel::insertRows(int row, int count, const QModelIndex &parent)
{
	Q_UNUSED(parent);
	
	QStringList emptyRecord;
	
	for (int i = 0; i < columnCount(QModelIndex()); i++)
		emptyRecord.append(QString());
		
	beginInsertRows(QModelIndex(),row,row+count-1);
	for (int i=0; i<count; i++)
		exosList.insert(row+1,emptyRecord);
	endInsertRows();
	
	return true;
}

bool ExosModel::removeRows(int row, int count, const QModelIndex &parent)
{
	Q_UNUSED(parent);
	if (row+count-1 > exosList.count()-1) return false;
	
	beginRemoveRows(QModelIndex(),row,row+count-1);
	for (int i=0; i<count; i++)
		exosList.removeAt(row+i);
	endRemoveRows();
    endResetModel();
	return true;
}

void ExosModel::updateModel(QMap<QString,QString> newRowMap, bool update)
{
	if (!update)
	{	
		QStringList curList=QStringList();
		QStringList headerList = Preferences::p_getMetaToView();
		QStringList tempList = Preferences::p_getMetaList();
		
		for (int i=0;i<tempList.size();i++)
		{
			if (!headerList.contains(tempList.at(i))) headerList << tempList.at(i);
			curList.insert(i,QString());
		}

		QMapIterator<QString, QString> j(newRowMap);
			
		while (j.hasNext())
		{
			j.next();
			if (curList.at(headerList.indexOf(j.key())).isEmpty())
			{
				curList.replace(headerList.indexOf(j.key()),j.value());
			}
			else curList.replace(headerList.indexOf(j.key()),curList.at(headerList.indexOf(j.key()))+"; "+j.value());
		}

		exosList.append(curList);
	}
		else
		{
			QStringList toUpdate;
			QString header;
			QString filePath;
			
			for (int i=0;i<Preferences::p_getMetaSize();i++) 
			{
				header = headerData(i,Qt::Horizontal,Qt::DisplayRole).toString();
				toUpdate << newRowMap.value(header);
				if (header=="filepath") filePath=newRowMap.value(header);
			}
			
			for (int i = 0; i < exosList.size(); ++i)
			{
				if (exosList.at(i).contains(filePath)) exosList.replace(i,toUpdate);
			}
		}
    endResetModel();
}
