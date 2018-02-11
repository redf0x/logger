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

namespace logger {

class logger_sink {
public:
        virtual void open (const std::string& name) = 0;
        virtual void close () = 0;
        virtual void write (const std::string& msg) = 0;

        virtual ~logger_sink() = 0;
};

inline logger_sink::~logger_sink() { }

enum class log_level {
        DEBUG,
        INFO,
        WARNING,
        CRITICAL,
        FATAL
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
        unsigned line;
        log_level severity;
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
        logger(const std::string& name, log_level min_severity);
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
        auto now = std::chrono::system_clock::now ();

        {
                using namespace date;
                stream << line++ << " " << now << " ";
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
                                             log_level min_severity) :
        line(0), severity(min_severity)
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

class file_logger_sink : public logger_sink {
        std::ofstream out;

public:
        file_logger_sink() { }
        ~file_logger_sink() { }

        void open (const std::string& name) override;
        void close () override;
        void write (const std::string& message) override;
};

}

#endif // LOGGER_H
