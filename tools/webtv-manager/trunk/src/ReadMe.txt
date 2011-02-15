                     FreetuxTv(WebTv-manager) by Eric Beuq
                      ==========

  Version 0.2.X - 15th February 2011
 
    Initiated by Badgeling and j.Delmes
    Site Web = http://code.google.com/p/freetuxtv/

  Contents
   0. Preamble 
   1. Presentation
   2. Installation
	2.1 New Installation
	2.2 Update form previous version
   3. Will you join us?

0. Preamble 
----------------------------------------------

Welcome on Webtv manager.


Happy reading,
Badgeling

  1. PRESENTATION
------------------

The goal of this project is to create a WebTV player, working on the Linux platform. It can be used by french people to watch ADSL TV on the PC with ISPs or by other people to watch WebTV and Web Radio in many languages. The GUI is developed in GTK2+ and is using the VLC engine to display the channels.

Features
Play channel TV and radio:
French ISP: Free, SFR, Orange, Alice, Bouygues...
Web TV: Arabic, Chinese, English, French, German, Italian, Japanese, Portuguese, Russian, Spain
Web Radio: English, French, German 
Play in full screen mode and miniature mode.
More than 365 logos of TV channels.
Support for keyboard multimedia keys.
Add custom groups of channels from M3U playlists.
Time shifting.
Recording live channels with ability to set duration.
Ability to create favorite channels groups.
Multi-language interface: English, French, German, Portugese
Features in future releases:

Programming Record
Program Management TV
Multi platform support

   2. Installation
-------------------

This paragraph on the first installation explains how to prepare a site for manage-webtv.

1. Upload all files and folders on your server.

2. Create a new Database Freetuxtv

3. Go to http://.../webtv-manager/trunk/src/index.php/user/install and valide 

4. Execute the database.sql on your sql.server

5. Edit ..\webtv-manager\trunk\src\protected\config\main.php 

	Search line [ 'debug'                 => true, //Turn it to false after user/installation.
]
	

	replace true by false!!

6. Congratulations, you just installed your site!


   3. Will you join us?
-------------------

Check http://code.google.com/p/freetuxtv/