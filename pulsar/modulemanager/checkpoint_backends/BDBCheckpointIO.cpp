/*! \file
 *
 * \brief File checkpointing backend
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include "pulsar/modulemanager/checkpoint_backends/BDBCheckpointIO.hpp"
#include "pulsar/util/Serialization.hpp"
#include "pulsar/exception/Exceptions.hpp"
#include "pulsar/output/GlobalOutput.hpp"

using namespace pulsar::exception;



namespace pulsar {
namespace modulemanager {
        

BDBCheckpointIO::BDBCheckpointIO(const std::string & path, bool truncate)
    : path_(path), db_(NULL, 0)
{

    u_int32_t open_flags = DB_CREATE;
    if(truncate)
        open_flags |= DB_TRUNCATE;

    try {
        // Open the database
        db_.open(NULL,           // Transaction pointer
                 path_.c_str(),  // Database file name
                 NULL,           // Optional logical database name
                 DB_BTREE,       // Database access method
                 open_flags,     // Open flags
                 0);             // File mode (using defaults)

    // DbException is not subclassed from std::exception, so
    // need to catch both of these.
    }
    catch(DbException &e) {
        throw GeneralException("DB error when opening checkpoint file",
                               "what", e.what(),
                               "path", path);
    }
    catch(std::exception &e) {
        throw GeneralException("STD error when opening checkpoint file",
                               "what", e.what(),
                               "path", path);
    }
}

BDBCheckpointIO::~BDBCheckpointIO()
{
    //! \todo exceptions?
    db_.close(0);
}


size_t BDBCheckpointIO::size(void) const
{
    //! \todo I don't understand Db.stat()
    return all_keys().size();
}


size_t BDBCheckpointIO::count(const std::string & key) const
{
    char key_str[key.size()+1];
    strncpy(key_str, key.c_str(), key.size()+1);
    Dbt dbt_key(key_str, key.size()+1);
    if(db_.exists(NULL, &dbt_key, 0) != DB_NOTFOUND)
        return 1;
    return 0;
}

std::set<std::string> BDBCheckpointIO::all_keys(void) const
{
    std::set<std::string> keys;

    Dbc *cursorp;

    db_.cursor(NULL, &cursorp, 0);

    Dbt key, data;
    int ret;

    while ((ret = cursorp->get(&key, &data, DB_NEXT)) == 0)
    {
        char * key_data = reinterpret_cast<char *>(key.get_data());
        std::string key_str(key_data);
        keys.emplace(std::move(key_str));
    }

    if (cursorp != NULL)
        cursorp->close();

    return keys;
}

void BDBCheckpointIO::write(const std::string & key, const ByteArray & data)
{
    using pulsar::util::to_byte_array;

    char key_str[key.size()+1];
    strncpy(key_str, key.c_str(), key.size()+1);
    Dbt dbt_key(key_str, key.size()+1);

    ByteArray data_tmp = data;
    Dbt dbt_data(data_tmp.data(), data.size());    

    //! \todo errors & exceptions?
    db_.put(NULL, &dbt_key, &dbt_data, 0);
}

ByteArray BDBCheckpointIO::read(const std::string & key) const
{
    using pulsar::util::to_byte_array;

    if(count(key) == 0)
        throw GeneralException("Cannot read data from checkpoint file - key doesn't exist",
                               "key", key);

    char key_str[key.size()+1];

    strncpy(key_str, key.c_str(), key.size()+1);


    Dbt dbt_key(key_str, key.size()+1);
    Dbt dbt_data;

    //! \todo errors & exceptions?

    // We need to get the size of the data
    dbt_data.set_data(NULL);
    dbt_data.set_ulen(0);
    dbt_data.set_flags(DB_DBT_USERMEM);

    try {
        db_.get(NULL, &dbt_key, &dbt_data, 0);
    }
    catch(...)
    { } // swallow the exception

    // now we have the size. allocate the byte array
    size_t datasize = dbt_data.get_size();
    ByteArray ret(datasize);
    dbt_data.set_ulen(datasize);
    dbt_data.set_data(ret.data());

    // actually retrieve the data
    db_.get(NULL, &dbt_key, &dbt_data, 0);

    return ret;
}

void BDBCheckpointIO::erase(const std::string & key)
{
}

void BDBCheckpointIO::clear(void)
{
}


} // close namespace modulemanager
} // close namespace pulsar

