#pragma once

#ifdef OS_WINDOWS
#include <windows.h>
#endif
#include <unrar.h>

#include <QString>
#include <set>

template<typename T>
QString rarOpenErrorToString(const T &error)
{
    switch (error)
    {
        case ERAR_SUCCESS         : return "Success.";
        case ERAR_NO_MEMORY       : return "Not enough memory to initialize data structures.";
        case ERAR_BAD_DATA        : return "Archive header broken!";
        case ERAR_UNKNOWN_FORMAT  : return "Unknown encryption used for archive headers!";
        case ERAR_EOPEN           : return "Cannot open archive!";
        case ERAR_BAD_PASSWORD    : return "Entered password is invalid!";
        default                   : return "Unknown Error: " + QString::number(error);
    }
}

template<typename T>
QString rarProcessFileErrorToString(const T &error)
{
    switch (error)
    {
        case ERAR_NO_MEMORY          : return "Not enough memory";
        case ERAR_BAD_DATA           : return "File CRC error";
        case ERAR_UNKNOWN_FORMAT     : return "Unknown archive format";
        case ERAR_EOPEN              : return "Volume open error";
        case ERAR_ECREATE            : return "File create error";
        case ERAR_ECLOSE             : return "File close error";
        case ERAR_EREAD              : return "Read error";
        case ERAR_EWRITE             : return "Write error";
        case ERAR_EREFERENCE         : return "ERAR_EREFERENCE";
        case ERAR_BAD_PASSWORD       : return "Entered password is invalid";
        default                      : return "Unknown Error: " + QString::number(error);
    }
}

template<typename T>
QString rarHeaderErrorToString(const T &error)
{
    switch (error)
    {
        case ERAR_END_ARCHIVE        : return "End of archive";
        case ERAR_BAD_DATA           : return "File header broken";
        case ERAR_MISSING_PASSWORD   : return "Password was not provided for encrypted file header";
        default                      : return "Unknown Error: " + QString::number(error);
    }
}

namespace unrarhelpers_impl
{
template<typename T, typename U>
T parseArchiveFlag(const T &flags, std::set<QString> &parsedFlags, U flag, QString name)
{
    if ((flags & flag) == flag)
    {
        parsedFlags.insert(name);
        return flags & ~flag;
    }
    return flags;
}
}

template<typename T>
std::set<QString> parseArchiveFlags(const T &flags_in)
{
    T flags = flags_in;

    std::set<QString> parsedFlags;

    flags = unrarhelpers_impl::parseArchiveFlag(flags, parsedFlags, ROADF_VOLUME, "ROADF_VOLUME");
    flags = unrarhelpers_impl::parseArchiveFlag(flags, parsedFlags, ROADF_COMMENT, "ROADF_COMMENT");
    flags = unrarhelpers_impl::parseArchiveFlag(flags, parsedFlags, ROADF_LOCK, "ROADF_LOCK");
    flags = unrarhelpers_impl::parseArchiveFlag(flags, parsedFlags, ROADF_SOLID, "ROADF_SOLID");
    flags = unrarhelpers_impl::parseArchiveFlag(flags, parsedFlags, ROADF_NEWNUMBERING, "ROADF_NEWNUMBERING");
    flags = unrarhelpers_impl::parseArchiveFlag(flags, parsedFlags, ROADF_SIGNED, "ROADF_SIGNED");
    flags = unrarhelpers_impl::parseArchiveFlag(flags, parsedFlags, ROADF_RECOVERY, "ROADF_RECOVERY");
    flags = unrarhelpers_impl::parseArchiveFlag(flags, parsedFlags, ROADF_ENCHEADERS, "ROADF_ENCHEADERS");
    flags = unrarhelpers_impl::parseArchiveFlag(flags, parsedFlags, ROADF_FIRSTVOLUME, "ROADF_FIRSTVOLUME");

    if (flags)
    {
        parsedFlags.insert(QString("Flag:0x") + QString::number(flags, 16));
    }

    return parsedFlags;
}
