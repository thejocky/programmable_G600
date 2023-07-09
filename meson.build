project('programmable_g600', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3',
                     'cpp_std=c++20'])


incdir = include_directories('include')

src_files = ['source/profile.cpp', 'source/device.cpp', 'source/main.cpp',
  'source/serial/serial_zzz.cpp']

zzz = dependency('zzzlib', version : '>=1.0.0')

exe = executable('programmable_g600', src_files,
  install : true, include_directories : incdir,
  dependencies : zzz)

instruction_test = executable('prog', 'test_files/test_instruction_system.cpp')
test('instruction test', instruction_test)


