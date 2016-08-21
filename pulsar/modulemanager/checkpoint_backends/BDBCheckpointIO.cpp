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
using pulsar::util::to_byte_array;
using pulsar::util::from_byte_array;
    
#define SPECIAL_KEY "_%%__STORED_KEYS__%%_"


namespace pulsar {
namespace modulemanager {
        

BDBCheckpointIO::BDBCheckpointIO(const std::string & path, bool truncate)
    : path_(path), db_(nullptr)
{
    u_int32_t open_flags = DB_CREATE;
    int ret = 0;

    if(truncate)
        open_flags |= DB_TRUNCATE;

    ret = db_create(&db_, NULL, 0);

    if(ret != 0)
        throw GeneralException("Error while creating Berkeley DB",
                               "what", db_strerror(ret),
                               "path", path);


    // Open the database
    ret = db_->open(db_,
                    NULL,           // Transaction pointer
                    path_.c_str(),  // Database file name
                    NULL,           // Optional logical database name
                    DB_BTREE,       // Database access method
                    open_flags,     // Open flags
                    0);             // File create mode (using defaults)

    if(ret != 0)
        throw GeneralException("Error while opening Berkeley DB",
                               "what", db_strerror(ret),
                               "path", path);


    if(count(SPECIAL_KEY) > 0)
        read_keys_();
}

BDBCheckpointIO::~BDBCheckpointIO()
{
    //! \todo exceptions?
    db_->close(db_, 0);
}


void BDBCheckpointIO::read_keys_(void)
{
    ByteArray key_data = read(SPECIAL_KEY);
    stored_keys_ = from_byte_array<std::set<std::string>>(key_data);
}

size_t BDBCheckpointIO::count(const std::string & key) const
{
    // we need to const cast, since the C api for DBT 
    // requires non-const
    char * key_str = const_cast<char *>(key.c_str());

    DBT dbt_key;
    dbt_key.data = key_str;
    dbt_key.size = key.size()+1; // don't forget terminating \0
    dbt_key.flags = 0;

    if(db_->exists(db_, NULL, &dbt_key, 0) == 0)
        return 1;
    else
        return 0;
}

std::set<std::string> BDBCheckpointIO::all_keys(void) const
{
    return stored_keys_;
}

void BDBCheckpointIO::write(const std::string & key, const ByteArray & data)
{
    write_(key, data, true);
}

void BDBCheckpointIO::write_(const std::string & key, const ByteArray & data,
                             bool add_key)
{
    // we need to const cast, since the C api for DBT 
    // requires non-const
    char * key_ptr = const_cast<char *>(key.c_str());
    char * data_ptr = const_cast<char *>(data.data());

    DBT dbt_key;
    dbt_key.data = key_ptr;
    dbt_key.size = key.size()+1;
    dbt_key.flags = 0;

    DBT dbt_data;
    dbt_data.data = data_ptr;
    dbt_data.size = data.size();
    dbt_data.flags = 0;

    //! \todo errors & exceptions?
    int ret = db_->put(db_, NULL, &dbt_key, &dbt_data, 0);
    if(ret != 0)
        throw GeneralException("Error while writing to Berkeley DB",
                               "what", db_strerror(ret),
                               "path", path_,
                               "key", key);

    if(add_key)
    {
        // add to the stored keys
        stored_keys_.insert(key);
        ByteArray key_data = to_byte_array(stored_keys_);
        write_(SPECIAL_KEY, key_data, false);
    }
    
}

ByteArray BDBCheckpointIO::read(const std::string & key) const
{
    if(count(key) == 0)
        throw GeneralException("Cannot read data from checkpoint file - key doesn't exist",
                               "key", key);

    // we need to const cast, since the C api for DBT 
    // requires non-const
    char * key_ptr = const_cast<char *>(key.c_str());

    DBT dbt_key;
    dbt_key.data = key_ptr;
    dbt_key.size = key.size()+1;
    dbt_key.flags = 0;

    DBT dbt_data;


    // start with a 4k buffer. If more is needed, that is handled
    // in the loop below
    ByteArray data_bytes(4096);
    dbt_data.data = data_bytes.data();
    dbt_data.ulen = 4096;
    dbt_data.flags = DB_DBT_USERMEM;

    int ret;
    while( (ret = db_->get(db_, NULL, &dbt_key, &dbt_data, 0)) == DB_BUFFER_SMALL)
    {
        data_bytes.resize(dbt_data.size);        
        dbt_data.data = data_bytes.data();
        dbt_data.ulen = data_bytes.size();
    }

    if(ret != 0)
        throw GeneralException("Error while reading from Berkeley DB",
                               "what", db_strerror(ret),
                               "path", path_,
                               "key", key);

    // but now data_bytes might be too big.
    // Shrink it
    data_bytes.resize(dbt_data.size);

    return data_bytes;
}

void BDBCheckpointIO::erase(const std::string & key)
{
}

void BDBCheckpointIO::clear(void)
{
}


} // close namespace modulemanager
} // close namespace pulsar

