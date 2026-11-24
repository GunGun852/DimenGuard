#pragma once

#include <ostream>

namespace dimenguard::cli {

// Plain ASCII works in ordinary terminals without colour or Unicode support.
// A raw string keeps the backslashes in the artwork readable.
inline void print_startup_logo(std::ostream& out) {
    out << R"DG_LOGO(
  ____  ___ __  __ _____ _   _  ____ _   _   _    ____  ____
 |  _ \|_ _|  \/  | ____| \ | |/ ___| | | | / \  |  _ \|  _ \
 | | | || || |\/| |  _| |  \| | |  _| | | |/ _ \ | |_) | | | |
 | |_| || || |  | | |___| |\  | |_| | |_| / ___ \|  _ <| |_| |
 |____/|___|_|  |_|_____|_| \_|\____|\___/_/   \_\_| \_\____/

       C++20 | Compile-time units & dimensional analysis
 .-----------------------------------------------------------.
)DG_LOGO";
}

} // namespace dimenguard::cli
