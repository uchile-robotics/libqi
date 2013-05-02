/*
**
** Author(s):
**  - Cedric GESTES <gestes@aldebaran-robotics.com>
**
** Copyright (C) 2013 Aldebaran Robotics
*/

#include "pyservicedirectory.hpp"
#include <qimessaging/servicedirectory.hpp>
#include <boost/python.hpp>
#include "pyfuture.hpp"

qiLogCategory("qimpy");

namespace qi {
  namespace py {

    class PyServiceDirectory : public qi::ServiceDirectory
    {
    public:
      //return a future, or None (and throw in case of error)
      boost::python::object listen(const std::string &url, bool _async=false) {
        if (_async)
          return boost::python::object(toPyFuture(ServiceDirectory::listen(url)));
        else {
          qi::Future<void> fut = ServiceDirectory::listen(url);
          //TODO: unlock GIL here?
          fut.value(); //throw on error
          return boost::python::object();
        }
      }

      //override because python do not know qi::Url
      boost::python::list endpoints() const {
        boost::python::list ret;
        std::vector<qi::Url>     eps = ServiceDirectory::endpoints();
        for (unsigned int i = 0; i < eps.size(); ++i) {
          ret.append(eps.at(i).str());
        }
        return ret;
      }

    };

    void export_pyservicedirectory() {
      boost::python::class_<PyServiceDirectory>("ServiceDirectory")
          .def("listen", &PyServiceDirectory::listen, (boost::python::arg("url"), boost::python::arg("_async") = false))
          .def("setIdentity", &PyServiceDirectory::setIdentity)
          .def("close", &PyServiceDirectory::close)
          .def("endpoints", &PyServiceDirectory::endpoints);
    }

  }
}
