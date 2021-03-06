/***************************************************************************
 *   Copyright (C) 2008-2013 by Alexey S. Malakhov <brezerk@gmail.com>     *
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

#include "httpcore.h"

HttpCore::HttpCore()
{
    http.reset(new QHttp(this));

    connect(http.get(), SIGNAL(requestFinished(int, bool)),
                 this, SLOT(httpRequestFinished(int, bool)));
    connect(http.get(), SIGNAL(dataReadProgress(int, int)),
                 this, SIGNAL(updateDataReadProgress(int, int)));
    connect(http.get(), SIGNAL(stateChanged(int)),
                 this, SIGNAL(stateChanged(int)));
    connect(http.get(), SIGNAL(responseHeaderReceived(const QHttpResponseHeader &)),
                 this, SLOT(readResponseHeader(const QHttpResponseHeader &)));

    // Loading libq4wine-core.so
#ifdef RELEASE
    libq4wine.setFileName(_CORELIB_PATH_);
#else
    libq4wine.setFileName("../q4wine-lib/libq4wine-core");
#endif

    if (!libq4wine.load()){
        libq4wine.load();
    }

    // Getting corelib calss pointer
    CoreLibClassPointer = (CoreLibPrototype*) libq4wine.resolve("createCoreLib");
    CoreLib.reset((corelib*) CoreLibClassPointer(true));

    int type = CoreLib->getSetting("network", "type", false).toInt();

    if (type!=0){
        QString proxyUser, proxyPass, proxyHost;
        int proxyPort;

        proxyHost = CoreLib->getSetting("network", "host", false).toString();
        proxyPort = CoreLib->getSetting("network", "port", false).toInt();
        proxyUser = CoreLib->getSetting("network", "user", false).toString();
        proxyPass = CoreLib->getSetting("network", "pass", false).toString();
        http->setProxy(proxyHost, proxyPort, proxyUser, proxyPass);
    }

    //Create user_agent string
    user_agent=QString("%1/%2 (X11; %3 %4) xmlparser/%5").arg(APP_SHORT_NAME).arg(APP_VERS).arg(APP_HOST).arg(APP_ARCH).arg(APPDB_EXPORT_VERSION);
}

HttpCore::~HttpCore(){
    //Nothing but...
}

void HttpCore::getAppDBXMLPage(QString host, short int port, QString page)
{
    this->page = page;
    if (!this->getCacheFile(page)){

        http->setHost(host, QHttp::ConnectionModeHttp, port);

        QByteArray enc_page = QUrl::toPercentEncoding(page, "/");

        QHttpRequestHeader header("POST", enc_page);
        header.setValue("Host", host);
        header.setValue("Port", QString("%1").arg(port));
        header.setContentType("application/x-www-form-urlencoded");
        header.setValue("Accept-Encoding", "deflate");
        header.setValue("User-Agent", user_agent);

#ifdef DEBUG
        qDebug()<<"[ii] Connecting to"<<host<<":"<<port<<" reuested page is: "<<enc_page;
#endif

        this->xmlreply="";
        this->aborted=false;
        this->getId = http->request(header, "");
    } else {
#ifdef DEBUG
        qDebug()<<"[ii] Cache hit";
#endif
        emit(pageReaded());
    }
}

bool HttpCore::getCacheFile(QString page){
    QString cache_file = QDir::homePath();
    cache_file.append("/.config/");
    cache_file.append(APP_SHORT_NAME);
    cache_file.append("/tmp/cache/");
    cache_file.append(QCryptographicHash::hash(page.toUtf8().constData(), QCryptographicHash::Md4).toHex());

    QFile file(cache_file);
    if (file.exists()){
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return false;

        QTextStream in(&file);
        this->xmlreply.clear();
        while (!in.atEnd()) {
            xmlreply.append(in.readLine());
        }
        return true;
    }

    return false;
}

QString HttpCore::getXMLReply(){
    QString cache_file = QDir::homePath();
    cache_file.append("/.config/");
    cache_file.append(APP_SHORT_NAME);
    cache_file.append("/tmp/cache/");
    cache_file.append(QCryptographicHash::hash(this->page.toUtf8().constData(), QCryptographicHash::Md4).toHex());

    QFile file(cache_file);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)){
         QTextStream out(&file);
         out << xmlreply;
     }
    file.close();
    return xmlreply;
}

void HttpCore::httpRequestFinished(int requestId, bool error){
    if (this->aborted)
        return;

    if (this->getId!=requestId)
        return;

    if (error){
        xmlreply="";
        emit(requestError(http->errorString()));
    } else {
        xmlreply=http->readAll();

#ifdef DEBUG
        qDebug()<<"[ii] Received page:"<<xmlreply;
#endif
        emit(pageReaded());
    }

    return;
}

void HttpCore::readResponseHeader(const QHttpResponseHeader &responseHeader){
    switch (responseHeader.statusCode()) {
         case 200:                   // Ok
         case 301:                   // Moved Permanently
         case 303:                   // See Other
         case 307:                   // Temporary Redirect
             // these are not error conditions
             break;

         default:
             emit(requestError(tr("Download failed: %1.").arg(responseHeader.reasonPhrase())));;
             this->aborted=true;
             http->abort();
         }
    return;
}

