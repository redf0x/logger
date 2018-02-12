#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <chrono>
#include <mutex>
#include <vector>
#include <thread>
#include <map>
#include <fstream>

#include "date.h"
#include "logger_sink.h"

namespace logger {

enum class log_level {
        DEBUG,
        INFO,
        WARNING,
        CRITICAL,
        FATAL
};

enum class logger_flags {
        flag_none = 0,
        flag_time_stamp = 1
};

template<typename sink> class logger;

template<typename sink> void logd (logger<sink>* logger)
{
        std::unique_lock<std::timed_mutex> wr_lck{logger->wr_mtx, std::defer_lock};

        do {
                std::this_thread::sleep_for (std::chrono::milliseconds{10});
                if (logger->log_buffer.size ()) {
                        wr_lck.lock ();

                        for (auto& item : logger->log_buffer)
                                logger->destination.write (item);

                        logger->log_buffer.clear ();
                        wr_lck.unlock ();
                }
        } while (logger->running.test_and_set () || logger->log_buffer.size ());
        logger->destination.write ("logd shutting down");
}

template<typename sink> class logger {
        std::chrono::high_resolution_clock::time_point reference_epoch;
        unsigned seq_no;
        log_level severity;
        logger_flags flags;
        sink destination;
        std::timed_mutex wr_mtx;
        std::vector<std::string> log_buffer;
        std::thread daemon;
        std::atomic_flag running = ATOMIC_FLAG_INIT;
        std::map<std::thread::id, std::string> thread_name;

        void dump (std::stringstream&& stream);
        template<typename Head, typename... Tail> void dump (
                        std::stringstream&&, Head&& h,
                        Tail&&... t);

public:
        logger(const std::string& name, log_level min_severity,
               logger_flags flags = logger_flags::flag_time_stamp);
        ~logger();

        void set_logging_level (log_level new_level);
        template<log_level severity, typename...Args> void log (Args&&...args);
        void set_thread_name (const std::string& name);
        void shutdown ();

        template<typename destination> friend void logd (logger<destination>* logger);
};

template<typename sink> void logger<sink>::shutdown ()
{
        running.clear ();
        daemon.join ();
}

template<typename sink>
        void logger<sink>::set_thread_name (const std::string& name)
{
        thread_name [std::this_thread::get_id ()] = name;
}

template<typename sink>
        template<log_level severity, typename... Args>
        void logger<sink>::log (Args&&... args)
{
        if (severity < this->severity)
                return;

        std::stringstream stream;

        stream << seq_no++ << " ";

        if (flags == logger_flags::flag_time_stamp) {
            auto now = std::chrono::system_clock::now ();
            using namespace date;
            stream << now << " ";
        }

        stream << thread_name [std::this_thread::get_id ()] <<" ";

        switch (severity) {
                case log_level::DEBUG:
                        stream << "[DEBUG]: ";
                        break;

                case log_level::INFO:
                        stream << "[INFO]: ";
                        break;

                case log_level::WARNING:
                        stream << "[WARNING]: ";
                        break;

                case log_level::CRITICAL:
                        stream << "[CRITICAL]: ";
                        break;

                case log_level::FATAL:
                        stream << "[FATAL]: ";
                        break;
        };

        dump (std::forward<std::stringstream>(stream),
                   std::move (args)...);
}

template<typename sink> void logger<sink>::dump (std::stringstream&& stream)
{
        std::lock_guard<std::timed_mutex> lck(wr_mtx);
        log_buffer.push_back (stream.str ());
}

template<typename sink>
        template<typename Head, typename... Tail>
        void logger<sink>::dump (std::stringstream&& stream,
                                 Head&& h, Tail&&... t)
{
        stream << h;
        dump (std::forward<std::stringstream>(stream),
                   std::move (t)...);
}

template<typename sink> logger<sink>::logger(const std::string& name,
                                             log_level min_severity,
                                             logger_flags flags) :
        seq_no(0), severity(min_severity), flags(flags)
{
        destination.open (name);
        reference_epoch = std::chrono::high_resolution_clock::now ();
        running.test_and_set ();
        daemon = std::move (std::thread(logd<sink> , this));
}

template<typename sink> void logger<sink>::set_logging_level (log_level new_level)
{
        if (new_level > log_level::FATAL)
                new_level = log_level::FATAL;

        severity = new_level;
}

template<typename sink> logger<sink>::~logger()
{
        shutdown ();
        destination.write ("logger terminated");
        destination.close ();
}

}

#endif // LOGGER_H
