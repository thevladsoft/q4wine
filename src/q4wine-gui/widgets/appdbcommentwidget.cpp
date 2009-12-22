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

#include "appdbcommentwidget.h"

AppDBCommentWidget::AppDBCommentWidget(const WineAppDBComment *comment, QWidget * parent) : QFrame(parent)
{
	setupUi(this);
	this->setObjectName(QString("%1").arg(comment->id));
	this->setId(comment->id);
	this->setParentId(comment->padent_id);
	setTopic(comment->topic);
	setDate(comment->autor, comment->date);
	setMessage(comment->message);


	return;
}

void AppDBCommentWidget::setTopic(QString topic){
	//
	AppDBLinkItemWidget *label = new AppDBLinkItemWidget(topic, 7);
	label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	label->setBold(true);
	label->setWordWrap(true);
	if (ParentId>0){
		label->setEnabled(true);
		label->setParentId(ParentId);
		connect (label, SIGNAL(linkTrigged(short int, QString, int, int)), this, SIGNAL(linkTrigged(short int, QString, int, int)));
	} else {
		label->setEnabled(false);
	}

	widgetLabelLayout->addWidget(label);
	QPalette p(palette());
	//if (topic)
	//FIXME: check for WARNING and HOWTO colors
	p.setColor(QPalette::Background, QPalette().color(QPalette::Dark));
	widgetLabel->setPalette(p);
	widgetLabel->setAutoFillBackground(true);
	return;
}

void AppDBCommentWidget::setDate(QString autor, QString date){
	QLabel *label = new QLabel();

	label->setText(QString("by %1 on %2").arg(autor).arg(date));
	widgetLabelLayout->addWidget(label);
	return;
}

void AppDBCommentWidget::setMessage(QString message){
	lblContent->setText(message);
	return;
}

void AppDBCommentWidget::setId(int id){
	this->Id=id;
	return;
}

void AppDBCommentWidget::setParentId(int id){
	this->ParentId=id;
	return;
}

bool AppDBCommentWidget::isId(int id){
	if (Id==id){
		return true;
	} else {
		return false;
	}
}

