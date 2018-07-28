#pragma once

#ifdef OS_WINDOWS
#include <windows.h>
#endif
#include <unrar.h>

#include <QString>
#include <set>

template<typename T>
QString rarErrorToEnumString(const T &error)
{
    switch (error)
    {
        case ERAR_SUCCESS          : return "ERAR_SUCCESS";
        case ERAR_END_ARCHIVE      : return "ERAR_END_ARCHIVE";
        case ERAR_NO_MEMORY        : return "ERAR_NO_MEMORY";
        case ERAR_BAD_DATA         : return "ERAR_BAD_DATA";
        case ERAR_BAD_ARCHIVE      : return "ERAR_BAD_ARCHIVE";
        case ERAR_UNKNOWN_FORMAT   : return "ERAR_UNKNOWN_FORMAT";
        case ERAR_EOPEN            : return "ERAR_EOPEN";
        case ERAR_ECREATE          : return "ERAR_ECREATE";
        case ERAR_ECLOSE           : return "ERAR_ECLOSE";
        case ERAR_EREAD            : return "ERAR_EREAD";
        case ERAR_EWRITE           : return "ERAR_EWRITE";
        case ERAR_SMALL_BUF        : return "ERAR_SMALL_BUF";
        case ERAR_UNKNOWN          : return "ERAR_UNKNOWN";
        case ERAR_MISSING_PASSWORD : return "ERAR_MISSING_PASSWORD";
        case ERAR_EREFERENCE       : return "ERAR_EREFERENCE";
        case ERAR_BAD_PASSWORD     : return "ERAR_BAD_PASSWORD";
        default                    : return "ERAR_UNKNOWN_ERROR_" + QString::number(error);
    }
}

template<typename T>
QString rarOpenErrorToString(const T &error)
{
    switch (error)
    {
        case ERAR_SUCCESS         : return "Success.";
        case ERAR_NO_MEMORY       : return "Not enough memory to initialize data structures.";
        case ERAR_BAD_DATA        : return "Archive header broken!";
        case ERAR_BAD_ARCHIVE     : return "Unknown archive format!";
        case ERAR_UNKNOWN_FORMAT  : return "Unknown encryption used for archive headers!";
        case ERAR_EOPEN           : return "Cannot open archive!";
        case ERAR_BAD_PASSWORD    : return "Entered password is invalid!";
        default                   : return rarErrorToEnumString(error);
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
        case ERAR_BAD_PASSWORD       : return "Entered password is invalid";
        default                      : return rarErrorToEnumString(error);
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
        default                      : return rarErrorToEnumString(error);
    }
}

namespace unrarhelpers_impl
{
template<typename T, typename U>
T parseArchiveFlag(const T &flags, QString &result, U flag, QString name)
{
    if ((flags & flag) == flag)
    {
        if (!result.isEmpty())
            result += " | ";
        result += name;
        return flags & ~flag;
    }
    return flags;
}
}

template<typename T>
QString parseArchiveFlags(const T &flags_in)
{
    T flags = flags_in;

    QString result;

    flags = unrarhelpers_impl::parseArchiveFlag(flags, result, ROADF_VOLUME, "ROADF_VOLUME");
    flags = unrarhelpers_impl::parseArchiveFlag(flags, result, ROADF_COMMENT, "ROADF_COMMENT");
    flags = unrarhelpers_impl::parseArchiveFlag(flags, result, ROADF_LOCK, "ROADF_LOCK");
    flags = unrarhelpers_impl::parseArchiveFlag(flags, result, ROADF_SOLID, "ROADF_SOLID");
    flags = unrarhelpers_impl::parseArchiveFlag(flags, result, ROADF_NEWNUMBERING, "ROADF_NEWNUMBERING");
    flags = unrarhelpers_impl::parseArchiveFlag(flags, result, ROADF_SIGNED, "ROADF_SIGNED");
    flags = unrarhelpers_impl::parseArchiveFlag(flags, result, ROADF_RECOVERY, "ROADF_RECOVERY");
    flags = unrarhelpers_impl::parseArchiveFlag(flags, result, ROADF_ENCHEADERS, "ROADF_ENCHEADERS");
    flags = unrarhelpers_impl::parseArchiveFlag(flags, result, ROADF_FIRSTVOLUME, "ROADF_FIRSTVOLUME");

    if (flags)
    {
        if (!result.isEmpty())
            result += " | ";
        result += QString("Flag:0x") + QString::number(flags, 16);
    }

    return result;
}
