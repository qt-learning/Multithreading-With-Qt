// Copyright (C) 2026 Qt Group.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef RAYCASTER_H
#define RAYCASTER_H

#include <QRect>
#include <QImage>
#include <QSharedPointer>

class RayCaster
{
public:
    RayCaster();

    QImage render(const QSize &viewport, qreal dpi, bool *cancelFlag);
};

#endif // RAYCASTER_H
