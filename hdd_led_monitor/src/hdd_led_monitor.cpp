#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <map>
#include <sstream>
#include <algorithm>
#include <cstdlib>

struct DiskStats {
    unsigned long long reads;
    unsigned long long writes;
};

// Reads stats from /proc/diskstats
std::map<std::string, DiskStats> get_disk_stats(const std::string& disk) {
    std::map<std::string, DiskStats> stats;
    std::ifstream file("/proc/diskstats");
    if (!file.is_open()) return stats;

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string major, minor, dev_name;
        unsigned long long r_ios, r_merges, r_sectors, r_ticks;
        unsigned long long w_ios, w_merges, w_sectors, w_ticks;

        if (ss >> major >> minor >> dev_name >> r_ios >> r_merges >> r_sectors >> r_ticks >> w_ios >> w_merges >> w_sectors >> w_ticks) {
            if (dev_name == disk) {
                stats[disk] = {r_ios, w_ios};
                break;
            }
        }
    }
    return stats;
}

// Converts a hex color string to the space-separated string "R G B"
std::string hex_to_rgb_string(std::string hex) {
    if (hex.find("0x") == 0 || hex.find("0X") == 0) {
        hex = hex.substr(2);
    } else if (hex.find('#') == 0) {
        hex = hex.substr(1);
    }

    if (hex.length() != 6) return "0 0 0";

    try {
        int r = std::stoi(hex.substr(0, 2), nullptr, 16);
        int g = std::stoi(hex.substr(2, 2), nullptr, 16);
        int b = std::stoi(hex.substr(4, 2), nullptr, 16);
        return std::to_string(r) + " " + std::to_string(g) + " " + std::to_string(b);
    } catch (...) {
        return "0 0 0";
    }
}

void write_led_string(const std::string& target_file_path, const std::string& rgb_string) {
    std::ofstream file(target_file_path);
    if (file.is_open()) {
        file << rgb_string;
    }
}

// Runs smartctl -H and evaluates the standard bitmask exit codes
bool is_drive_healthy(const std::string& disk) {
    std::string cmd = "/usr/sbin/smartctl -H /dev/" + disk + " >/dev/null 2>&1";
    int status = std::system(cmd.c_str());
    int exit_code = WEXITSTATUS(status);

    // Bit 3 (value 8) or Bit 4 (value 16) flags failure attributes or predictions
    if ((exit_code & 8) || (exit_code & 16)) {
        return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    if (argc != 8) {
        std::cerr << "Usage: " << argv 
                  << " <led_file_path> <interval_ms> <disk> <idle_hex> <act_hex> <fault_hex> <smart_check_interval_sec>\n";
        return 1;
    }

    std::string led_file_path = argv[1];
    int interval_ms = std::stoi(argv[2]);
    std::string disk = argv[3];
    
    std::string healthy_idle_rgb = hex_to_rgb_string(argv[4]);
    std::string act_rgb          = hex_to_rgb_string(argv[5]);
    std::string fault_idle_rgb   = hex_to_rgb_string(argv[6]);
    int smart_interval_sec = std::stoi(argv[7]);

    auto last_smart_check = std::chrono::steady_clock::now();
    bool drive_failed = !is_drive_healthy(disk);

    // Set dynamic target background color based on status
    std::string current_idle_rgb = drive_failed ? fault_idle_rgb : healthy_idle_rgb;

    // Optional: Clear triggers if the driver exposes one
    size_t last_slash = led_file_path.find_last_of('/');
    if (last_slash != std::string::npos) {
        std::string trigger_path = led_file_path.substr(0, last_slash) + "/trigger";
        std::ofstream trigger_file(trigger_path);
        if (trigger_file.is_open()) {
            trigger_file << "none";
        }
    }

    // Apply baseline idle state
    write_led_string(led_file_path, current_idle_rgb);
    auto last_stats = get_disk_stats(disk);

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(interval_ms));
        
        // 1. Periodically update SMART status to adjust the target background color
        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(now - last_smart_check).count() >= smart_interval_sec) {
            drive_failed = !is_drive_healthy(disk);
            last_smart_check = now;
            
            std::string new_idle = drive_failed ? fault_idle_rgb : healthy_idle_rgb;
            if (new_idle != current_idle_rgb) {
                current_idle_rgb = new_idle;
                write_led_string(led_file_path, current_idle_rgb);
            }
        }

        // 2. Normal Activity Blinking Cycle
        auto current_stats = get_disk_stats(disk);
        bool activity_detected = false;

        if (current_stats.find(disk) != current_stats.end() && last_stats.find(disk) != last_stats.end()) {
            if (current_stats[disk].reads != last_stats[disk].reads || 
                current_stats[disk].writes != last_stats[disk].writes) {
                activity_detected = true;
            }
        }

        if (activity_detected) {
            write_led_string(led_file_path, act_rgb);
            int flash_duration = std::min(50, interval_ms / 2);
            std::this_thread::sleep_for(std::chrono::milliseconds(flash_duration));
            // Always falls back to the dynamic background color (Healthy vs Fault)
            write_led_string(led_file_path, current_idle_rgb);
        }

        last_stats = current_stats;
    }

    return 0;
}
