#ifndef NETWORK_BASE_CLIENTTHREADMANAGER_HH_
# define NETWORK_BASE_CLIENTTHREADMANAGER_HH_

# include <boost/asio.hpp>
# include <boost/thread.hpp>
# include <boost/thread/mutex.hpp>
# include <boost/noncopyable.hpp>
# include <boost/shared_ptr.hpp>
# include <boost/make_shared.hpp>
# include <types.hh> //libcommon

namespace xtd {
namespace network {
namespace base {

class ThreadManager : boost::noncopyable
{
public:
  typedef map<size_t, boost::shared_ptr<boost::thread> > threadMap_t;

private:
  ThreadManager(void);
  ~ThreadManager(void);

public:
  static ThreadManager&    getInstance(void);
  void                     createThread(const size_t p_threadId);
  boost::asio::io_service& getIoService(void);

private:
  // Attention à l'initialisation d'un pointeur static lors des tests unitaires qui cachent un fork
  // pour garantir l'ordre d'initialisation, création du pointeur au constructeur
  static boost::asio::io_service*                  m_ioService;
  static boost::mutex                              m_mutex;
  static ThreadManager*                            m_threadManager;
  boost::shared_ptr<boost::asio::io_service::work> m_workPtr;
  threadMap_t                                      m_threadMap;
  boost::shared_ptr<boost::thread>                 m_threadPtr;
};

}}} // end namespaces

#endif // !NETWORK_BASE_CLIENTTHREADMANAGER_HH_
