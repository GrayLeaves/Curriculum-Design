#ifndef RECENTOPENED_H
#define RECENTOPENED_H

#include <QWidget>
class QMenu;
class QAction;

class RecentOpened : public QWidget
{
    Q_OBJECT
public:
    RecentOpened(QMenu *menu);
    ~RecentOpened(){}
    void initRecent();
    void updateList(const QString &file);
    void clearActions();

signals:
    void selectedItem(int i);

public slots:
    void actionTriggered();

private:
    QMenu *recentMenu;
    QAction *clearOpenedHistoryAct;
};

#endif // RECENTOPENED_H
