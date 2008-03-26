CREATE TABLE IF NOT EXISTS channel_logo (
   id_channellogo INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
   label_channellogo VARCHAR(50) NOT NULL,
   filename_channellogo VARCHAR(20)
);
CREATE TABLE IF NOT EXISTS label_channellogo (
   id_labelchannellogo INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
   label_labelchannellogo VARCHAR(50) NOT NULL,
   idchannellogo_labelchannellogo INTEGER NOT NULL
     CONSTRAINT fk_idchannellogo_labelchannellogo REFERENCES channel_logo(id_channellogo) ON DELETE CASCADE
);
CREATE TABLE IF NOT EXISTS channels_group (
   id_channelsgroup INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
   name_channelsgroup VARCHAR(50) NOT NULL,
   uri_channelsgroup VARCHAR(255) NOT NULL
);
CREATE TABLE IF NOT EXISTS channel (
   id_channel INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
   name_channel VARCHAR(50) NOT NULL,
   idchannellogo_channel INTEGER NULL
     CONSTRAINT fk_idchannellogo_channel REFERENCES channel_logo(id_channellogo) ON DELETE SET NULL,
   uri_channel VARCHAR(255) NOT NULL,
   channelsgroup_channel INTEGER NOT NULL
     CONSTRAINT fk_channelsgroup_channel REFERENCES channels_group(id_channelsgroup) ON DELETE CASCADE
);

insert into channels_group (name_channelsgroup, uri_channelsgroup)
			values ('FreeboxTV','http://mafreebox.freebox.fr/freeboxtv/playlist.m3u');
insert into channel (name_channel, idchannellogo_channel, uri_channel, channelsgroup_channel)
			values ('France 2',8,'rtsp://mafreebox.freebox.fr/freeboxtv/stream?id=201', 1);
insert into channel (name_channel, idchannellogo_channel, uri_channel, channelsgroup_channel)
			values ('France 3',10,'rtsp://mafreebox.freebox.fr/freeboxtv/stream?id=202', 1);
insert into channel (name_channel, idchannellogo_channel, uri_channel, channelsgroup_channel)
			values ('France 5',12,'rtsp://mafreebox.freebox.fr/freeboxtv/stream?id=203', 1);

insert into channels_group (name_channelsgroup, uri_channelsgroup) 
			values ('FreeboxTV (non dégroupé)','http://tv.freebox.fr/');
insert into channel (name_channel, idchannellogo_channel, uri_channel, channelsgroup_channel)
			values ('France 2',8,'http://tv.freebox.fr/stream_france2', 2);
insert into channel (name_channel, idchannellogo_channel, uri_channel, channelsgroup_channel)
			values ('France 3 national',10,'http://tv.freebox.fr/stream_france3', 2);
insert into channel (name_channel, idchannellogo_channel, uri_channel, channelsgroup_channel)
			values ('France 4',11,'http://tv.freebox.fr/stream_france4', 2);
insert into channel (name_channel, idchannellogo_channel, uri_channel, channelsgroup_channel)
			values ('France 5',12,'http://tv.freebox.fr/stream_france5', 2);
insert into channel (name_channel, idchannellogo_channel, uri_channel, channelsgroup_channel)
			values ('Direct 8',6,'http://tv.freebox.fr/stream_direct8', 2);
insert into channel (name_channel, idchannellogo_channel, uri_channel, channelsgroup_channel)
			values ('NT1',26,'http://tv.freebox.fr/stream_nt1', 2);
insert into channel (name_channel, idchannellogo_channel, uri_channel, channelsgroup_channel)
			values ('NRJ 12',25,'http://tv.freebox.fr/stream_nrj12', 2);
insert into channel (name_channel, idchannellogo_channel, uri_channel, channelsgroup_channel)
			values ('La Chaîne Parlementaire',18,'http://tv.freebox.fr/stream_lcp', 2);
insert into channel (name_channel, idchannellogo_channel, uri_channel, channelsgroup_channel)
			values ('BFM TV',4,'http://tv.freebox.fr/stream_bfmtv', 2);
insert into channel (name_channel, idchannellogo_channel, uri_channel, channelsgroup_channel)
			values ('TV5',30,'http://tv.freebox.fr/stream_tv5', 2);
insert into channel (name_channel, idchannellogo_channel, uri_channel, channelsgroup_channel)
			values ('France Ô',13,'http://tv.freebox.fr/stream_franceo', 2);
insert into channel (name_channel, idchannellogo_channel, uri_channel, channelsgroup_channel)
			values ('Al Jazeera',2,'http://tv.freebox.fr/stream_aljazeera', 2);

insert into channels_group (name_channelsgroup, uri_channelsgroup)
			values ('Web TV','http://mafreebox.freebox.fr/freeboxtv/playlist.m3u');
insert into channel (name_channel, idchannellogo_channel, uri_channel, channelsgroup_channel)
			values ('MCM Top',24,'mms://viplagardere.yacast.net/mcm_top', 3);
insert into channel (name_channel, idchannellogo_channel, uri_channel, channelsgroup_channel)
			values ('France 24',9,'mms://live.france24.com/france24_fr.wsx', 3);
insert into channel (name_channel, idchannellogo_channel, uri_channel, channelsgroup_channel)
			values ('BFM TV',4,'mms://vipmms9.yacast.net/bfm_bfmtv', 3);
insert into channel (name_channel, idchannellogo_channel, uri_channel, channelsgroup_channel)
			values ('Liberty TV',19,'mms://193.252.152.88/livefr', 3);

