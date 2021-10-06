/*
 * Rosalie's Mupen GUI - https://github.com/Rosalie241/RMG
 *  Copyright (C) 2020 Rosalie Wanders <rosalie@mailbox.org>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 3.
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <https://www.gnu.org/licenses/>.
 */
#include "RomSearcherThread.hpp"
#include "Globals.hpp"

#include <QDirIterator>

using namespace Thread;

RomSearcherThread::RomSearcherThread(QObject *parent) : QThread(parent)
{
    qRegisterMetaType<M64P::Wrapper::RomInfo_t>("M64P::Wrapper::RomInfo_t");
}

RomSearcherThread::~RomSearcherThread(void)
{
    // kill the thread when we're running
    if (this->isRunning())
    {
        this->terminate();
        this->wait();
    }
}

void RomSearcherThread::SetDirectory(QString directory)
{
    this->rom_Directory = directory;
}

void RomSearcherThread::SetRecursive(bool value)
{
    this->rom_Search_Recursive = value;
}

void RomSearcherThread::SetMaximumFiles(int value)
{
    this->rom_Search_MaxItems = value;
}

void RomSearcherThread::run(void)
{
    this->rom_Search_Count = 0;
    this->rom_Search(this->rom_Directory);
    return;
}

void RomSearcherThread::rom_Search(QString directory)
{
    QStringList filter;
    filter << "*.N64";
    filter << "*.Z64";
    filter << "*.V64";
    filter << "*.NDD";
    filter << "*.D64";

    QDirIterator::IteratorFlag flag = this->rom_Search_Recursive ? 
        QDirIterator::Subdirectories : 
        QDirIterator::NoIteratorFlags;
    QDirIterator romDirIt(directory, filter, QDir::Files, flag);

    M64P::Wrapper::RomInfo_t romInfo;

    while (romDirIt.hasNext())
    {
        QString file = romDirIt.next();

        if (!g_RomBrowserCache.ContainsEntry(file))
        { // when not found in cache, try to add it
            if (g_MupenApi.Core.GetRomInfo(file, &romInfo, true))
            {
                g_RomBrowserCache.AddEntry(file, romInfo);
            }
            else
            { // skip file
                continue;
            }
        }
        else
        { // when found in cache, retrieve entry
            romInfo = g_RomBrowserCache.GetEntry(file);
        }

        // make sure we don't go over the
        // search limit
        this->rom_Search_Count++;
        if (this->rom_Search_Count > this->rom_Search_MaxItems)
        {
            return;
        }
        
        emit this->on_Rom_Found(romInfo);
    }
}
