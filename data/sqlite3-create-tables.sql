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
   uri_channelsgroup VARCHAR(500)
);
CREATE TABLE IF NOT EXISTS channel (
   id_channel INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
   name_channel VARCHAR(50) NOT NULL,
   order_channel INTEGER NOT NULL,
   idchannellogo_channel INTEGER NULL
     CONSTRAINT fk_idchannellogo_channel REFERENCES channel_logo(id_channellogo) ON DELETE SET NULL,
   uri_channel VARCHAR(255) NOT NULL,
   channelsgroup_channel INTEGER NOT NULL
     CONSTRAINT fk_channelsgroup_channel REFERENCES channels_group(id_channelsgroup) ON DELETE CASCADE
);

CREATE TRIGGER fkd_channel_channellogo_id
  BEFORE DELETE ON channel_logo
  FOR EACH ROW BEGIN
      UPDATE channel SET idchannellogo_channel = NULL WHERE idchannellogo_channel = OLD.id_channellogo;
      DELETE FROM label_channellogo WHERE idchannellogo_labelchannellogo = OLD.id_channellogo;
  END;

CREATE TRIGGER fkd_channel_channelsgroup_id
  BEFORE DELETE ON channels_group
  FOR EACH ROW BEGIN
      DELETE from channel WHERE channelsgroup_channel = OLD.id_channelsgroup;
  END;


insert into channels_group (name_channelsgroup, uri_channelsgroup)
			values ('FreeboxTV','http://mafreebox.freebox.fr/freeboxtv/playlist.m3u');
insert into channels_group (name_channelsgroup, uri_channelsgroup) 
			values ('FreeboxTV (non dégroupé)','http://www.eric-beuque.com/freetuxtv/playlists/playlist_freeboxtv_nd.m3u');
insert into channels_group (name_channelsgroup, uri_channelsgroup)
			values ('Neuf TV','http://www.n9ws.com/tools/tvsurpc.m3u');
insert into channels_group (name_channelsgroup, uri_channelsgroup)
			values ('Web TV (Française)','http://www.eric-beuque.com/freetuxtv/playlists/playlist_webtv_fr.m3u');

