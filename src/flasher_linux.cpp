#if defined(__linux__)


#include "flasher.hpp"
#include <vector>
#include <dirent.h>
#include <string>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

generic_flasher flash_master;

char read_device_flag(const std::string& device, const std::string& flag){
    std::string path = "/sys/block/" + device + flag;

    FILE *f = fopen(path.c_str(), "r");
    if (!f){
        std::cout << "Cannot open device: " << path << std::endl;
        return -1;
    }

    char value[8];
    if (!fgets(value, sizeof(value), f)) {
        fclose(f);
        return -1;
    }

    fclose(f);

    return value[0];

}


long get_file_size(int fd) {
    struct stat st;

    if (fstat(fd, &st) != 0) {
        return -1; // error
    }

    return st.st_size;
}

void generic_flasher::SetProgressCallback(ProgressCallback cb){
    progressCb = cb;
}


std::vector<std::string> generic_flasher::get_devices(){

    std::vector<std::string> rtv;

    DIR *d = opendir("/sys/block");
    struct dirent *dir;

    if (!d) return rtv;

    while ((dir = readdir(d)) != NULL) {
        if (dir->d_name[0] == '.') continue;

        char is_removable = read_device_flag(dir->d_name, "/removable");

        if(is_removable == '1') rtv.push_back(dir->d_name);

    }

    closedir(d);
    return rtv;
}

int generic_flasher::flash_device(const std::string& device, const std::string& file){
    std::cout << "Writing file: " << file << " to device: " << device << std::endl;
    int iso_fd = open(file.c_str(), O_RDONLY);
    if (iso_fd < 0) {
        std::cout << "Failed to open file!\n";
        progressCb(100);
        return FLASH_ERROR_ISO_NOT_FOUND;
    }
    int usb_fd = open(std::string("/dev/" + device).c_str(), O_WRONLY);
    if (usb_fd < 0) {
        close(iso_fd);
        std::cout << "Failed to find usb device!\n";
        std::cout << "USB device in question: " << "/dev/" + device << std::endl;
        printf("errno = %d (%s)\n", errno, strerror(errno));
        progressCb(100);
        return FLASH_ERROR_DEVICE_NOT_FOUND;
    }

    long total = get_file_size(iso_fd);
    long written = 0;
    char read_buffer[BUFFER_SIZE];

    while(1){
        ssize_t bytes_read = read(iso_fd, read_buffer, BUFFER_SIZE);
        if (bytes_read < 0) {
            std::cout << "Error while reading ISO file";
            progressCb(100);
            return FLASH_ERROR_ISO_NOT_READING;
        }
        if (bytes_read == 0) {
            break;
        }

        ssize_t bytes_written = write(usb_fd, read_buffer, bytes_read);
        if (bytes_written != bytes_read) {
            std::cout << "Error writing to USB device!\n";
            progressCb(100);
            return FLASH_ERROR_USB_NOT_WRITING;
        }
        written = written + bytes_written;
        int percent = (written * 100) / total;

        if (progressCb) {
            if(percent >= 100) percent = 99;
            progressCb(percent);
        }
    }

    fsync(usb_fd);

    close(iso_fd);
    close(usb_fd);

    progressCb(100);

    printf("ISO successfully written to device!\n");

    return FLASH_ERROR_NON;
}

#endif
