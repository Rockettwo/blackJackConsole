#include "logger.hpp"

logging::logger< logging::file_log_policy > log_inst("execution.log", logging::severity_type::debug1);
logging::logger< logging::cmdline_log_policy > print_inst("cout", logging::severity_type::print);