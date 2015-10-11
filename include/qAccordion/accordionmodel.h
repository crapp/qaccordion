// This file is part of qAccordion. An Accordion widget for Qt
// Copyright © 2015 Christian Rapp <0x2a at posteo dot org>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef ACCORDIONMODEL_H
#define ACCORDIONMODEL_H

#include <QObject>

#include <vector>

#include "contentpane.h"

class AccordionModel : public QObject
{
    Q_OBJECT
public:
    AccordionModel();

    void setCurrentlyOpen(ContentPane *pane);
    ContentPane* getCurrentlyOpen();

signals:

public slots:

private:
    ContentPane *currentlyOpen;

    std::vector<ContentPane *> contentPanes;
};

#endif // ACCORDIONMODEL_H
