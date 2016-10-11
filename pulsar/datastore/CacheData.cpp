#include "pulsar/datastore/CacheData.hpp"

namespace pulsar{
namespace datastore{

    const unsigned int CacheData::NoCheckpoint     = 1;
   const unsigned int CacheData::CheckpointLocal  = 2;
    const unsigned int CacheData::CheckpointGlobal = 4;
}}