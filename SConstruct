env = Environment()
env['CXXFLAGS'] = '-std=c++11'
Export('env')
VariantDir('build', 'src', duplicate=0)
SConscript('build/SConscript')
