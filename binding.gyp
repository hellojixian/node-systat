{
  "targets": [
    {
      
      "sources": [           
            "./src/sensor/it8728f.cc",         
            "./src/systat.cc"
          ],
      "cflags_cc": ["-std=c++11",
				"-Wno-unused-function",
				"-Wno-unused-variable"],
      "cflags!": [ '-fno-exceptions' ],
      "cflags_cc!": [ '-fno-exceptions' ],
      "conditions": [
        ['OS=="mac"', {
          "target_name": "systat-darwin",
          'xcode_settings': {
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
          },
          "sources": [                    
            "./src/system/osx.cc",
            
          ],  
        }],
        ['OS=="freebsd"', {
          "target_name": "systat-freebsd",
          'libraries': ["-ldevstat","-lc"],
          "sources": [                    
            "./src/system/freebsd.cc",            
          ],  
        }]
      ]
    }
  ]
}