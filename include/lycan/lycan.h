#ifndef __LYCAN_H__
#define __LYCAN_H__

#include <exception>
#include <string>

namespace lycan {

typedef int ModuleId;
typedef int ErrorCode;

const int ModuleIdGlobal = 0;
const int ErrorCodeNoError = 0;

/**
* The Error class which wraps ResultCode.
*/
class Result {
public:
    Result() : m_err_code(ErrorCodeNoError), m_module_id(ModuleIdGlobal) {  }
    Result(ErrorCode code) : m_err_code(code), m_module_id(ModuleIdGlobal) {  }
    Result(ErrorCode code, ModuleId mid) : m_err_code(code), m_module_id(mid) {  }
    Result(const Result& err) : m_err_code(err.code()), m_module_id(ModuleIdGlobal) {  }
    Result(const Result& err, ModuleId mid) : m_err_code(err.code()), m_module_id(mid) {  }
    Result& operator=(const Result& err) { m_err_code = err.code(); return *this; }
    Result& operator=(ErrorCode code) { m_err_code = code; return *this; }
    bool operator==(const Result& err) const { return m_err_code == err.code(); }
    bool operator==(ErrorCode code) const { return m_err_code == code; }
    template <typename T=ErrorCode> T code() const { return static_cast<T>(m_err_code); }
    template <typename T=ModuleId> T moduleId() const { return static_cast<T>(m_module_id); }
    void reset(ErrorCode code) { m_err_code = code; }
    void reset(ErrorCode code, ModuleId mid) { m_err_code = code; m_module_id = mid; }
    bool success() const { return m_err_code == ErrorCodeNoError; }
    bool fail() const { return m_err_code != ErrorCodeNoError; }
private:
    ErrorCode    m_err_code;
    ModuleId     m_module_id;
};


template <typename T> inline T max(T&& a, T&& b) { return a > b ? a : b; }
template <typename T> inline T min(T&& a, T&& b) { return a < b ? a : b; }



}

#endif /* __LYCAN_H__ */
