{
  "targets": [
    {
      "target_name": "systat",
      "sources": [ 
      	"./src/systat.cc",
      	"./src/sensor/it8728f.cc",
      	"./src/system/bsd.cc"      	
      ],      
      "cflags_cc": ["-std=c++11","-Wno-unused-function"],      
    }
  ]
}