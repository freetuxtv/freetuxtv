-- Table for the TV Channel
CREATE TABLE IF NOT EXISTS tvchannel (
   id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
   name VARCHAR(50) NOT NULL,
   langs VARCHAR(50),
   countries VARCHAR(50),
   logo_filename VARCHAR(20),
   website VARCHAR(255),
   tvguide_url VARCHAR(255)
);

-- Table for alternative name for TV Channel 
CREATE TABLE IF NOT EXISTS label_tvchannel (
   id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
   label VARCHAR(50) NOT NULL,
   tvchannel_id INTEGER NOT NULL
     CONSTRAINT fk_labeltvchannel_tvchannelid REFERENCES tvchannel(id) ON DELETE CASCADE
);

-- Table for the group of Channel
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

-- Table for the Channel
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

