/*
Per installare Open SSL sui propri sistemi per poi adoperarla in Visual Studio si possono seguire questi passaggi:

Installare VSCPKG:
git clone  https://github.com/Microsoft/vcpkg
cd vcpkg
bootstrap-vcpkg.bat
vcpkg integrate install

https://learn.microsoft.com/it-it/vcpkg/

Installare Open SSL con VSCPKG:
vcpkg install openssl

altrimenti

vcpkg search ssl
vcpkg install openssl-windows --triplet x64-windows



*/