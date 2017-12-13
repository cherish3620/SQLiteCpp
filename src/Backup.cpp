/**
 * @file    Backup.cpp
 * @ingroup SQLiteCpp
 * @brief   Backup is used to backup a database file in a safe and online way.
 *
 * Copyright (c) 2015 Shibao HONG (shibaohong@outlook.com)
 * Copyright (c) 2015-2016 Sebastien Rombauts (sebastien.rombauts@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */


 /* definition to expand macro then apply to pragma message */
#define VALUE_TO_STRING(x) #x
#define VALUE(x) VALUE_TO_STRING(x)
#define VAR_NAME_VALUE(var) #var "="  VALUE(var)

#if defined(_MSC_VER) 
#pragma message(VAR_NAME_VALUE(__cplusplus))
#pragma message(VAR_NAME_VALUE(_MSC_VER))
#elif defined(__clang__)
#pragma message(VAR_NAME_VALUE(__clang__))
#pragma message(VAR_NAME_VALUE(__APPLE__))
#pragma message(VAR_NAME_VALUE(__GXX_EXPERIMENTAL_CXX0X__))
#pragma message(VAR_NAME_VALUE(__has_feature(cxx_noexcept)))
#pragma message(VAR_NAME_VALUE(__has_feature(cxx_nullptr)))
#else
#pragma message "__cplusplus " __cplusplus
#pragma message "__GNUC__ " __GNUC__
#pragma message "__APPLE__ " __APPLE__
#pragma message "__GXX_EXPERIMENTAL_CXX0X__ " __GXX_EXPERIMENTAL_CXX0X__
#pragma message "__has_feature(cxx_noexcept) " __has_feature(cxx_noexcept)
#pragma message "__has_feature(cxx_nullptr) " __has_feature(cxx_nullptr)
#endif


#include <SQLiteCpp/Backup.h>

#include <SQLiteCpp/Exception.h>

#include <sqlite3.h>

namespace SQLite
{

// Initialize resource for SQLite database backup
Backup::Backup(Database&    aDestDatabase,
               const char*  apDestDatabaseName,
               Database&    aSrcDatabase,
               const char*  apSrcDatabaseName) :
    mpSQLiteBackup(NULL)
{
    mpSQLiteBackup = sqlite3_backup_init(aDestDatabase.getHandle(),
                                         apDestDatabaseName,
                                         aSrcDatabase.getHandle(),
                                         apSrcDatabaseName);
    if (NULL == mpSQLiteBackup)
    {
        // If an error occurs, the error code and message are attached to the destination database connection.
        throw SQLite::Exception(aDestDatabase.getHandle());
    }
}

// Initialize resource for SQLite database backup
Backup::Backup(Database&            aDestDatabase,
               const std::string&   aDestDatabaseName,
               Database&            aSrcDatabase,
               const std::string&   aSrcDatabaseName) :
    mpSQLiteBackup(NULL)
{
    mpSQLiteBackup = sqlite3_backup_init(aDestDatabase.getHandle(),
                                         aDestDatabaseName.c_str(),
                                         aSrcDatabase.getHandle(),
                                         aSrcDatabaseName.c_str());
    if (NULL == mpSQLiteBackup)
    {
        // If an error occurs, the error code and message are attached to the destination database connection.
        throw SQLite::Exception(aDestDatabase.getHandle());
    }
}

// Initialize resource for SQLite database backup
Backup::Backup(Database &aDestDatabase, Database &aSrcDatabase) :
    mpSQLiteBackup(NULL)
{
    mpSQLiteBackup = sqlite3_backup_init(aDestDatabase.getHandle(),
                                         "main",
                                         aSrcDatabase.getHandle(),
                                         "main");
    if (NULL == mpSQLiteBackup)
    {
        // If an error occurs, the error code and message are attached to the destination database connection.
        throw SQLite::Exception(aDestDatabase.getHandle());
    }
}

// Release resource for SQLite database backup
Backup::~Backup()
{
    if (NULL != mpSQLiteBackup)
    {
        sqlite3_backup_finish(mpSQLiteBackup);
    }
}

// Execute backup step with a given number of source pages to be copied
int Backup::executeStep(const int aNumPage /* = -1 */)
{
    const int res = sqlite3_backup_step(mpSQLiteBackup, aNumPage);
    if (SQLITE_OK != res && SQLITE_DONE != res && SQLITE_BUSY != res && SQLITE_LOCKED != res)
    {
        throw SQLite::Exception(sqlite3_errstr(res), res);
    }
    return res;
}

// Get the number of remaining source pages to be copied in this backup process
int Backup::getRemainingPageCount()
{
    return sqlite3_backup_remaining(mpSQLiteBackup);
}

// Get the number of total source pages to be copied in this backup process
int Backup::getTotalPageCount()
{
    return sqlite3_backup_pagecount(mpSQLiteBackup);
}

}  // namespace SQLite
