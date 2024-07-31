#pragma once
#include <fstream>
class OutputEngine {
public:
    OutputEngine (std::ofstream);

    virtual void write_class ();

private:
    std::ofstream out_file;
};
