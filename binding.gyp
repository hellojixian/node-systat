{
  "targets": [
    {
      "target_name": "systat",
      "sources": [                    
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
          },
          "sources": [        
            "./src/sensor/it8728f.cc",
            "./src/system/osx.cc",
            
          ],  
        }],
        ['OS=="freebsd"', {
          'libraries': ["-ldevstat","-lc"],
          "sources": [        
            "./src/sensor/it8728f.cc",
            "./src/system/freebsd.cc",
            
          ],  
        }]
      ]
    }
  ]
}