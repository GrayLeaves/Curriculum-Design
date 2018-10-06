#include "recentopened.h"
#include <QMenu>
#include <QSettings>
#include <QFileInfo>
#include <QAction>
#include <QDebug>
RecentOpened::RecentOpened(QMenu *menu)
{
    recentMenu = menu;
    clearOpenedHistoryAct = recentMenu->actions().at(0);
    initRecent();
}

void RecentOpened::initRecent()
{
    QSettings settings("settings.ini",QSettings::IniFormat);
    settings.beginGroup("RecentlyOpened");
    QStringList keys,values;
    keys = settings.childKeys();
    for(int i=0; i<keys.size(); i++)
        values << settings.value(keys[i]).toString();
    settings.endGroup();

    if(keys.size())
    {
        clearOpenedHistoryAct->setEnabled(true);
        clearOpenedHistoryAct->setText("清除记录");
    }
    for(int i=0;i<values.size();i++)
    {
        QFileInfo info(values[i]);
        QString item = QString("%1--%2").arg(i+1).arg(info.fileName());
        QAction *recent = new QAction(QIcon(values[i]),item,this);
        recent->setToolTip(values[i]);
        connect(recent,&QAction::triggered,this,&RecentOpened::actionTriggered);
        recentMenu->insertAction(clearOpenedHistoryAct,recent);
    }
}

void RecentOpened::updateList(const QString &file)
{
    QStringList keys,values;
    QSettings settings("settings.ini",QSettings::IniFormat);
    settings.beginGroup("RecentlyOpened");
    keys = settings.childKeys();
    for(int i=0;i<keys.size();i++)
        values << settings.value(keys[i]).toString();
    settings.endGroup();

    int index = values.indexOf(file);
    if(index != -1)
    {
        values.removeAt(index);
    }

    QFileInfo fi(file);
    if(fi.exists())
    {
        values.insert(0,file);
    }
    keys.clear();
    for(int i=0;i<values.size();i++)
    {
        keys << QString::number(i+1);
    }

    settings.remove("RecentlyOpened");
    settings.beginGroup("RecentlyOpened");
    for(int i=0;i<values.size();i++)
    {
        settings.setValue(keys[i],values[i]);
    }
    settings.endGroup();

    QList<QAction *> actions = recentMenu->actions();
    for(int i=0;i<actions.size()-1;i++)
    {
        recentMenu->removeAction(actions[i]);
    }

    for(int i=0;i<values.size();i++)
    {
        QFileInfo info(values[i]);
        QString item = QString("%1 %2").arg(i+1).arg(info.fileName());
        QAction *recent = new QAction(QIcon(values[i]),item,this);
        recent->setToolTip(values[i]);
        connect(recent,&QAction::triggered,this,&RecentOpened::actionTriggered);
        recentMenu->insertAction(clearOpenedHistoryAct,recent);
    }

    clearOpenedHistoryAct->setEnabled(true);
    clearOpenedHistoryAct->setText("清除记录");
}

void RecentOpened::actionTriggered()
{
    int index=0;
    QList<QAction *> actions = recentMenu->actions();
    for(int i=0;i<actions.size()-1;i++)
    {
        if(sender() == actions[i])
        {
            index = i+1;
            emit selectedItem(i+1);
            break;
        }
    }
    QSettings settings("settings.ini",QSettings::IniFormat);
    settings.beginGroup("RecentlyOpened");
    this->updateList(settings.value(QString::number(index)).toString());
    settings.endGroup();
}

void RecentOpened::clearActions()
{
    QSettings settings("settings.ini",QSettings::IniFormat);
    settings.remove("RecentlyOpened");

    QList<QAction *> actions = recentMenu->actions();

    if(actions.size()>1)
    {
        for(int i=0;i<actions.size()-1;i++)
        {
            recentMenu->removeAction(actions[i]);
        }
        clearOpenedHistoryAct->setEnabled(false);
        clearOpenedHistoryAct->setText("(零记录)");
    }
}
