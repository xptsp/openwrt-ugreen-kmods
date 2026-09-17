#include <iostream>
#include <fstream>
#include <sstream>  // Fixed: Added for std::stringstream
#include <string>
#include <unistd.h>
#include <cstdlib>

// Function to read the current disk stats for a specific drive
bool get_disk_stats(const std::string& drive, unsigned long& reads, unsigned long& writes) {
    std::ifstream file("/proc/diskstats");
    if (!file.is_open()) {
        std::cerr << "Error: Could not open /proc/diskstats" << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        // Look for the line containing our drive name (e.g., sda)
        if (line.find(" " + drive + " ") != std::string::npos) {
            std::string major, minor, name;
            unsigned long rio, rmerge, rsect, ruse, wio, wmerge, wsect, wuse;
            
            // Fixed: Removed the broken std::setdatastream line
            std::stringstream ss(line);
            ss >> major >> minor >> name >> rio >> rmerge >> rsect >> ruse >> wio;
            
            reads = rio;
            writes = wio;
            return true;
        }
    }
    return false;
}

// Function to write a value to the LED brightness file
void set_led(const std::string& led_path, const std::string& value) {
    std::ofstream led_file(led_path + "/brightness");
    if (led_file.is_open()) {
        led_file << value;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <drive_name> <led_sysfs_path>" << std::endl;
        std::cerr << "Example: " << argv[0] << " sda /sys/class/leds/green:disk" << std::endl;
        return 1;
    }

    std::string drive = argv[1];
    std::string led_path = argv[2];

    unsigned long last_reads = 0, last_writes = 0;
    unsigned long current_reads = 0, current_writes = 0;

    // Get initial baseline
    if (!get_disk_stats(drive, last_reads, last_writes)) {
        std::cerr << "Error: Drive " << drive << " not found in /proc/diskstats" << std::endl;
        return 1;
    }

    // Main monitoring loop
    while (true) {
        usleep(100000); // Check every 100ms

        if (get_disk_stats(drive, current_reads, current_writes)) {
            // Check if reads or writes increased
            if (current_reads > last_reads || current_writes > last_writes) {
                set_led(led_path, "255"); // Turn LED on
                usleep(50000);            // Keep it on for 50ms so humans can see it blink
                set_led(led_path, "0");   // Turn LED off
                
                last_reads = current_reads;
                last_writes = current_writes;
            }
        }
    }

    return 0;
}
