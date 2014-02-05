#include "Time.hh"

#include <cstdio> //to access stderr and stdio for now
//TODO : the Logger should handle this

#include <stdexcept>

namespace Core
{

/********** Date ***********/
Date::Date(int year , int month , int mday , int hour , int min , int sec  )
{
    //recomendd for safety
    struct tm zerod = {0} ;

    zerod.tm_sec = sec;
    zerod.tm_min = min;
    zerod.tm_hour = hour;
    zerod.tm_mday = mday;
    zerod.tm_mon = month -1; //month starts at 0;
    zerod.tm_year = year -1900; //year starts at 1900 ( EPOCH )

    d = zerod;
}

Date::~Date()
{}


const std::string Date::ascii()
{
#ifdef _WIN32
    char *res = NULL;
    if (asctime_s(res, 32, &d))
        return "";
    else
        return std::string(res);
#else
    return std::string(asctime(&d));
#endif

}

Time Date::mkTime()
{
    return Time(mktime( &d));
}

/************ Time *************/

Time::Time()
{
    time(&tt);
}

Time::Time( Date & d )
{

    tt = mktime (& d.d);

}

Time::~Time()
{
}

void sleep (unsigned int sec)
{

    /*#ifdef _WIN32
        //Sleep (sec * 1000);
    #else*/
#  if _POSIX_VERSION > 198808L
    ::sleep (sec);
#  else
    clock_t start, end;

    end = start = ::clock ();
    if (start != -1)
    {
        while (static_cast<unsigned int>( (end - start) / CLOCKS_PER_SEC) < sec)
        {
            //That uses system resources, better solution should be possible
            end = ::clock ();
        }
    }
#  endif /* _POSIX_VERSION */
    /*#endif */ /* _WIN32 */
}


void usleep (unsigned int usec)
{

    /*#ifdef _WIN32
        //Sleep (usec / 1000);
    #else*/
#  if _POSIX_VERSION > 198808L
    ::usleep (usec);
#  else
    clock_t start, end;

    end = start = ::clock ();
    if (start != -1)
    {
        while ( static_cast<unsigned int>( (end - start) / CLOCKS_PER_SEC * 1000000l ) < usec)
        {
            //That uses system resources, better solution should be possible
            end = ::clock ();
        }
    }
#  endif /* _POSIX_VERSION */
    /*#endif *//* _WIN32 */
}

unsigned long clock ()
{

    clock_t clk = ::clock();

    //0 is an error code here : information not availalbe
    unsigned long res = 0;

    if ( clk != -1 )
    {
        res = (unsigned long)clk;
    }
    else
    {
        fprintf(stderr,"Error : ::clock() returned -1");
    }
    //clk == -1 -> information not available we return 0

    return res;
}

unsigned long clocksec ()
{

    clock_t clk = ::clock();

    //0 is an error code here : information not availalbe
    unsigned long res = 0;

    if ( clk != -1 )
    {
        res = (unsigned long)clk / ( CLOCKS_PER_SEC) ;

    }
    else
    {
        fprintf(stderr,"Error : ::clock() returned -1");
    }
    //clk == -1 -> information not available we return 0

    return res;
}


unsigned long clockusec ()
{

    clock_t clk = ::clock();

    //0 is an error code here : information not availalbe
    unsigned long res = 0;

    if ( clk != -1 )
    {
        //TODO : test long limits
        res = (unsigned long)clk / ( CLOCKS_PER_SEC ) * 1000000l ;
    }
    else
    {
        fprintf(stderr,"Error : ::clock() returned -1");
    }
    //clk == -1 -> information not available we return 0

    return res;
}



double clockprec ()
{
    return 1/(double)CLOCKS_PER_SEC;
}

Time Time::now()
{
    return Time(time(NULL));

}

std::string Time::localascii()
{
#ifdef _WIN32
    char *res = NULL;
    if (ctime_s(res, 32, &tt))
        return "";
    else
        return std::string(res);
#else
    return std::string(ctime( & tt ) );
#endif
}

Date Time::GMT()
{
#ifdef _WIN32
    struct tm d;
    if (!gmtime_s(&d, &tt))
    {
        Date gmt(&d);
        return gmt;
    }
    else
    {
        throw std::logic_error("Invalid Argument to gmtime_s");
    }
#else
    Date gmt(gmtime( & tt ));
    return gmt;
#endif
}


Date Time::local()
{
#ifdef _WIN32
    struct tm d;
    if (!localtime_s(&d, &tt))
    {
        Date loc(&d);
        return loc;
    }
    else
    {
        throw std::logic_error("Invalid Argument to localtime_s");
    }
#else
    Date loc(localtime( & tt ));
    return loc;
#endif
}

double Time::operator-(const Time & t ) const
{
    return difftime(tt,t.tt);
}



}//Core
