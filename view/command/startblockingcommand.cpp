/*
    LBSim: a fluid dynamics simulator using the lattice Boltzmann method
    Copyright (C) 2013 Fabio Sussumu Komori - NDS/GNMD/LME/PSI/EP/USP

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "startblockingcommand.h"
#include <QMutex>
#include <QWaitCondition>
#include "../../model/latticeboltzmann/lbgrid.h"
#include "../../view/lbmainwindow.h"
#include <QDebug>

StartBlockingCommand::StartBlockingCommand(Grid *grid, LBMainWindow *mainWindow) : Command(grid, mainWindow) {
}

QString StartBlockingCommand::name() {
    return "startBlocking";
}

QString StartBlockingCommand::help() {
    return "startBlocking";
}

QString StartBlockingCommand::execute() {
    QMutex mutex;
    QWaitCondition waitCondition;
    QMetaObject::invokeMethod(mainWindow, "on_actionStart_triggered");
    while (!canceled) {
        mutex.lock();
        waitCondition.wait(&mutex, 1000);
        mutex.unlock();
        //sleep(1);
        if (!grid->isRunning()) {
            break;
        }
    }
    QMetaObject::invokeMethod(mainWindow, "on_actionStop_triggered");
    mutex.lock();
    waitCondition.wait(&mutex, 1000);
    mutex.unlock();
    return "";
}

Command* StartBlockingCommand::clone() {
    return (new StartBlockingCommand(grid, mainWindow))->copy(this);
}