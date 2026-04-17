#pragma once

#include <string>
#include <vector>
#include <functional>

using ProgressCallback = std::function<void(int)>;

#define BUFFER_SIZE 64 * 1024

typedef enum{
    FLASH_ERROR_NON,
    FLASH_ERROR_ISO_NOT_FOUND,
    FLASH_ERROR_DEVICE_NOT_FOUND,
    FLASH_ERROR_ISO_NOT_READING,
    FLASH_ERROR_USB_NOT_WRITING,
    FLASH_ERROR_GENERIC
}FLASH_ERRORS ;

class generic_flasher{
    private:
        std::string device_name;

        ProgressCallback progressCb;

    public:

        std::vector<std::string> get_devices();

        int flash_device(const std::string& device, const std::string& file);

        void SetProgressCallback(ProgressCallback cb);
};

extern generic_flasher flash_master;


