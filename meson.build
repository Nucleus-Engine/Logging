project('NucleusLogging', 'cpp', default_options: ['cpp_std=c++17'])

subdir('src')

install_headers('include/Logger.h',
                'include/LogLevel.h',
                'include/LogSink.h',
                'include/LogMessage.h',
                'include/ConsoleSink.h',
                'include/RotatingFileSink.h',
                subdir: 'NucleusLogging')

subdir('examples')
