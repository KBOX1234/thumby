#include "../include/state_manager.hpp"
#include <string>

state_manager state_master;

void state_manager::set_file(const std::string& fpath){
    current_file_path = fpath;
}

std::string state_manager::get_file_path(){
    return current_file_path;
}
