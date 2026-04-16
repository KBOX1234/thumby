#pragma once

#include <string>

class state_manager{
    private:

        std::string current_file_path;

    public:

        void set_file(const std::string& file);

        std::string get_file_path();
};

extern state_manager state_master;
