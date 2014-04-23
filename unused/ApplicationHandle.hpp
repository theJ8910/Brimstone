#ifndef APPLICATION_HANDLE_H
#define APPLICATION_HANDLE_H




#if defined( BS_BUILD_WINDOWS )

#include <Windows.h>
namespace Brimstone {
typedef HINSTANCE ApplicationHandle;
}

#elif defined( BS_BUILD_LINUX )

namespace Brimstone {
typedef void* ApplicationHandle;
}

#endif




#endif //APPLICATION_HANDLE_H