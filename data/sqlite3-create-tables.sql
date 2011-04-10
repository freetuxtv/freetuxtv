-- @libdbevolution-lastdbversion=0.5.0.1

-- @libdbevolution-dbversion=0.1.0.1

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
   bregex_channelsgroup VARCHAR(50) NULL,
   eregex_channelsgroup VARCHAR(50) NULL,
   uri_channelsgroup VARCHAR(500) NULL
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

CREATE TRIGGER fkd_channellogo_id
  BEFORE DELETE ON channel_logo
  FOR EACH ROW BEGIN
      UPDATE channel SET idchannellogo_channel = NULL WHERE idchannellogo_channel = OLD.id_channellogo;
      DELETE FROM label_channellogo WHERE idchannellogo_labelchannellogo = OLD.id_channellogo;
  END;

CREATE TRIGGER fkd_channel_id
  BEFORE DELETE ON channels_group
  FOR EACH ROW BEGIN
      DELETE from channel WHERE channelsgroup_channel = OLD.id_channelsgroup;
  END;

-- @libdbevolution-dbversion=0.5.0.1

ALTER TABLE channels_group RENAME TO channels_group_tmp;
ALTER TABLE channel RENAME TO channel_tmp;

CREATE TABLE IF NOT EXISTS config (
   id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
   dbversion VARCHAR(50) NOT NULL
);

CREATE TABLE IF NOT EXISTS tvchannel (
   id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
   name VARCHAR(50) NOT NULL,
   langs VARCHAR(50),
   countries VARCHAR(50),
   logo_filename VARCHAR(20),
   website VARCHAR(255),
   tvguide_url VARCHAR(255)
);

CREATE TABLE IF NOT EXISTS label_tvchannel (
   id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
   label VARCHAR(50) NOT NULL,
   tvchannel_id INTEGER NOT NULL
     CONSTRAINT fk_labeltvchannel_tvchannelid REFERENCES tvchannel(id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS channels_group (
   id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
   position INTEGER NOT NULL,
   name VARCHAR(50) NOT NULL,
   type INTEGER NOT NULL,
   uri VARCHAR(500) NULL,
   bregex VARCHAR(50) NULL,
   eregex VARCHAR(50) NULL,
   last_update DATETIME NULL
);

CREATE TABLE IF NOT EXISTS channel (
   id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
   name VARCHAR(50) NOT NULL,
   position INTEGER NOT NULL,
   uri VARCHAR(255) NOT NULL,
   vlc_options TEXT NULL,
   views INTEGER NOT NULL DEFAULT 0,
   channelsgroup_id INTEGER NOT NULL
     CONSTRAINT fk_channel_channelsgroupid REFERENCES channels_group(id) ON DELETE CASCADE,
   tvchannel_id INTEGER NULL
     CONSTRAINT fk_channel_tvchannelid REFERENCES tvchannel(id) ON DELETE SET NULL
);

CREATE TRIGGER fkd_delele_tvchannel
  BEFORE DELETE ON tvchannel
  FOR EACH ROW BEGIN
      UPDATE channel SET tvchannel_id = NULL WHERE tvchannel_id = OLD.id;
      DELETE FROM label_tvchannel WHERE tvchannel_id = OLD.id;
  END;

CREATE TRIGGER fkd_delete_channelsgroup
  BEFORE DELETE ON channels_group
  FOR EACH ROW BEGIN
      DELETE from channel WHERE channelsgroup_id = OLD.id;
  END;

INSERT INTO tvchannel (id, name, logo_filename)
SELECT id_channellogo, label_channellogo, filename_channellogo
FROM channel_logo;

INSERT INTO label_tvchannel (id, label, tvchannel_id)
SELECT id_labelchannellogo, label_labelchannellogo,
   idchannellogo_labelchannellogo
FROM label_channellogo;

INSERT INTO channels_group (id, position, name, type, uri, bregex, eregex, last_update)
SELECT id_channelsgroup,
  (
   SELECT COUNT(0) FROM channels_group_tmp t1
   WHERE t1.id_channelsgroup <= channels_group_tmp.id_channelsgroup
  ) AS position,
  name_channelsgroup, 0, uri_channelsgroup,
  bregex_channelsgroup, eregex_channelsgroup, DATETIME('NOW', 'localtime')
FROM channels_group_tmp;

INSERT INTO channel (id, name, position, uri, channelsgroup_id, tvchannel_id)
SELECT id_channel, name_channel, order_channel, uri_channel, channelsgroup_channel, idchannellogo_channel
FROM channel_tmp;

DROP TRIGGER fkd_channel_id;
DROP TRIGGER fkd_channellogo_id;
DROP TABLE channel_tmp;
DROP TABLE channels_group_tmp;
DROP TABLE label_channellogo;
DROP TABLE channel_logo;

-- @libdbevolution-dbversion=0.5.1.1

ALTER TABLE channel ADD COLUMN deinterlace_mode VARCHAR(15) NULL;

-- @libdbevolution-dbversion=0.5.1.2

ALTER TABLE channel ADD COLUMN updated BOOLEAN NULL DEFAULT 1;

-- @libdbevolution-dbversion=0.6.0.1

CREATE TABLE IF NOT EXISTS recording (
   id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
   title VARCHAR(100) NOT NULL,
   begin_date DATETIME NOT NULL,
   end_date DATETIME NOT NULL,
   filename TEXT NULL,
   channel_id INTEGER NULL
     CONSTRAINT fk_recording_channelid REFERENCES channel(id) ON DELETE SET NULL
);

CREATE TRIGGER fkd_delele_channel
  BEFORE DELETE ON channel
  FOR EACH ROW BEGIN
      UPDATE recording SET channel_id = NULL WHERE channel_id = OLD.id;
  END;
  
-- @libdbevolution-dbversion=0.6.0.2

ALTER TABLE recording ADD COLUMN status INTEGER NOT NULL DEFAULT 1;