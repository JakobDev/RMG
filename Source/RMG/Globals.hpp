#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include "M64P/Wrapper/Api.hpp"
#include "Thread/EmulationThread.hpp"
#include "UserInterface/Widget/OGLWidget.hpp"
#include "Utilities/Settings.hpp"
#include "Utilities/Logger.hpp"
#include "Utilities/Plugins.hpp"
#include "Utilities/RomBrowserCache.hpp"

extern Utilities::Logger g_Logger;
extern Utilities::Settings g_Settings;
extern Utilities::Plugins g_Plugins;
extern Utilities::RomBrowserCache g_RomBrowserCache;
extern M64P::Wrapper::Api g_MupenApi;
extern UserInterface::Widget::OGLWidget *g_OGLWidget;
extern Thread::EmulationThread *g_EmuThread;
extern QThread *g_RenderThread;

#endif // GLOBALS_HPP
