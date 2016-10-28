/*! \file
 *
 * \brief Checkpointing of a calculation (header)
 */
#pragma once

#include <memory>
#include <vector>

namespace pulsar {
class CacheData;
class ModuleManager;
class CheckpointIO;


/*! \brief Some common functionality for checkpointing
 */
class Checkpoint
{
    public:
        Checkpoint(const std::shared_ptr<CheckpointIO> & backend_local,
                   const std::shared_ptr<CheckpointIO> & backend_global);

        ~Checkpoint() = default;


        // no copy construction or assignment
        Checkpoint(const Checkpoint & rhs)             = delete;
        Checkpoint(Checkpoint && rhs)                  = default;
        Checkpoint & operator=(const Checkpoint & rhs) = delete;
        Checkpoint & operator=(Checkpoint && rhs)      = default;



        void save_local_cache(const ModuleManager & mm);

        void load_local_cache(ModuleManager & mm);

        void save_global_cache(const ModuleManager & mm);

        void load_global_cache(ModuleManager & mm);

    private:
        std::shared_ptr<CheckpointIO> backend_local_;
        std::shared_ptr<CheckpointIO> backend_global_;

        static std::vector<std::string>
        form_cache_save_list_(const ModuleManager & mm,
                              CheckpointIO & backend,
                              std::function<bool(unsigned int)> policy_check);

        void perform_on_all_ranks_(const std::string & description, std::function<void(void)> func);

        void load_cache_(ModuleManager & mm, CheckpointIO & backend);

        void save_cache_(const ModuleManager & mm, CheckpointIO & backend,
                         std::function<bool(unsigned int)> policy_check);
};

} // close namespace pulsar

