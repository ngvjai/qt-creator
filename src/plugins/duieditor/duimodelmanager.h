/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** Commercial Usage
**
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
**
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at http://qt.nokia.com/contact.
**
**************************************************************************/

#ifndef DUIMODELMANAGER_H
#define DUIMODELMANAGER_H

#include <QFuture>
#include <QFutureSynchronizer>
#include <QMutex>

#include "duidocument.h"
#include "duimodelmanagerinterface.h"

namespace Core {
class ICore;
}

namespace DuiEditor {
namespace Internal {

class DuiModelManager: public DuiModelManagerInterface
{
    Q_OBJECT

public:
    DuiModelManager(QObject *parent = 0);

    virtual Snapshot snapshot() const;
    virtual void updateSourceFiles(const QStringList &files);

    void emitDocumentUpdated(DuiDocument::Ptr doc);

Q_SIGNALS:
    void projectPathChanged(const QString &projectPath);

    void documentUpdated(DuiDocument::Ptr doc);
    void aboutToRemoveFiles(const QStringList &files);

private Q_SLOTS:
    // this should be executed in the GUI thread.
    void onDocumentUpdated(DuiDocument::Ptr doc);

protected:
    QFuture<void> refreshSourceFiles(const QStringList &sourceFiles);
    QMap<QString, QString> buildWorkingCopyList();

    static void parse(QFutureInterface<void> &future,
                      QMap<QString, QString> workingCopy,
                      QStringList files,
                      DuiModelManager *modelManager);

private:
    mutable QMutex m_mutex;
    Core::ICore *m_core;
    Snapshot _snapshot;

    QFutureSynchronizer<void> m_synchronizer;
};

} // namespace Internal
} // namespace DuiEditor

#endif // DUIMODELMANAGER_H
