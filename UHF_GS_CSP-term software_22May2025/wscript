#!/usr/bin/env python
# encoding: utf-8

import os
import time

APPNAME = 'csp-term'
VERSION = 'v2.9'

top    = '.'
out    = 'build'

# Scan modules in local lib dir
modules = ['lib/libcsp', 'lib/libparam', 'lib/libutil', 'lib/libgosh', 'lib/libftp', 'lib/liblog', 'clients']
bin_modules = {
    'lib/liblog': [],
    'lib/libgosh': [],
    'lib/libparam': [],
    'lib/libftp': [],
    'lib/libfp': [],
    'lib/libhk': [],
}

# Addon modules
addons = ['fp', 'hk']
addon_bundles = {
    'mission-lib': ['fp', 'hk'],
}

addon_modules = []
for addon in addons:
    module = 'lib/lib' + addon
    addon_modules.append(module)
    if os.path.exists(module) or os.path.exists(module + '.tar.gz'):
        modules.append(module)

def unpack_modules_lib():
    for bin_mod in bin_modules.keys():
        if os.path.exists(bin_mod + '.tar.gz'):
            if not os.path.exists(bin_mod + '/'):
                os.system('tar -x -f ' + bin_mod + '.tar.gz lib')

def unpack_modules_build():
    for bin_mod in bin_modules.keys():
        if os.path.exists(bin_mod + '.tar.gz'):
            if not os.path.exists('./obj'):
                os.system('mkdir ./obj')
            os.system('tar -x -C ./obj -f ' + bin_mod + '.tar.gz build')

def options(ctx):
    ctx.load('gcc')
    if os.path.exists('eclipse.py'):
        ctx.load('eclipse')
    unpack_modules_lib()
    ctx.recurse(modules)
    ctx.add_option('--enable-mission-lib', action='store_true', help='Enable Mission Lib')
    ctx.add_option('--full-source', action='store_true', help='Include full source in dist')

def configure(ctx):
    ctx.load('gcc')
    
    ctx.find_program("sphinx-build", var="SPHINX_BUILD", mandatory=False)
    
    ctx.env.append_unique('FILES_CSPTERM', 'src/*.c')
    ctx.env.append_unique('LIBS_CSPTERM', ['rt', 'pthread', 'util'])

    # External Libs
    ctx.check_cfg(package='libcurl', args='--cflags --libs', uselib_store='CURL')
    ctx.env.LIBS += ctx.env.LIB_CURL
    ctx.env.INCLUDES += ctx.env.INCLUDES_CURL

    # Options for CSP
    ctx.options.with_os = 'posix'
    ctx.options.enable_rdp = True
    ctx.options.enable_qos = True
    ctx.options.enable_crc32 = True
    ctx.options.enable_hmac = True
    ctx.options.enable_xtea = True
    ctx.options.enable_promisc = True
    ctx.options.enable_if_kiss = True
    ctx.options.enable_if_can = True
    ctx.options.enable_if_zmqhub = True
    ctx.options.disable_stlib = True
    ctx.options.with_rtable = 'cidr'
    ctx.options.enable_can_socketcan = True
    ctx.options.with_driver_usart = 'linux'
    ctx.options.with_router_queue_length = 100
    ctx.options.with_conn_queue_length = 100
    
    # Options for clients
    ctx.options.enable_nanopower2_client = True
    ctx.options.enable_bpx_client = True

    # Options for libftp
    ctx.options.enable_ftp_server = True
    ctx.options.enable_ftp_client = True
    ctx.options.enable_ftp_backend_file = True
    
    # Options for liblog
    ctx.options.enable_log_node = True

    # Options for libutil
    ctx.options.clock = 'linux'
    ctx.options.enable_driver_debug = True
    ctx.options.with_log = 'cdh'
    ctx.options.enable_vmem = True
    ctx.options.enable_lzo = True
    
    # Options for libparam
    ctx.options.enable_param_host = True
    ctx.options.enable_param_server = True
    ctx.options.enable_param_client = True
    ctx.options.param_lock = 'none'
    ctx.options.param_max_tables = 40

    # Options for libgosh
    ctx.options.enable_gosh_remote = True
    ctx.options.enable_gscript = True

    if 'lib/libhk' in modules:
        # Options for libhk
        ctx.options.enable_hk_file = True
        ctx.options.enable_hk_parser = True

    if 'lib/libfp' in modules:
        # Options for libfp
        ctx.options.enable_fp_client = True

    if not 'lib/libfp' in modules and not 'lib/libhk' in modules:
        ctx.env.append_unique('EXCLUDES_CSPTERM', ['src/mission_init.c'])
        ctx.options.enable_mission_lib = False

    # Options for clients
    if ctx.options.with_clients == 'all':
        ctx.options.with_clients = None
    elif ctx.options.with_clients == None:
        ctx.options.with_clients = ' '

    revision = os.popen('git describe --always --dirty=+ 2> /dev/null || echo {0}'.format(VERSION)).read().strip()
    ctx.define('CSPTERM_VERSION', revision)

    # Recurse and write config
    ctx.write_config_header('include/conf_cspterm.h', top=True, remove=True)
    ctx.recurse(modules, mandatory=False)    

    unpack_modules_build()

def build(ctx):
    ctx(export_includes=['include', 'client'], name='include')
    ctx.recurse(modules, mandatory=False)
    use = ['csp', 'param', 'util', 'gosh', 'ftp', 'log']
    for idx in range(0, len(addons)):
        if addon_modules[idx] in modules:
            use.append(addons[idx])
    use.extend(ctx.env.LIBCLIENTS_USE)
    for bin_mod in bin_modules.keys():
        # Check for source code
        c_files = ctx.path.ant_glob(bin_mod + '/**/*.c')
        if len(c_files) > 0:
            continue
        bin_objs = ctx.path.ant_glob('obj/build/' + bin_mod + '/**/*.o')
        for obj in bin_objs:
            ctx.env.append_unique('LINKFLAGS_CSPTERM', '../' + obj.relpath())
    ctx.program(
        source=ctx.path.ant_glob(ctx.env.FILES_CSPTERM, excl=ctx.env.EXCLUDES_CSPTERM), 
        target='csp-term', 
        linkflags=ctx.env.LINKFLAGS_CSPTERM,
        use=use,
        lib=ctx.env.LIBS_CSPTERM + ctx.env.LIBS + ['m']
        )

def doc(ctx):
    os.system('rm -rf doc/html')
    os.system('rm -rf doc/*.pdf')
    VERSION = os.popen('git describe --always --dirty=-dirty 2> /dev/null || echo unknown').read().strip()
    ctx(
        rule   = "${SPHINX_BUILD} -q -c ./doc -b html -D release="+VERSION+" -d build/doc/doctrees . build/doc/html -t linux_sdk -t addon_lib",
        cwd    = ctx.path.abspath(),
        source = ctx.path.ant_glob('**/*.rst'),
        target = './doc/html/doc/index.html',
        )
    ctx(
        rule   = "${SPHINX_BUILD} -q -c ./doc -b latex -D release="+VERSION+" -d build/doc/doctrees . build/doc/latex -t linux_sdk -t addon_lib",
        cwd    = ctx.path.abspath(),
        source = ctx.path.ant_glob('**/*.rst'),
        target = './doc/latex/linux-sdk.tex',
        )
    ctx(
        cwd    = ctx.path.abspath(),
        rule   = 'make -C build/doc/latex all-pdf 2>&1 > /dev/null || echo make error',
        source = './doc/latex/linux-sdk.tex',
        target = './doc/latex/linux-sdk.pdf',
        )

from waflib.Build import BuildContext
class Doc(BuildContext):
   cmd = 'doc'
   fun = 'doc'
   
def dist(ctx):
    global bin_modules

    if not ctx.path.find_node('build/doc/latex/linux-sdk.pdf'):
        ctx.fatal('You forgot to run \'waf doc\' first, we need to include the documentation in the output')

    # Remove/clean all libs
    os.system('rm -f lib/*.gz')
    # Remove/clean all dist files
    os.system('rm -f gs-sw-*.*')

    # Get git version
    VERSION = os.popen('git describe --always --dirty=+ 2> /dev/null || echo unknown').read().strip()

    os.system('cp build/doc/latex/linux-sdk.pdf doc/gs-man-linux-sdk-'+VERSION+'.pdf')
    os.system('cp -r build/doc/html doc/html')

    ctx.base_name = 'gs-sw-linux-sdk-' + VERSION + '-x86_64'
    if ctx.options.full_source:
        ctx.base_name += '-source'
        bin_modules.clear()
    ctx.excl = ['**/.waf-1*', '**/*~', '**/*.pyc', '**/*.swp', '**/.lock-w*', '**/build', '**/.settings', '**/.git*']
    ctx.files = ctx.path.ant_glob(['waf', 'wscript', 'eclipse.py', 'client/*', 'src/*', 'doc/**', 'CHANGELOG'])
    for module in modules:
        # Default include all files in module_files list
        module_files = ctx.path.ant_glob(module + '/**')
        # Process all binary and add-on modules
        if module in bin_modules or module in addon_modules:
            if module in bin_modules:
                file_set_list = bin_modules[module]
                module_files = []
                if len(file_set_list):
                    for file_set in file_set_list:
                        module_files.extend(ctx.path.ant_glob(module + file_set))
                else:
                    module_files.extend(ctx.path.ant_glob(module + '/**/*.h'))
                    module_files.extend(ctx.path.ant_glob(module + '/**/wscript'))
            module_file_list = ''
            for obj in module_files:
                module_file_list += ' ' + os.path.relpath(str(obj.abspath()))
            # Bundle all module files in tar file
            tar_file = module + '.tar.gz'
            cmd = 'tar cfz ' + tar_file + ' build/' + module + module_file_list
            os.system(cmd)
            if module in addon_modules:
                # Don't include add-ons in main dist
                module_files = []
            else:
                # If not add-on, include the tar file for binary modules in dist
                module_files = ctx.path.ant_glob(tar_file)
        # Add all files in module_files to list of files to include in dist
        ctx.files.extend(module_files)
    # Bundle add-ons in tar archives
    for bundle in addon_bundles.keys():
        tar_file_dist = './gs-sw-' + bundle + '-' + VERSION + '-linux-x86_64'
        if ctx.options.full_source:
            tar_file_dist += '-source'
        tar_file_dist += '.tar.gz'
        bundle_file_list = ''
        for module in addon_bundles[bundle]:
            bundle_file_list += ' lib/lib' + module + '.tar.gz'
        cmd = 'tar cfz ' + tar_file_dist + ' ' + bundle_file_list
        os.system(cmd)
