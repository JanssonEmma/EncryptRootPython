## Build the depending Crypto++ libraries

- Go to Extern/cryptopp and open cryptest solution
- Select the correct Build Configuration (has to match with the Encryption project)
  - Project -> Settings -> C/C++ -> Code Generation -> Runtime Library
- Build cryptlib 
- Include the build result library into your project
  - Project -> Settings -> Linker -> Include -> Additional Dependencies
  - link the lib file `Extern\lib\cryptlib-Debug.lib;%(AdditionalDependencies)`
  
