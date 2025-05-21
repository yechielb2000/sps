#include <iostream>
#include "OptionsParser.hpp"




int main(int argc, char *argv[]) {
    ScanConfig opts = OptionsParser::parse(argc, argv);

    return 0;
}
