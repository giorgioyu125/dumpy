project('dumpy', 'c')

cc = meson.get_compiler('c')
libm = cc.find_library('m', required : false)

src = [
    'src/main.c'
]

executable('dumpy',
            sources : src,
            dependencies : libm
)
