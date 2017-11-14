# versatile
wireshark: the dissector files for the flex ID

resolver: resolver code

	- 2016/resolver_per.py: socketIO version

	-     /resolver.py: non socketIO version

	- 2017/resolver.py: pub/sub version

compile and install
 - source code: flex-1.0.tar.gz
   tar xvzf flex-1.0.tar.gz
   cd flex-1.0
   make && sudo make install
 - github: git clone https://github.com/hw5773/versatile.git
   cd versatile
   make && sudo make uninstall

uninstall and clean
 - in the source code directory
   sudo make uninstall && make clean
