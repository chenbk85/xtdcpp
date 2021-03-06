#ifndef COMMON_LOG_HXX_
# define COMMON_LOG_HXX_

namespace xtd {

template<typename ... Args>
void
logger::emerg(const string& p_module, const string& p_format, Args... p_args)
{
  get().log(level::emerg, p_module, p_format, p_args...);
}

template<typename ... Args>
void
logger::alert(const string& p_module, const string& p_format, Args... p_args)
{
  get().log(level::alert, p_module, p_format, p_args...);
}

template<typename ... Args>
void
logger::crit(const string& p_module, const string& p_format, Args... p_args)
{
  get().log(level::crit, p_module, p_format, p_args...);
}

template<typename ... Args>
void
logger::err(const string& p_module, const string& p_format, Args... p_args)
{
  get().log(level::err, p_module, p_format, p_args...);
}

template<typename ... Args>
void
logger::warning(const string& p_module, const string& p_format, Args... p_args)
{
  get().log(level::warning, p_module, p_format, p_args...);
}


template<typename ... Args>
void
logger::info(const string& p_module, const string& p_format, Args... p_args)
{
  get().log(level::info, p_module, p_format, p_args...);
}

template<typename ... Args>
void
logger::notice(const string& p_module, const string& p_format, Args... p_args)
{
  get().log(level::notice, p_module, p_format, p_args...);
}

template<typename ... Args>
void
logger::debug(const string& p_module, const string& p_format, Args... p_args)
{
  get().log(level::debug, p_module, p_format, p_args...);
}

template<typename... Arguments>
string
logger::format_vargs(string const& p_fmt, Arguments&&... p_args)
{
  boost::format l_format(p_fmt);
  int l_unroll[] {0, (l_format % std::forward<Arguments>(p_args), 0)...};
  static_cast<void>(l_unroll);
  return boost::str(l_format);
}



template<typename ... Args>
void
logger::log(level         p_level,
            const string& p_module,
            const string& p_format,
            Args...       p_args)
{
  if (false == m_initialized)
    initialize();

  if (getLevel(p_module) >= p_level)
  {
    string l_message;
    try {
      l_message = format_vargs("[%s] (%s) " + p_format + " in %s:%s:%d ", stringOf(p_level), p_module, p_args...);
    } catch (std::exception& p_error) {
      try {
        l_message = format_vargs("[%s] (%s) " + p_format, stringOf(p_level), p_module, p_args...);
      } catch (std::exception& p_error) {
        l_message = "unable to render log format : " + p_format;
      }
    }
    wrap_syslog(static_cast<int32_t>(p_level), l_message.c_str());
  }
}

}

#endif // !COMMON_LOG_HXX_
