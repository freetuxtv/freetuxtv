   
   ## FreetuxTV
   - ### FreetuxTV is a Linux media player based on GTK+ and LibVLC for watching and recording free WebTV, WebRadio and WebCam channels on a PC.
   - ### It can also be used for importing, watching and recording TV channels from [Tvheadend streaming server.](https://github.com/tvheadend/tvheadend)
   
   ### Features
   - Add new groups of channels from M3U playlists 
   - Import and play TV and Radio channels from Tvheadend
   - Listen WebRadio services
   - Play TV (Free, SFR and WebTV)
   - Play in fullscreen mode
   - Play in mini mode
   - Record selected channel
   - Support for keyboard multimedia keys
   - Timer recording

![Main Window](https://github.com/freetuxtv/freetuxtv/blob/master/data/freetuxtv.png)

### Requirements
   - gtk+ (>= 2.12.0)
   - glib (>= 2.26.0)
   - libsqlite3 (>= 3.4.0)
   - libvlc (>= 0.8.6)
   - libcurl (>= 7.18.0)
   - libnotify (>= 0.4)

   ### Bug reports, feature requests or feedback
   - [GitHub issue tracker](https://github.com/freetuxtv/freetuxtv/issues)

   ### FreetuxTV project translations
   - [Launchpad translations](https://translations.launchpad.net/freetuxtv)

   ### Ubuntu PPA repositories
   - [Git build stable](https://launchpad.net/~freetuxtv/+archive/ubuntu/freetuxtv) package
   - [Git build testing](https://launchpad.net/~freetuxtv/+archive/ubuntu/freetuxtv-dev) package
   
   ### Installing on Ubuntu
   1) Add PPA repository
      - Add **stable** or **develop git** repository
         ```
         sudo add-apt-repository ppa:freetuxtv/freetuxtv
         sudo add-apt-repository ppa:freetuxtv/freetuxtv-dev
         ```
   2) Run system update and install FreetuxTV
      ```
      sudo apt update
      sudo apt install freetuxtv
      ```
   4) Run FreetuxTV
      ```
      freetuxtv
      ```

   ### Installing from tarball archive
   1) Download release tarball from [GitHub release website](https://github.com/freetuxtv/freetuxtv/releases) into your home folder
   2) Untar tarball archive and enter into new freetuxtv folder
      ```
      tar xvzf freetuxtv-<version>.tar.gz
      cd freetuxtv-<version>
      ```
   3) Run configure script (you can see more options with - help) and install FreetuxTV
      ```
      ./configure --prefix=/usr
      sudo make install
      ```
   4) Run FreetuxTV
      ```
      freetuxtv
      ```

   ### Installing from GIT repository
   1) Install build dependencies according to your distribution
      - Ubuntu
        ```
        sudo apt install git gcc make autoconf automake intltool libtool libtool-bin \
                           gettext libgtk-3-dev libsqlite3-0 libsqlite3-dev libcurl4 \
                           libcurl4-openssl-dev vlc libvlc-dev libnotify-dev
        ```
      - openSUSE
        ```
        su root
        zypper install git gcc autoconf automake make gettext-tools gtk2-devel vlc vlc-devel \
                         sqlite3-devel libcurl-devel libnotify-devel
        exit
        ```
   2) Clone sources from GitHub and enter into cloned freetuxtv folder
       ```
       git clone https://github.com/freetuxtv/freetuxtv.git
       cd freetuxtv
       ```
   3) Build and install FreetuxTV
       - Using **autotools**
         ```
         ./autogen.sh
         ./configure --prefix=/usr
         make
         sudo make install
         ```
       - Using **cmake**
         ```
         cmake -G "Unix Makefiles" .
         make
         cpack -G DEB
         gdebi freetuxtv-[...].deb
         ```
   4) Run FreetuxTV
      ```
      freetuxtv
      ```
      
   ### Uninstalling FreetuxTV
   1) Uninstalling the program
      ```
      sudo make uninstall
      ```
   2) Remove FreetuxTV config files
      ```
      rm -rf ~/.config/FreetuxTV
      ```

