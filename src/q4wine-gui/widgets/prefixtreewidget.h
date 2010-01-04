/***************************************************************************
 *   Copyright (C) 2008, 2009, 2010 by Malakhov Alexey                     *
 *   brezerk@gmail.com                                                     *
 *                                                                         *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 ***************************************************************************/

#ifndef PREFIXTREEWIDGET_H
#define PREFIXTREEWIDGET_H

#include "memory"

#include "config.h"

#include "src/core/database/prefix.h"
#include "src/core/database/dir.h"
#include "src/core/database/icon.h"
#include "src/core/database/last_run_icon.h"

#include "run.h"

#include "iconsettings.h"

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QMouseEvent>
#include <QPoint>
#include <QApplication>
#include <QUrl>
#include <QAction>
#include <QMenu>
#include <QLibrary>

#ifdef DEBUG
#include <QDebug>
#endif

#include <q4wine-lib/main.h>

class PrefixTreeWidget : public QTreeWidget
{
Q_OBJECT
public:
      explicit PrefixTreeWidget(QString themeName, QWidget *parent = 0);
      ~PrefixTreeWidget();

private:
      void contextMenuEvent(QContextMenuEvent *event);

      QIcon loadIcon(QString iconName);

      QString themeName, prefixName, dirName, prefixMediaDrive, prefixMontPoint;

      //! This is need for libq4wine-core.so import.
      QLibrary libq4wine;
      typedef void *CoreLibPrototype (bool);
      CoreLibPrototype *CoreLibClassPointer;
      std::auto_ptr<corelib> CoreLib;

      // Database classes
      Image db_image;
      Prefix db_prefix;
      Dir db_dir;
      Icon db_icon;

private slots:
      void itemClicked (QTreeWidgetItem *item, int);

      void dirAdd_Click(void);
      void dirRename_Click(void);
      void dirDelete_Click(void);
      void menuRun_triggered(QAction*);
      void menuMount_triggered(QAction*);
      void menuUmount_Click(void);

      void xdgOpenPrefixDir_Click(void);
      void xdgOpenMountDir_Click(void);

      void winefileOpenPrefixDir_Click(void);
      void winefileOpenMountDir_Click(void);

      void setDefaultFocus(QString prefixName, QString dirName="");

signals:
      void showFolderContents(QString, QString);
      void changeStatusText(QString);
      void prefixIndexChanged(QString);

public slots:
      void getPrefixes();

};

#endif // PREFIXTREEWIDGET_H