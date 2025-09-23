import sys, os

gs_templates_folder= '/usr/local/share/gs_templates'

build_project_name = 'linux-sdk'
project_name = u'SDK for Linux'
filename_pdf = u'gs-man-linux-sdk-'

copyright_name = u'2016, GomSpace'
company_name = u'GomSpace'

front_name = "GomSpace"
front_doc_type = "Manual"
front_doc_category = "Software Documentation"
front_image = "linux_sdk_front"

execfile(os.path.join(os.path.abspath(gs_templates_folder), 'conf_template.py'))

exclude_patterns = ['clients', 'lib/libcsp/INSTALL.rst']