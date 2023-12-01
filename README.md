# C-TCP-Server
C simple TCP Server using the winsock2 library

First, you need to install the winsock library in VS if you don't have it installed.

- Download vcpkg from https://github.com/microsoft/vcpkg
- Create a vcpkg folder in C:\ so that it looks like this C:\vcpkg. 
- Unzip vcpkg from github to C:\vcpkg.
- Open VS project and on the VS toolbar select Tools->Command Line->Developer Command Promt
- In the cmd that open....
- Write -> cd C:\vcpkg
- Write -> bootstrap-vcpkg.bat
- Write -> vcpkg.exe integrate install
- Write -> vcpkg.exe install curl

After that, you can freely use the winsock libraries in VS


* The server contains a simple socket connection. There is also a dynamic array of users that expands when a new user enters and decreases when a user exits.
* The simplest chat system where messages are sent from one user to the others who are present on the server.


PS:This is just an example of connecting and sending or transferring data from one user to another. You can take this as an example, but don't rely on this code as the only true working version.
