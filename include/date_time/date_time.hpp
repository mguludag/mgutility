#ifndef DATE_TIME_HPP
#define DATE_TIME_HPP

#include <chrono>
#include <ctime>
#include <string>

#ifdef WIN32
#define localtime_r(_Time, _Tm) localtime_s(_Tm, _Time)
#define gmtime_r(_Time, _Tm) gmtime_s(_Tm, _Time)
#endif


namespace mgutility {

inline const std::string current_date_time_locale()
{
    using namespace std::chrono;
    char buf[80];
    struct tm tstruct;
    const system_clock::time_point t = system_clock::now();
    const time_t now = system_clock::to_time_t(t);
    localtime_r(&now, &tstruct);

    const duration<double> tse = t.time_since_epoch();
    const seconds::rep ms = duration_cast<milliseconds>(tse).count() % 1000;

    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    const auto pos = strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
#ifdef WIN32
    sprintf(buf + pos, (ms > 99 ? ".%lld" : ms > 9 ? ".0%lld" : ".00%lld"), ms);
#else
    sprintf(buf + pos, (ms > 99 ? ".%ld" : ms > 9 ? ".0%ld" : ".00%ld"), ms);
#endif

    return buf;
}

inline const std::string current_date_time_utc()
{
    using namespace std::chrono;
    char buf[80];
    struct tm tstruct;
    const system_clock::time_point t = system_clock::now();
    const time_t now = system_clock::to_time_t(t);
    gmtime_r(&now, &tstruct);

    const duration<double> tse = t.time_since_epoch();
    const seconds::rep ms = duration_cast<milliseconds>(tse).count() % 1000;

    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    const auto pos = strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
#ifdef WIN32
    sprintf(buf + pos, (ms > 99 ? ".%lld" : ms > 9 ? ".0%lld" : ".00%lld"), ms);
#else
    sprintf(buf + pos, (ms > 99 ? ".%ld" : ms > 9 ? ".0%ld" : ".00%ld"), ms);
#endif

    return buf;
}

inline const uint64_t current_epoch_ms()
{
    using namespace std::chrono;
    const auto now = system_clock::now();
    const auto duration = now.time_since_epoch();
    const auto millis = duration_cast<milliseconds>(duration).count();

    return static_cast<uint64_t>(millis);
}



} // namespace mgutility

#endif // DATE_TIME_HPP
