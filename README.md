### Kio Extras for Plasma 6 / Doplhin with Samba protocol enabled

You need to install for Debian Trixie these packages:
1. libkdsoap-bin, libkdsoap-qt6-2, libkdsoap-server-qt6-2
2. As I'm writing this file, 06 Feb 2025, KDSoap Discovery Client is not in Trixie / SID. You need to install manually from Debian Packages:

    a. Download lib KDSoap Discovery Client: https://packages.debian.org/sid/libkdsoapwsdiscoveryclient0
   
    b. Install manually: `apt install ./libkdsoapwsdiscoveryclient0_0.4.0-2_amd64.deb` or `dpkg -i ./libkdsoapwsdiscoveryclient0_0.4.0-2_amd64.deb`
  
4. Then download and manually install Kio Extras:
 
    `apt install ./kio-extras_24.12.0-3_amd64.deb ./kio-extras-data_24.12.0-3_all.deb` or `dpkg -i ./kio-extras_24.12.0-3_amd64.deb ./kio-extras-data_24.12.0-3_all.deb`

5. Restart Doplhin and check if `smb://` is working.
