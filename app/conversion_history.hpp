#pragma once

#include <cstddef>
#include <deque>
#include <ostream>
#include <string>
#include <utility>

namespace dimenguard::cli {

// Own the displayed results so later input cannot change earlier records.
// Create one history per interactive session; nothing is written to disk.
class conversion_history {
public:
    static constexpr std::size_t capacity = 10;

    // Call only after conversion and validation have succeeded.
    void record(std::string result) {
        records_.push_back(std::move(result));
        if (records_.size() > capacity) records_.pop_front();
    }

    void clear() { records_.clear(); }

    void print(std::ostream& out) const {
        out << "\nRecent successful conversions (" << records_.size()
            << '/' << capacity << ", oldest first):\n";
        if (records_.empty()) {
            out << "No successful conversions in this session.\n";
            return;
        }
        std::size_t number = 1;
        for (const auto& result : records_) {
            out << number++ << ". " << result << '\n';
        }
    }

private:
    std::deque<std::string> records_;
};

} // namespace dimenguard::cli
