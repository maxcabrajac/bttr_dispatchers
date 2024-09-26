project('bttr_dispatchers', 'cpp',
  version: '1.0',
  default_options: ['buildtype=release'],
)

hyprpm_toml = run_command('cat', 'hyprpm.toml', check: true).stdout().strip().split('\n')
foreach line: hyprpm_toml
  if line.startswith('authors')
    # line = 'authors = ["<author>", "<other_author>"]'
    author = line.split('"').get(1)
    message(f'author: @author@')
  endif
  if line.startswith('description')
    # line = 'description = "<description>"'
    description = line.split('"').get(1)
    message(f'description: @description@')
  endif
endforeach


cpp_compiler = meson.get_compiler('cpp')
if cpp_compiler.has_argument('-std=c++26')
  add_global_arguments('-std=c++26', language: 'cpp')
else
  error('Could not configure current C++ compiler (' + cpp_compiler.get_id() + ' ' + cpp_compiler.version() + ') with required C++ standard (C++26)')
endif

add_global_arguments('-DWLR_USE_UNSTABLE', language: 'cpp')
add_global_arguments('-DBTTR_NAME="@0@"'.format(meson.project_name()), language: 'cpp')
add_global_arguments('-DBTTR_VERSION="@0@"'.format(meson.project_version()), language: 'cpp')
add_global_arguments(f'-DBTTR_AUTHOR="@author@"', language: 'cpp')
add_global_arguments(f'-DBTTR_DESC="@description@"', language: 'cpp')

globber = run_command('find', './src', '-name', '*.cpp', check: true)
src = globber.stdout().strip().split('\n')
include = include_directories('include')

shared_module(meson.project_name(), src,
  dependencies: [
    dependency('hyprland'),
    dependency('pixman-1'),
    dependency('libdrm'),
  ],
  include_directories: include,
  install: true,
)
