#ifndef TIME_HH
#define TIME_HH

#include <ctime>
#include <string>

namespace Core
{

//return the number of microseconds ( warning : precision loss ) the  program has been running.
// or the number of clock cycles if usec == false
// 0  as a return code means we cannot access this information
unsigned long clock();
unsigned long clocksec();
unsigned long clockusec();

//returns the process clock precision by comparing with wallclock
double clockprec();

//pause the execution of the current thread
void sleep(unsigned int sec);
void usleep (unsigned int usec);


class Time;

class Date
{
    friend class Time;

    struct tm d;

    ///explicit constructor to allow Time to produce Dates
    explicit Date( struct tm * cdate ) : d(*cdate) {};

public:
    Date(int year =0, int month =0, int mday =0, int hour =0, int min =0, int sec =0 );
    ~Date();

    //TODO : improve that

    ///Seconds.	[0-60] (1 leap second)
    int & sec()
    {
        return d.tm_sec;
    }
    ///Minutes.	[0-59]
    int & min()
    {
        return d.tm_min;
    }
    ///Hours.	[0-23]
    int & hour()
    {
        return d.tm_hour;
    }
    ///Day.		[1-31]
    int & mday()
    {
        return d.tm_mday;
    }
    ///Month.	[0-11] ///BEWARE
    int & mon()
    {
        return d.tm_mon;
    }
    ///Year	- 1900. ///BEWARE
    int & year()
    {
        return d.tm_year;
    }
    ///Day of week.	[0-6]
    int & wday()
    {
        return d.tm_wday;
    }
    /// Days in year.[0-365]
    int & yday()
    {
        return d.tm_yday;
    }
    ///DST.		[-1/0/1]
    int & isdst()
    {
        return d.tm_isdst;
    }
    /* Not available on win32
    ///Timezone character code
    std::string zone()
    {
        return std::string(d.tm_zone);
    }*/

    ///returns a textual version of the time
    const std::string ascii();

    Time mkTime();

    //TODO : size_t strftime( char *str, size_t maxsize, const char *fmt, struct tm *time );
};



/** TODO : Class to handle portable calls to standard time functions
 * NB : Time precision is the second...
**/

class Time
{
    friend class Date ;
    explicit Time (time_t tmt) : tt(tmt) {};


public :

    static Time now();

private:

    time_t tt;

public :

    Time();
    Time( Date & d );
    ~Time();

    std::string localascii();

    //return current localtime
    Date local();

    //return current GMT time
    Date GMT();

//TODO setlocale...

    //returns the time difference in seconds
    double operator-(const Time & t ) const;


};

//TODO : check that to handle custom dates and conversion : http://c.developpez.com/sources/?page=III#DATE_mktime
/** TODO : Class to handle Simulated time and relation with runtime...
 * Pattern :
**/

class SimTime
{
    unsigned int rtfactor;

public :

};

} //Core
#endif
