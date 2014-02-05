#ifndef LOGSTREAMBUF_HH
#define LOGSTREAMBUF_HH

#include <cstdio> // for EOF
#include <iostream>
#include <fstream>
#include <sstream>
#include <locale>


namespace Core
{

//we define here logstream and logstreambuf which should integrates well with iostream from STL

/**
 * \class logstreambuf
 *
 * \brief This class defines an overloaded streambuf, to be used with logstream
 * It manages filtering in and out incoming messages from the stream.
 * It provides helpful function for derivated classes to implement prefix adding on output to sinks, on sync() calling...
 *
 * Date&Time (internally added) [TODO| ThreadID (internally added)] |Custom Prefix (set by logstream) | Message ( sent by logstream )
 */

class logstreambuf: public std::stringbuf
{

protected :
    std::string ptm_prefix;

    bool ptm_logtime;

    //IDEA : logstreambuf is a string buffer. Therefore it can be accessed and manipulated by a stringstream if needed.
    //On flush, the content of the buffer is transferred to a sink ( derivated classes )

    //filter boolean
    bool ptm_filterin;

public:
    logstreambuf();
    ~logstreambuf();

//to manage prefix
    void resetprefix(const std::string& newprefix = 0);
    const std::string& getprefix() const;

    void resetlogtime(bool v=true)
    {
        ptm_logtime = v;
    }

//to start and stop getting messages
    void filterout()
    {
        ptm_filterin = false ;
    }
    void filterin()
    {
        ptm_filterin = true;
    }


protected:


    ///Synchronize stream buffer
    ///must be overloaded by implementations of this abstract class (depending on log output)
    virtual int sync ( ) = 0;

    std::string getlocaltime();

    ///Output functions (put)
    ///Write sequence of characters
    virtual std::streamsize xsputn ( const char * s, std::streamsize n );
    ///Write character in the case of overflow ( endl for exemple )
    virtual int overflow ( int c = EOF );


};

/* TODO
///output to syslog
class syslogstreambuf: public logstreambuf
{
    virtual int sync ( );
};

///output to win32dbg
class win32logstreambuf: public logstreambuf
{
    virtual int sync ( );
};
*/

///Output to clog
class clogstreambuf: public logstreambuf
{
    //clog as sink on sync()
    std::streambuf* ptm_clogbuf;
public:
    clogstreambuf();
    ~clogstreambuf();

protected :
    virtual int sync ( );
};


///Output to clog
class filelogstreambuf: public logstreambuf
{
    //clog as sink on sync()
    std::filebuf ptm_filelogbuf;
public:
    filelogstreambuf(const std::string & filename);
    ~filelogstreambuf();

protected :
    virtual int sync ( );
};


}// Core

#endif // LOGSTREAMBUF_HH
