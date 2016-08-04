/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
****************************************************************************/

#pragma once

#include <sourcelocationscontainer.h>

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

#include <clang/Basic/SourceManager.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/FileUtilities.h>

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

namespace ClangBackEnd {

inline
Utils::SmallString absolutePath(const char *path)
{
    llvm::SmallString<256> absolutePath(path);

    if (!llvm::sys::path::is_absolute(absolutePath))
        llvm::sys::fs::make_absolute(absolutePath);

    return Utils::SmallString(absolutePath.begin(), absolutePath.end());
}

inline
void appendSourceLocationsToSourceLocationsContainer(
        ClangBackEnd::SourceLocationsContainer &sourceLocationsContainer,
        const std::vector<clang::SourceLocation> &sourceLocations,
        const clang::SourceManager &sourceManager)
{
    sourceLocationsContainer.reserve(sourceLocations.size());

    for (const auto &sourceLocation : sourceLocations) {
        clang::FullSourceLoc fullSourceLocation(sourceLocation, sourceManager);
        auto fileId = fullSourceLocation.getFileID();
        auto fileEntry = sourceManager.getFileEntryForID(fileId);
        auto fileName = fileEntry->getName();
        auto fileDirectoryPath = absolutePath(fileEntry->getDir()->getName());

        sourceLocationsContainer.insertFilePath(fileId.getHashValue(),
                                                std::move(fileDirectoryPath),
                                                fileName);
        sourceLocationsContainer.insertSourceLocation(fileId.getHashValue(),
                                                      fullSourceLocation.getSpellingLineNumber(),
                                                      fullSourceLocation.getSpellingColumnNumber());
    }
}

} // namespace ClangBackEnd