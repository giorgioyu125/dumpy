project('dumpy', 'c')

inc = include_directories('include')

cc = meson.get_compiler('c')
libm = cc.find_library('m', required : false)

src = [
    'src/main.c'
]

executable('dumpy',
            sources : src,
            include_directories : inc,
            dependencies : libm
)
