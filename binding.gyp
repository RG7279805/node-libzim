{
    "variables": {
      "libzim_dir": "<(module_root_dir)/download",
      "libzim_include": "<(module_root_dir)/download/include",
      "libzim_local": "<!(pkg-config --exists libzim && echo true || echo false)",
    },
    "targets": [
        {
            "conditions": [
              ["libzim_local=='true' and OS=='linux'", {
                "link_settings": {
                  "ldflags": [
                      "<!(pkg-config --libs-only-other --libs-only-L libzim)",
                  ],
                  "libraries": [
                      "<!(pkg-config --libs-only-l libzim)",
                  ],
                },
              }],
              ["libzim_local!='true' and OS=='linux'", {
                "include_dirs": [
                  "<(libzim_include)", 
                ],
                "libraries": [
                  "-Wl,-rpath,<(libzim_dir)",
                  "-L<(libzim_dir)",
                  "<(libzim_dir)/libzim.so.6",
                ],
              }],
              ["OS=='mac'", {
                "cflags+": ["-fvisibility=hidden"],
                "xcode_settings": {
                  "GCC_SYMBOLS_PRIVATE_EXTERN": "YES",  # -fvisibility=hidden
                },
                "libraries": [ "-lzim" ]
              }],
            ],
            "target_name": "zim_binding",
            "cflags!": [ "-fno-exceptions" ],
            "cflags_cc!": [ "-fno-exceptions" ],
            "cflags": ["-g"],
            "cflags_cc": [ "-std=c++17", "-fexceptions", "-g" ],
            "sources": [ 
                "src/module.cc", 
                "src/article.cc", 
                "src/reader.cc", 
                "src/writer.cc",
            ],
            "include_dirs": [
                "<!@(node -p \"require('node-addon-api').include\")",
            ],
            "defines": [
                "NAPI_CPP_EXCEPTIONS"
            ],
        }
    ]
}
