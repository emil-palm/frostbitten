require 'mkmf'
extension_name = 'native'
dir_config(extension_name)
create_makefile("frostbitten/#{extension_name}")