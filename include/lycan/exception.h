#ifndef __LYCAN_EXCEPTION_H__
#define __LYCAN_EXCEPTION_H__

#include <exception>
#include <string>
#include <sstream>

namespace lycan {
    class Exception : public std::exception {
    public:
        Exception() : m_code(0), m_line(0) {  }
        Exception(int t_code, const std::string& t_description)
            : m_code(t_code)
            , m_description(t_description)
            , m_func("")
            , m_file("")
            , m_line(0) {
            std::stringstream ss;
            ss << " -*- Lycan Exception -*-  Code[" << m_code << "] " << m_description << std::endl;
            m_msg = ss.str();
        }
        Exception(int t_code, const std::string& t_description, const std::string& t_func, const std::string& t_file, int t_line)
            : m_code(t_code)
            , m_description(t_description)
            , m_func(t_func)
            , m_file(t_file)
            , m_line(t_line) {
            std::stringstream ss;
            ss << " -*- Lycan Exception -*-  Code[" << m_code << "] " << m_func
                << "@" << m_file << ":" << m_line << " " << m_description << std::endl;
            m_msg = ss.str();
        }

        virtual ~Exception() throw() { }
        virtual const char* what() const throw() override { return m_msg.c_str(); }
    private:
        int         m_code;         ///< error code
        std::string m_description;  ///< error description
        std::string m_func;         ///< function name where the exception is throwed
        std::string m_file;         ///< source file name where the exception has occurred
        int         m_line;         ///< line number in the source file
        std::string m_msg;          ///< message of exception
    };
}

#endif /* __LYCAN_EXCEPTION_H__ */
