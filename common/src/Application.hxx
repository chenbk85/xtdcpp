#ifndef COMMON_APPLICATION_HXX_
# define COMMON_APPLICATION_HXX_

# include <boost/lexical_cast.hpp>

namespace xtd {


template<typename... Arguments>
void
Application::error_nohelp(int p_code, const string& p_format, Arguments&&... p_args) const
{
  string l_message = logger::format_vargs(p_format, p_args...);
  std::cerr << l_message << endl;
  logger::crit("common.application", "%s", l_message);
  exit(p_code);
}

template<typename... Arguments>
void
Application::error(int p_code, const string& p_format, Arguments&&... p_args) const
{
  string l_message = logger::format_vargs(p_format, p_args...);
  std::cerr << "error : " << l_message << endl;
  logger::crit("common.application", "error : %s", l_message);
  usage();
  exit(p_code);
}

template<typename... Arguments>
void
Application::warn(const string& p_format, Arguments&&... p_args) const
{
  string l_message = logger::format_vargs(p_format, p_args...);
  std::cerr << "warning : " << l_message << endl;
  logger::warning("common.application", "warning : %s", l_message);
}


template<typename T, template<class> class TCollection>
Application::t_callback
Application::bindAccumulator(TCollection<T>& p_target) const
{
  return [&p_target, this](const string& p_value, const t_option& p_opt) {
    T l_value;
    try {
      l_value = boost::lexical_cast<T, string>(p_value);
    }
    catch (boost::bad_lexical_cast& l_error) {
      error_nohelp(1, "invalid option -%c='%s', nost castable", p_opt.m_shortOpt, p_value);
    }
    p_target.insert(p_target.end(), l_value);
  };
}

template<typename T>
Application::t_callback
Application::bindNumber(T& p_target, T p_min, T p_max) const
{
  return [&p_target, p_min, p_max, this](const string& p_value, const t_option& p_opt) {
    try {
      p_target = boost::lexical_cast<T>(p_value);
    }
    catch (boost::bad_lexical_cast) {
      error(1, "invalid option -%c=%s, must be a number", p_opt.m_shortOpt, p_value);
    }

    if (p_target < p_min)
      error(1, "invalid option -%c=%s, must be superior to %d",
            p_opt.m_shortOpt, p_value, p_min);

    if (p_target > p_max)
      error(1, "invalid option -%c=%s, must be inferior to %d",
            p_opt.m_shortOpt, p_value, p_max);
  };
}


template<typename T, class Iterable>
Application::t_callback
Application::bindValues(T& p_target, const Iterable& p_values) const
{
  return [&p_target, &p_values, this](const string& p_value, const t_option& p_opt) {
    try {
      p_target = boost::lexical_cast<typename Iterable::value_type>(p_value);
    }
    catch (boost::bad_lexical_cast) {
      error(1, "invalid option -%c=%s, has invalid type", p_opt.m_shortOpt, p_value);
    }

    if (p_values.end() == std::find(p_values.begin(), p_values.end(), p_target))
      error(1, "invalid option -%c=%s, value not in range", p_opt.m_shortOpt, p_value);
  };
}

template<typename T>
Application::t_callback
Application::bindValueIfGiven(T& p_target, const T& p_default) const
{
  return [&p_target, &p_default, this](const string&, const t_option&) {
    p_target = p_default;
  };

}

template<typename T>
Application::t_callback
Application::bindCallback(T p_method) const
{
  return [&p_method, this](const string&, const t_option&) {
    p_method();
  };
}




}

#endif // !COMMON_APPLICATION_HXX_
