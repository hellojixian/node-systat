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
    }
  ]
}