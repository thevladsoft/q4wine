/***************************************************************************
 *   Copyright (C) 2008 by Malakhov Alexey                                 *
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
 *   In addition, as a special exception, the copyright holders give       *
 *   permission to link the code of this program with any edition of       *
 *   the Qt library by Trolltech AS, Norway (or with modified versions     *
 *   of Qt that use the same license as Qt), and distribute linked         *
 *   combinations including the two.  You must obey the GNU General        *
 *   Public License in all respects for all of the code used other than    *
 *   Qt.  If you modify this file, you may extend this exception to        *
 *   your version of the file, but you are not obligated to do so.  If     *
 *   you do not wish to do so, delete this exception statement from        *
 *   your version.                                                         *
 ***************************************************************************/

#ifndef APPDBAPPVERSIONWIDGET_H
#define APPDBAPPVERSIONWIDGET_H

#include "config.h"

#include "memory"

#include <QObject>
#include <QWidget>
#include <QString>
#include <QDebug>
#include <QLabel>
#include <QBoxLayout>
#include <QEvent>
#include <QFont>

#include "appdbstructs.h"

/*!
 * \class AppDBAppVersionWidget
 * \ingroup widgets
 * \brief This class provide database functions for AppDB search widget.
 *
 */

class AppDBAppVersionWidget : public QWidget
{
Q_OBJECT
public:
	//! Class constructor
	AppDBAppVersionWidget(const short int action, QWidget *parent = 0);

	//! Class destructor
	~AppDBAppVersionWidget();

	void addLabel(const QString text, const short int width=-1, const short int aligment=0, const bool worldwarp=false);
	void insertStretch(void);
	void setBold(void);

	void setAction(short int action);
	void setAppId(int id);
	void setAppVerId(int id);
	void setTestId(int id);
	void setEnabled(bool enable);

private:
	std::auto_ptr<QBoxLayout> contentLayout;
	int appid;
	int verid;
	int testid;
	short int action;

signals:
	void itemTrigged(short int, QString, int, int, int);

protected:
	//! \brief Event filter.
	bool eventFilter(QObject *obj, QEvent *event);

};

#endif // APPDBAPPVERSIONWIDGET_H