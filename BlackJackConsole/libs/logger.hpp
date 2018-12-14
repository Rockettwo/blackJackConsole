#pragma once

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include "log.hpp"

extern logging::logger< logging::file_log_policy > log_inst;
extern logging::logger< logging::cmdline_log_policy > print_inst;

#define PRINT print_inst.print< logging::severity_type::print >
#define LOG1 log_inst.print< logging::severity_type::debug1 >
#define LOG2 log_inst.print< logging::severity_type::debug2 >
#define LOG3 log_inst.print< logging::severity_type::debug3 >
#define ERR log_inst.print< logging::severity_type::error >
#define WARN1 log_inst.print< logging::severity_type::warning1 >
#define WARN2 log_inst.print< logging::severity_type::warning2 >

#define SET_LOG_THREAD_NAME(name) log_inst.set_thread_name(name);

#endif
