#ifndef BS_APPLICATION_H
#define BS_APPLICATION_H




//Includes
#include "ApplicationHandle.hpp"
#ifdef BS_BUILD_WINDOWS

#include "windows/WindowsApplication.hpp"
namespace Brimstone {
namespace Private {
typedef WindowsApplication ApplicationImpl;
}
}

#elif BS_BUILD_LINUX

#include "linux/LinuxAppliation.hpp"
namespace Brimstone {
namespace Private {
typedef LinuxApplication ApplicationImpl;
}
}

#endif




namespace Brimstone {

class Application {
private:
    Private::ApplicationImpl m_cImpl;
public:
    ApplicationHandle getHandle();
};

}




#endif //BS_APPLICATION_H