project('NucleusLogging', 'cpp', default_options: ['cpp_std=c++17'])

log_level = get_option('log_level')
log_level_define = ''

if log_level == 'TRACE'
  log_level_define = '1'
elif log_level == 'DEBUG'
  log_level_define = '2'
elif log_level == 'WARN'
  log_level_define = '3'
elif log_level == 'ERROR'
  log_level_define = '4'
elif log_level == 'FATAL'
  log_level_define = '5'
endif

nucleus_logging_dep = declare_dependency(
  include_directories: include_directories('include'),
  compile_args: ['-DLOG_LEVEL=' + log_level_define],
)

install_headers('include/ErrorLogger.h',
                subdir: 'NucleusLogging')

subdir('examples')