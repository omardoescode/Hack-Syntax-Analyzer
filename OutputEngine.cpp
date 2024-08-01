#include "OutputEngine.h"
#include <fstream>
#include <memory>
#include <string>

OutputEngine::OutputEngine (const std::string& out_file_name, std::shared_ptr<HackMap> hack_map)
: out_file (out_file_name), hack_map (hack_map) {
}
