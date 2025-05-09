log_level = get_option('log_level')
log_level_define = ''

if log_level == 'DEBUG'
  log_level_define = 'LogLevel::DEBUG'
elif log_level == 'INFO'
  log_level_define = 'LogLevel::INFO'
elif log_level == 'WARN'
  log_level_define = 'LogLevel::WARN'
elif log_level == 'ERROR'
  log_level_define = 'LogLevel::ERROR'
elif log_level == 'FATAL'
  log_level_define = 'LogLevel::FATAL'
endif

logger_sources = [
  'Logger.cpp',
  'ConsoleSink.cpp',
  'RotatingFileSink.cpp',
]

logger_lib = library('NucleusLogging', logger_sources,
  include_directories: include_directories('../include'),
  cpp_args: ['-DDEFAULT_LOG_LEVEL=' + log_level_define],
  install: true
)
