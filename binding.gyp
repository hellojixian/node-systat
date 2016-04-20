{
  "targets": [
    {
      "target_name": "systat",
      "sources": [       	
      	"./src/sensor/it8728f.cc",
      	"./src/system/bsd.cc",
        "./src/systat.cc"
      ],      
      "cflags_cc": ["-std=c++11",
				"-Wno-unused-function",
				"-Wno-unused-variable"],
      "cflags!": [ '-fno-exceptions' ],
      "cflags_cc!": [ '-fno-exceptions' ],
      "conditions": [
        ['OS=="mac"', {
          'xcode_settings': {
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
          }
        }],
        ['OS=="freebsd"', {
          'libraries': ["-ldevstat","-lc"]
        }]
      ]
    }
  ]
}